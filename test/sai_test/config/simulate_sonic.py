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


def enabled():
    return os.environ.get("SIMULATE_SONIC", "0") == "1"


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

    disable_v6 = "/proc/sys/net/ipv6/conf/{}/disable_ipv6".format(be_if)
    accept_dad = "/proc/sys/net/ipv6/conf/{}/accept_dad".format(be_if)
    try:
        with open(disable_v6, "w", encoding="ascii") as fh:
            fh.write("0")
        with open(accept_dad, "w", encoding="ascii") as fh:
            fh.write("0")
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
