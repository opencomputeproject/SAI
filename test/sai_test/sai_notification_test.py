# Copyright (c) 2026 Microsoft Open Technologies, Inc.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
#    FOR A PARTICULAR PURPOSE, MERCHANTABILITY, NON-INFRINGEMENT.

"""Opt-in SAI notification tests."""

import os
import threading
import time

from ptf.testutils import dp_poll, send_packet, test_params_get
from unittest import SkipTest

from sai_thrift.sai_adapter import *
from sai_test_base import T0TestBase
from sai_utils import sai_ipaddress, sai_ipprefix


PORT_NOTIFICATION_TYPE = (
    sai_thrift_notification_type_t.SAI_THRIFT_NOTIFICATION_TYPE_PORT_STATE_CHANGE
)
BFD_NOTIFICATION_TYPE = (
    sai_thrift_notification_type_t.SAI_THRIFT_NOTIFICATION_TYPE_BFD_SESSION_STATE_CHANGE
)
NOTIFICATION_TEST_PARAM = "notification_test"
NOTIFICATION_TEST_VALUE = "true"
PLATFORM_PARAM = "platform"
VPP_PLATFORM = "vpp"
NOTIFICATION_TIMEOUT = 5.0
NOTIFICATION_POLL_INTERVAL = 0.5
BFD_NOTIFICATION_TIMEOUT = 25.0
VPPCTL_TIMEOUT = 10
RESPONDER_POLL_INTERVAL = 0.5
BFD_EPHEMERAL_SRC_PORT = 49152
BFD_STATE_INIT = 2
BFD_STATE_UP = 3
BFD_INTERVAL_USEC = 1000000
BFD_MULTIPLIER = 3


class NotificationTestBase(T0TestBase):
    """Common setup for the opt-in server-owned notification bridge."""

    def setUp(self, **kwargs):
        self.pending_events = []
        params = test_params_get() or {}
        if params.get(NOTIFICATION_TEST_PARAM) != NOTIFICATION_TEST_VALUE:
            super().setUp(skip_reason="SAI notification tests are opt-in")
            return

        T0TestBase.setUp(self, **kwargs)
        status = self.client.sai_thrift_enable_notifications()
        self.assertEqual(status, SAI_STATUS_SUCCESS)
        self.discard_pending_notifications()

    def tearDown(self):
        try:
            if self.client is not None:
                self.discard_pending_notifications()
        finally:
            super().tearDown()

    def discard_pending_notifications(self):
        self.pending_events = []
        self.client.sai_thrift_drain_notifications()

    def wait_for_notification(self, predicate, timeout=NOTIFICATION_TIMEOUT):
        """Return the first queued event matching predicate, keeping the rest.

        Events the caller is not waiting for yet are retained so that a drain
        which returns several transitions at once cannot lose the one a later
        assertion depends on.
        """
        deadline = time.monotonic() + timeout
        while True:
            for index, event in enumerate(self.pending_events):
                if predicate(event):
                    return self.pending_events.pop(index)

            if time.monotonic() >= deadline:
                break

            self.pending_events.extend(
                self.client.sai_thrift_drain_notifications()
            )
            time.sleep(NOTIFICATION_POLL_INTERVAL)

        self.fail(
            "timed out waiting for the expected SAI notification; observed {}".format(
                [
                    (event.notification_type, hex(event.object_id), event.state)
                    for event in self.pending_events
                ]
            )
        )


class PortNotificationTestBase(NotificationTestBase):
    """Port notification setup without unrelated L3 configuration."""

    def setUp(self):
        super().setUp(
            is_remove_default_vlan=False,
            is_create_vlan=False,
            is_create_fdb=False,
            is_create_default_route=False,
            is_create_lag=False,
            is_create_vlan_itf=False,
            is_create_route_for_vlan_itf=False,
            is_create_route_for_lag=False,
            wait_sec=1,
        )
        self.port = self.dut.port_obj_list[0]
        attributes = sai_thrift_get_port_attribute(
            self.client,
            port_oid=self.port.oid,
            admin_state=True,
            oper_status=True,
        )
        self.initial_admin_state = attributes["admin_state"]
        self.initial_oper_status = attributes["oper_status"]
        self.port_skip_reason = None
        if not self.initial_admin_state:
            self.port_skip_reason = "test fixture must provide an administratively up port"
        elif self.initial_oper_status != SAI_PORT_OPER_STATUS_UP:
            self.port_skip_reason = "test fixture must provide an operationally up port"
        self.discard_pending_notifications()

    def require_port_ready(self):
        if self.port_skip_reason:
            raise SkipTest(self.port_skip_reason)

    def port_event(self, state):
        return self.wait_for_notification(
            lambda event: event.notification_type == PORT_NOTIFICATION_TYPE
            and event.object_id == self.port.oid
            and event.state == state
        )

    def set_admin_state(self, admin_state):
        status = sai_thrift_set_port_attribute(
            self.client,
            port_oid=self.port.oid,
            admin_state=admin_state,
        )
        self.assertEqual(status, SAI_STATUS_SUCCESS)


class PortStateChangeTest(PortNotificationTestBase):
    """Verify a port admin-down event reaches the SAI callback bridge."""

    def runTest(self):
        self.require_port_ready()
        try:
            self.set_admin_state(False)
            self.port_event(SAI_PORT_OPER_STATUS_DOWN)
        finally:
            self.set_admin_state(self.initial_admin_state)


class PortStateRecoveryTest(PortNotificationTestBase):
    """Verify a port admin-down/admin-up sequence reaches SAI in order."""

    def runTest(self):
        self.require_port_ready()
        try:
            self.set_admin_state(False)
            self.port_event(SAI_PORT_OPER_STATUS_DOWN)
            self.set_admin_state(True)
            self.port_event(SAI_PORT_OPER_STATUS_UP)
        finally:
            self.set_admin_state(self.initial_admin_state)


class BfdResponder:
    """Respond to BFD packets received on PTF dataplane ports."""

    def __init__(
        self,
        test_obj,
        port_ids,
        local_ip,
        remote_ip,
        udp_port,
        discriminator,
    ):
        self.test_obj = test_obj
        self.port_ids = list(port_ids)
        self.local_ip = local_ip
        self.remote_ip = remote_ip
        self.udp_port = udp_port
        self.discriminator = discriminator
        self.stop_event = threading.Event()
        self.threads = []

    def start(self):
        # PTF's dataplane thread already queues packets per port, so a responder
        # thread that starts late still sees the BFD packets sent before it ran.
        for port_id in self.port_ids:
            thread = threading.Thread(
                target=self._run,
                args=(port_id,),
                daemon=True,
            )
            self.threads.append(thread)
            thread.start()

    def stop(self):
        self.stop_event.set()
        for thread in self.threads:
            thread.join(timeout=2.0)
        self.threads = []

    def _run(self, port_id):
        while not self.stop_event.is_set():
            result = dp_poll(
                self.test_obj,
                device_number=0,
                port_number=port_id,
                timeout=RESPONDER_POLL_INTERVAL,
            )
            if not isinstance(result, self.test_obj.dataplane.PollSuccess):
                continue
            self._respond(result.port, result.packet)

    def _respond(self, port_id, packet_data):
        from scapy.all import Ether, IP, UDP
        from scapy.contrib.bfd import BFD

        packet = Ether(packet_data)
        if not packet.haslayer(IP) or not packet.haslayer(UDP):
            return

        ip = packet[IP]
        udp = packet[UDP]
        if ip.src != self.local_ip or udp.dport != self.udp_port:
            return

        bfd = packet.getlayer(BFD)
        if bfd is None:
            try:
                bfd = BFD(bytes(udp.payload))
            except Exception:
                return

        response_state = (
            BFD_STATE_UP
            if bfd.sta in (BFD_STATE_INIT, BFD_STATE_UP)
            else BFD_STATE_INIT
        )
        response = (
            Ether(src=packet[Ether].dst, dst=packet[Ether].src)
            / IP(src=self.remote_ip, dst=self.local_ip, ttl=255)
            / UDP(sport=BFD_EPHEMERAL_SRC_PORT, dport=self.udp_port)
            / BFD(
                version=1,
                diag=0,
                sta=response_state,
                flags=0,
                detect_mult=BFD_MULTIPLIER,
                my_discriminator=self.discriminator,
                your_discriminator=bfd.my_discriminator,
                min_tx_interval=BFD_INTERVAL_USEC,
                min_rx_interval=BFD_INTERVAL_USEC,
                echo_rx_interval=0,
            )
        )
        send_packet(self.test_obj, port_id, response)


class BfdNotificationTestBase(NotificationTestBase):
    """Build a BFD session and peer through the shared SAI test topology."""

    local_ip = "10.1.1.1"
    gateway_peer_group = 1
    gateway_peer_id = 2
    remote_peer_group = 1
    remote_peer_id = 2
    multihop = False

    def setUp(self):
        params = test_params_get() or {}
        self.platform = params.get(PLATFORM_PARAM)
        self.bfd_session = None
        self.lag_rif = None
        self.owns_lag_rif = False
        self.neighbor_entry = None
        self.next_hop = None
        self.route_entry = None
        self.responder = None

        if (
            self.platform == VPP_PLATFORM
            and os.environ.get("SIMULATE_SONIC") != "1"
        ):
            super().setUp(
                skip_reason="VPP BFD notification tests require SIMULATE_SONIC=1"
            )
            return

        super().setUp(
            is_remove_default_vlan=False,
            is_create_vlan=False,
            is_create_fdb=False,
            is_create_default_route=False,
            is_create_lag=True,
            is_create_vlan_itf=False,
            is_create_route_for_vlan_itf=False,
            is_create_route_for_lag=False,
            wait_sec=1,
        )

        if not self.dut.default_vrf:
            self.route_configer.get_default_virtual_router()

        gateway_peer = self.t1_list[self.gateway_peer_group][self.gateway_peer_id]
        remote_peer = self.t1_list[self.remote_peer_group][self.remote_peer_id]
        self.gateway_ip = gateway_peer.ipv4
        self.remote_ip = remote_peer.ipv4
        self.udp_port = 4784 if self.multihop else 3784

        lag = self.dut.lag_list[0]
        existing_rifs = set(lag.rif_list or [])
        self.lag_rif = self.route_configer.create_router_interface(lag)
        self.owns_lag_rif = self.lag_rif not in existing_rifs
        self.peer_port_ids = self.get_dev_port_indexes(lag.member_port_indexs)
        self.peer_mac = gateway_peer.mac

        self.neighbor_entry = sai_thrift_neighbor_entry_t(
            rif_id=self.lag_rif,
            ip_address=sai_ipaddress(self.gateway_ip),
        )
        status = sai_thrift_create_neighbor_entry(
            self.client,
            self.neighbor_entry,
            dst_mac_address=self.peer_mac,
            no_host_route=False,
        )
        self.assertEqual(status, SAI_STATUS_SUCCESS)

        if self.multihop:
            self.next_hop = sai_thrift_create_next_hop(
                self.client,
                ip=sai_ipaddress(self.gateway_ip),
                router_interface_id=self.lag_rif,
                type=SAI_NEXT_HOP_TYPE_IP,
            )
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
            self.route_entry = sai_thrift_route_entry_t(
                vr_id=self.dut.default_vrf,
                destination=sai_ipprefix(self.remote_ip + "/32"),
            )
            status = sai_thrift_create_route_entry(
                self.client,
                self.route_entry,
                next_hop_id=self.next_hop,
            )
            self.assertEqual(status, SAI_STATUS_SUCCESS)

    def start_session(self):
        self.responder = BfdResponder(
            self,
            self.peer_port_ids,
            self.local_ip,
            self.remote_ip,
            self.udp_port,
            0x2001,
        )
        self.responder.start()

        try:
            self.bfd_session = sai_thrift_create_bfd_session(
                self.client,
                type=SAI_BFD_SESSION_TYPE_ASYNC_ACTIVE,
                virtual_router=self.dut.default_vrf,
                local_discriminator=0x1001,
                remote_discriminator=0x2001,
                udp_src_port=BFD_EPHEMERAL_SRC_PORT,
                bfd_encapsulation_type=SAI_BFD_ENCAPSULATION_TYPE_NONE,
                iphdr_version=4,
                src_ip_address=sai_ipaddress(self.local_ip),
                dst_ip_address=sai_ipaddress(self.remote_ip),
                min_tx=BFD_INTERVAL_USEC,
                min_rx=BFD_INTERVAL_USEC,
                multiplier=BFD_MULTIPLIER,
                hw_lookup_valid=True,
                multihop=self.multihop,
                cbit=False,
                admin_state=True,
            )
            self.assertNotEqual(self.bfd_session, SAI_NULL_OBJECT_ID)
            self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        except Exception:
            self.stop_peer()
            raise

    def stop_peer(self):
        if self.responder is not None:
            self.responder.stop()
            self.responder = None

    @staticmethod
    def vppctl(*command):
        import subprocess

        result = subprocess.run(
            ["vppctl"] + list(command),
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            timeout=VPPCTL_TIMEOUT,
        )
        return result.stdout.decode("utf-8", "replace")

    def log_vpp_state(self, *command):
        output = self.vppctl(*command)
        print("vppctl {}:\n{}".format(" ".join(command), output))
        return output

    def assert_vpp_bfd_state(self, expected_state_word):
        output = self.log_vpp_state("show", "bfd", "sessions")
        self.assertIn(
            self.remote_ip,
            output,
            "VPP has no BFD session towards {}".format(self.remote_ip),
        )
        self.assertRegex(
            output,
            r"(?i)\b{}\b".format(expected_state_word),
            "VPP did not report BFD state {}".format(expected_state_word),
        )

    # A platform opts in to dataplane corroboration by adding its own entry.
    PLATFORM_BFD_STATE_CHECKS = {
        VPP_PLATFORM: assert_vpp_bfd_state,
    }

    def assert_platform_bfd_state(self, expected_state_word):
        check = self.PLATFORM_BFD_STATE_CHECKS.get(self.platform)
        if check is not None:
            check(self, expected_state_word)

    def tearDown(self):
        try:
            self.stop_peer()
            if self.bfd_session is not None:
                sai_thrift_remove_bfd_session(self.client, self.bfd_session)
            if self.route_entry is not None:
                sai_thrift_remove_route_entry(self.client, self.route_entry)
            if self.next_hop is not None:
                sai_thrift_remove_next_hop(self.client, self.next_hop)
            if self.neighbor_entry is not None:
                sai_thrift_remove_neighbor_entry(self.client, self.neighbor_entry)
            if self.owns_lag_rif and self.lag_rif is not None:
                sai_thrift_remove_router_interface(self.client, self.lag_rif)
        finally:
            super().tearDown()

    def bfd_event(self, state):
        return self.wait_for_notification(
            lambda event: event.notification_type == BFD_NOTIFICATION_TYPE
            and event.object_id == self.bfd_session
            and event.state == state,
            timeout=BFD_NOTIFICATION_TIMEOUT,
        )

    def assert_bfd_state(self, state):
        attributes = sai_thrift_get_bfd_session_attribute(
            self.client,
            self.bfd_session,
            state=True,
        )
        self.assertEqual(attributes["state"], state)

    def bring_session_up(self):
        self.start_session()
        self.bfd_event(SAI_BFD_SESSION_STATE_UP)
        self.assert_bfd_state(SAI_BFD_SESSION_STATE_UP)
        self.assert_platform_bfd_state("up")

    def break_path(self):
        self.stop_peer()
        self.bfd_event(SAI_BFD_SESSION_STATE_DOWN)
        self.assert_bfd_state(SAI_BFD_SESSION_STATE_DOWN)
        self.assert_platform_bfd_state("down")


class BfdSessionUpTest(BfdNotificationTestBase):
    """Verify that a responder-driven BFD session emits an UP notification."""

    def runTest(self):
        self.bring_session_up()


class BfdSessionDownTest(BfdNotificationTestBase):
    """Verify that stopping the responder emits a BFD DOWN notification."""

    def runTest(self):
        self.bring_session_up()
        self.break_path()


class BfdMultihopTest(BfdNotificationTestBase):
    """Verify multihop BFD uses UDP/4784 and a routed lookup."""

    remote_peer_group = 2
    multihop = True

    def runTest(self):
        self.bring_session_up()
        self.break_path()
