# Copyright (c) 2026 Microsoft Open Technologies, Inc.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
#    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
#
#    See the Apache Version 2.0 License for specific language governing
#    permissions and limitations under the License.
#
#    Microsoft would like to thank the following companies for their review and
#    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
#    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
#
#

# Optional SONiC control-plane simulation for standalone sai_test benches.
#
# In a real SONiC device teamd creates PortChannel netdevs and IntfMgr assigns the
# interface IPs that the SAI backend mirrors onto LAG/SVI router interfaces. A
# standalone sai_test bench (no teamd / IntfMgr) has neither, so LAG bring-up and
# routed-to-LAG/SVI forwarding can fail. When SIMULATE_SONIC=1 the config helpers
# call into this module to emulate that setup from the test's setUp path.
#
# This is opt-in and default-preserving: with SIMULATE_SONIC unset (the default) all
# entry points are no-ops, so real ASICs and other harnesses are unaffected. All
# interface names and address patterns are environment-overridable so the defaults
# can be retargeted without code changes. This module is backend-neutral: it uses
# standard Linux "ip" for PortChannel netdevs and LAG router-interface IPs, and for
# a VLAN (SVI) router interface -- which has no host-interface netdev to mirror from
# -- it runs the command templates the harness provides via SVI_RIF_SET_IP_CMD (and
# optional SVI_RIF_PROBE_CMD), so no backend-specific tooling is hardcoded here.

import logging
import os
import shlex
import subprocess
import time

_logger = logging.getLogger(__name__)
_IPV6_CONTROL_FILTER_INSTALLED = False
_ROUTER_SOLICITATION_DESTINATION = bytes.fromhex("ff020000000000000000000000000002")
_MLDV2_REPORT_DESTINATION = bytes.fromhex("ff020000000000000000000000000016")


def enabled():
    return os.environ.get("SIMULATE_SONIC", "0") == "1"


def _is_ipv6_control_packet(packet_data, source_mac):
    """Return whether packet_data is a simulated router RS or MLDv2 frame."""
    packet_bytes = bytes(packet_data)
    ethernet_length = 14
    ipv6_header_length = 40
    payload_offset = ethernet_length + ipv6_header_length

    if len(packet_bytes) <= payload_offset:
        return False
    if packet_bytes[6:12] != source_mac or packet_bytes[12:14] != b"\x86\xdd":
        return False

    destination = packet_bytes[ethernet_length + 24:ethernet_length + 40]
    next_header = packet_bytes[ethernet_length + 6]
    if next_header == 0:
        if len(packet_bytes) < payload_offset + 2:
            return False
        next_header = packet_bytes[payload_offset]
        payload_offset += (packet_bytes[payload_offset + 1] + 1) * 8
    if next_header != 58 or len(packet_bytes) <= payload_offset:
        return False

    icmpv6_type = packet_bytes[payload_offset]
    return (
        destination == _ROUTER_SOLICITATION_DESTINATION and icmpv6_type == 133
    ) or (
        destination == _MLDV2_REPORT_DESTINATION and icmpv6_type == 143
    )


def install_ipv6_control_filter():
    """Discard simulated-router IPv6 startup frames when the harness opts in."""
    global _IPV6_CONTROL_FILTER_INSTALLED

    source_mac = os.environ.get("SIMULATE_SONIC_IPV6_CONTROL_SRC_MAC", "")
    if _IPV6_CONTROL_FILTER_INSTALLED or not enabled() or not source_mac:
        return

    from ptf.testutils import add_filter

    try:
        source_mac_bytes = bytes.fromhex(source_mac.replace(":", ""))
    except ValueError:
        _logger.warning("invalid SIMULATE_SONIC_IPV6_CONTROL_SRC_MAC: %s", source_mac)
        return
    if len(source_mac_bytes) != 6:
        _logger.warning("invalid SIMULATE_SONIC_IPV6_CONTROL_SRC_MAC: %s", source_mac)
        return

    def keep_non_control_packet(packet_data):
        return not _is_ipv6_control_packet(packet_data, source_mac_bytes)

    add_filter(keep_non_control_packet)
    _IPV6_CONTROL_FILTER_INSTALLED = True


def _run(cmd, check=False, quiet=False):
    result = subprocess.run(cmd, check=False, capture_output=True, text=True)
    if result.returncode != 0 and not quiet:
        msg = "command failed (rc={}): {}".format(result.returncode, cmd)
        stderr = result.stderr.strip() if result.stderr else ""
        if stderr:
            msg = "{} stderr: {}".format(msg, stderr)
        if check:
            _logger.error(msg)
            result.check_returncode()
        else:
            _logger.warning(msg)
    elif check:
        result.check_returncode()
    return result


def _mtu():
    return int(os.environ.get("MTU", "9100"))


def _templated(cmd_tmpl, **kw):
    """Split a command template and substitute {ifname}/{addr} placeholders."""
    return shlex.split(cmd_tmpl.format(**kw))


def ensure_portchannel(lag_index):
    """Create the PortChannel<lag_index> netdev teamd would provide for a LAG."""
    if not enabled():
        return

    install_ipv6_control_filter()

    pc_if = "PortChannel{}".format(lag_index)
    mtu = _mtu()

    if _run(["ip", "link", "show", pc_if], quiet=True).returncode != 0:
        if _run(["ip", "link", "add", pc_if, "type", "bond"]).returncode != 0:
            _run(["ip", "link", "add", pc_if, "type", "dummy"], check=False)

    _run(["ip", "link", "set", "dev", pc_if, "mtu", str(mtu)], check=False)
    _run(["ip", "link", "set", "dev", pc_if, "up"], check=False)


def assign_lag_rif_ips(lag_index, retries=20, interval=0.3):
    """Assign the connected IPs IntfMgr would put on a LAG router interface.

    The IP is placed on the backend's LAG host-interface netdev (default "be<N>"),
    which the backend mirrors onto the LAG router interface.
    """
    if not enabled() or os.environ.get("LAG_RIF_IPS", "1") != "1":
        return

    install_ipv6_control_filter()

    be_prefix = os.environ.get("LAG_BE_TAP_PREFIX", "be")
    v4_pattern = os.environ.get("LAG_RIF_IPV4_PATTERN", "10.1.%d.1/24")
    v6_pattern = os.environ.get("LAG_RIF_IPV6_PATTERN", "fc00:1::%d:1/112")
    subnet_id = lag_index + 1
    be_if = "{}{}".format(be_prefix, lag_index)
    v4_addr = v4_pattern % subnet_id
    v6_addr = v6_pattern % subnet_id

    for _ in range(retries):
        if _run(["ip", "link", "show", be_if], quiet=True).returncode == 0:
            break
        time.sleep(interval)
    else:
        return

    ipv6_conf = "/proc/sys/net/ipv6/conf/{}".format(be_if)
    # Re-enabling IPv6 creates the link-local address immediately, so apply all
    # host-control settings first to prevent DAD and router discovery packets.
    settings = (
        ("accept_dad", "0"),
        ("accept_ra", "0"),
        ("autoconf", "0"),
        ("router_solicitations", "0"),
        ("disable_ipv6", "0"),
    )
    for setting, value in settings:
        try:
            with open(os.path.join(ipv6_conf, setting), "w", encoding="ascii") as fh:
                fh.write(value)
        except OSError:
            pass

    _run(["ip", "addr", "add", v4_addr, "dev", be_if], check=False)
    _run(["ip", "-6", "addr", "add", v6_addr, "dev", be_if, "nodad"], check=False)


def _svi_group_id(vlan_id):
    for entry in os.environ.get("SVI_RIF_VLANS", "10:1 20:2").split():
        vid, gid = entry.split(":", 1)
        if int(vid) == int(vlan_id):
            return int(gid)
    return int(vlan_id)


def assign_svi_rif_ips(vlan_id, retries=20, interval=0.3):
    """Assign the connected IPs IntfMgr would put on a VLAN (SVI) router interface.

    A VLAN SVI has no host-interface netdev to mirror from, so the IP must be set on
    the backend interface directly. The backend-specific commands are injected by the
    harness as templates (with {ifname}/{addr} placeholders): SVI_RIF_SET_IP_CMD sets
    the address, and the optional SVI_RIF_PROBE_CMD waits until the interface exists.
    With SVI_RIF_SET_IP_CMD unset this is a no-op, keeping this module backend-neutral.
    """
    if not enabled() or os.environ.get("SVI_RIF_IPS", "1") != "1":
        return

    install_ipv6_control_filter()

    set_ip_cmd = os.environ.get("SVI_RIF_SET_IP_CMD")
    if not set_ip_cmd:
        return

    probe_cmd = os.environ.get("SVI_RIF_PROBE_CMD")
    bvi_prefix = os.environ.get("SVI_BVI_PREFIX", "bvi")
    v4_pattern = os.environ.get("SVI_RIF_IPV4_PATTERN", "192.168.%d.1/24")
    v6_pattern = os.environ.get("SVI_RIF_IPV6_PATTERN", "fc02::%d:1/112")
    group_id = _svi_group_id(vlan_id)
    bvi_if = "{}{}".format(bvi_prefix, vlan_id)
    v4_addr = v4_pattern % group_id
    v6_addr = v6_pattern % group_id

    if probe_cmd:
        for _ in range(retries):
            if _run(_templated(probe_cmd, ifname=bvi_if, addr=""), quiet=True).returncode == 0:
                break
            time.sleep(interval)
        else:
            return

    for addr in (v4_addr, v6_addr):
        _run(_templated(set_ip_cmd, ifname=bvi_if, addr=addr), check=False)
