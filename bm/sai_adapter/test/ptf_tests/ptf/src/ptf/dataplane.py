"""
OpenFlow Test Framework

DataPlane and DataPlanePort classes

Provide the interface to the control the set of ports being used
to stimulate the switch under test.

See the class dataplaneport for more details.  This class wraps
a set of those objects allowing general calls and parsing
configuration.

@todo Add "filters" for matching packets.  Actions supported
for filters should include a callback or a counter
"""

import sys
import os
import socket
import time
import select
import logging
import struct
from collections import defaultdict
from threading import Thread
from threading import Lock
from threading import Condition
import ptfutils
import netutils
import mask
from pcap_writer import PcapWriter

try:
    import nnpy
    with_nnpy = True
except ImportError:
    with_nnpy = False

if "linux" in sys.platform:
    import afpacket
else:
    import pcap

have_pypcap = False
# See Jira issue TSW-13
#try:
#    import pcap
#    if hasattr(pcap, "pcap"):
#        # the incompatible pylibpcap library masquerades as pcap
#        have_pypcap = True
#except:
#    pass

def match_exp_pkt(exp_pkt, pkt):
    """
    Compare the string value of pkt with the string value of exp_pkt,
    and return True iff they are identical.  If the length of exp_pkt is
    less than the minimum Ethernet frame size (60 bytes), then padding
    bytes in pkt are ignored.
    """
    if isinstance(exp_pkt, mask.Mask):
        if not exp_pkt.is_valid():
            return False
        return exp_pkt.pkt_match(pkt)
    e = str(exp_pkt)
    p = str(pkt)
    if len(e) < 60:
        p = p[:len(e)]
    return e == p


class DataPlanePacketSourceIface:
    """
    Interface for an object that can be passed to select and on which packets
    can be received. This was introduced so that several ports can share the
    same packet 'source'
    """
    def fileno(self):
        """
        Return an integer file descriptor that can be passed to select(2).
        """
        raise NotImplementedError()

    def recv(self):
        """
        Receive a packet from this port.
        @retval (device, port, packet data, timestamp)
        """
        raise NotImplementedError()


class DataPlanePortIface:
    def get_packet_source(self):
        """
        @retval An object implementing DataPlanePacketSourceIface
        """
        raise NotImplementedError()

    def send(self, packet):
        """
        Send a packet out this port.
        @param packet The packet data to send to the port
        @retval The number of bytes sent
        """
        raise NotImplementedError()

    def down(self):
        """
        Bring the physical link down.
        """
        raise NotImplementedError()

    def up(self):
        """
        Bring the physical link up.
        """
        raise NotImplementedError()


class DataPlanePortLinux(DataPlanePortIface, DataPlanePacketSourceIface):
    """
    Uses raw sockets to capture and send packets on a network interface.
    """

    RCV_SIZE_DEFAULT = 4096
    ETH_P_ALL = 0x03
    RCV_TIMEOUT = 10000

    def __init__(self, interface_name, device_number, port_number):
        """
        @param interface_name The name of the physical interface like eth1
        """
        self.interface_name = interface_name
        self.device_number = device_number
        self.port_number = port_number
        self.socket = socket.socket(socket.AF_PACKET, socket.SOCK_RAW, 0)
        afpacket.enable_auxdata(self.socket)
        self.socket.bind((interface_name, self.ETH_P_ALL))
        netutils.set_promisc(self.socket, interface_name)
        self.socket.settimeout(self.RCV_TIMEOUT)

    def __del__(self):
        if self.socket:
            self.socket.close()

    def fileno(self):
        """
        Return an integer file descriptor that can be passed to select(2).
        """
        return self.socket.fileno()

    def recv(self):
        """
        Receive a packet from this port.
        @retval (device, port, packet data, timestamp)
        """
        pkt = afpacket.recv(self.socket, self.RCV_SIZE_DEFAULT)
        return (self.device_number, self.port_number, pkt, time.time())

    def get_packet_source(self):
        """
        @retval An object implementing DataPlanePacketSourceIface
        """
        return self

    def send(self, packet):
        """
        Send a packet out this port.
        @param packet The packet data to send to the port
        @retval The number of bytes sent
        """
        return self.socket.send(packet)

    def down(self):
        """
        Bring the physical link down.
        """
        os.system("ifconfig %s down" % self.interface_name)

    def up(self):
        """
        Bring the physical link up.
        """
        os.system("ifconfig %s up" % self.interface_name)

    def mac(self):
        """
        Return mac address
        """
        return netutils.get_mac(self.interface_name)


class DataPlanePacketSourceNN(DataPlanePacketSourceIface):
    """
    Wrapper class around nnpy used to capture data packets, send data packets
    and send port status messages. It implements DataPlanePacketSourceIface by
    exposing nanomsg receive file descriptor (RCVFD).
    Note that there has to be a 1-1 mapping between device and nanomsg
    socket. This is because the device number is not included in the PACKET_OUT
    messages. Maybe something to add in the future?
    """

    MSG_TYPE_PORT_ADD = 0
    MSG_TYPE_PORT_REMOVE = 1
    MSG_TYPE_PORT_SET_STATUS = 2
    MSG_TYPE_PACKET_IN = 3
    MSG_TYPE_PACKET_OUT = 4
    MSG_TYPE_INFO_REQ = 5
    MSG_TYPE_INFO_REP = 6

    MSG_PORT_STATUS_UP = 0
    MSG_PORT_STATUS_DOWN = 1

    MSG_INFO_TYPE_HWADDR = 0
    MSG_INFO_TYPE_CTRS = 1

    MSG_INFO_STATUS_SUCCESS = 0
    MSG_INFO_STATUS_NOT_SUPPORTED = 1

    def __init__(self, device_number, socket_addr, rcv_timeout):
        self.device_number = device_number
        self.socket_addr = socket_addr
        self.socket = nnpy.Socket(nnpy.AF_SP, nnpy.PAIR)
        self.socket.connect(socket_addr)
        self.rcv_timeout = rcv_timeout
        self.socket.setsockopt(nnpy.SOL_SOCKET, nnpy.RCVTIMEO, rcv_timeout)
        self.buffers = defaultdict(list)
        self.cvar = Condition()
        self.mac_addresses = {}
        self.nn_counters = {}

    def close(self):
        # TODO(antonin): something to do?
        pass

    def fileno(self):
        """
        Return an integer file descriptor that can be passed to select(2).
        """
        return self.socket.getsockopt(nnpy.SOL_SOCKET, nnpy.RCVFD)

    def __send_port_msg(self, msg_type, port_number, more):
        hdr = struct.pack("<iii", msg_type, port_number, more)
        self.socket.send(hdr)

    def __send_info_req_msg(self, port_number, info_type):
        self.__send_port_msg(self.MSG_TYPE_INFO_REQ, port_number, info_type)

    def __request_mac(self, port_number):
        self.__send_info_req_msg(port_number, self.MSG_INFO_TYPE_HWADDR)

    def __request_ctrs(self, port_number):
        self.__send_info_req_msg(port_number, self.MSG_INFO_TYPE_CTRS)

    def port_add(self, port_number):
        self.__send_port_msg(self.MSG_TYPE_PORT_ADD, port_number, 0)

    def port_remove(self, port_number):
        self.__send_port_msg(self.MSG_TYPE_PORT_REMOVE, port_number, 0)

    def port_bring_up(self, port_number):
        self.__send_port_msg(self.MSG_TYPE_PORT_SET_STATUS, port_number,
                             self.MSG_PORT_STATUS_UP)

    def port_bring_down(self, port_number):
        self.__send_port_msg(self.MSG_TYPE_PORT_SET_STATUS, port_number,
                             self.MSG_PORT_STATUS_DOWN)

    def __handle_info_rep(self, port_number, info_type, msg):
        fmt = "<i"
        status, = struct.unpack_from(fmt, msg)
        if status != self.MSG_INFO_STATUS_SUCCESS:
            msg = None
        else:
            msg = msg[struct.calcsize(fmt):]
        if info_type == self.MSG_INFO_TYPE_HWADDR:
            with self.cvar:
                self.mac_addresses[port_number] = msg
                self.cvar.notify_all()
        elif info_type == self.MSG_INFO_TYPE_CTRS:
            with self.cvar:
                self.nn_counters[port_number] = struct.unpack_from('<ii', msg)
                self.cvar.notify_all()

    def recv(self):
        msg = self.socket.recv()
        fmt = "<iii"
        msg_type, port_number, more = struct.unpack_from(fmt, msg)
        hdr_size = struct.calcsize(fmt)
        msg = msg[hdr_size:]
        if msg_type == self.MSG_TYPE_INFO_REP:
            self.__handle_info_rep(port_number, more, msg)
            # we return None (not a data packet)
            return
        assert (msg_type == self.MSG_TYPE_PACKET_OUT)
        assert (len(msg) == more)
        return (self.device_number, port_number, msg, time.time())

    def send(self, port_number, packet):
        msg = struct.pack("<iii%ds" % len(packet), self.MSG_TYPE_PACKET_IN,
                          port_number, len(packet), packet)
        # because nnpy expects unicode when using str
        msg = list(msg)
        self.socket.send(msg)
        # nnpy does not return the number of bytes sent
        return len(packet)

    def get_info(self, port_number, cache, send_request, timeout=2):
        # we use a timeout in case other endpoint does not reply
        end = time.time() + timeout
        with self.cvar:
            time_remaining = end - time.time()
            while port_number not in cache and time_remaining > 0:
                send_request(port_number)
                self.cvar.wait(time_remaining)
                time_remaining = end - time.time()
            return cache.get(port_number, None)

    def get_mac(self, port_number, timeout=2):
        return self.get_info(port_number, self.mac_addresses, self.__request_mac, timeout)

    def get_nn_counters(self, port_number, timeout=2):
        if port_number in self.nn_counters:
            del self.nn_counters[port_number]
        return self.get_info(port_number, self.nn_counters, self.__request_ctrs, timeout)


class DataPlanePortNN(DataPlanePortIface):
    """
    Uses nanomsg sockets to capture and send packets (through IPC or TCP)
    """

    RCV_TIMEOUT = 10000

    # indexed by device_number, maps to a PacketInjectNN instance
    packet_injecters = {}

    def __init__(self, interface_name, device_number, port_number):
        """
        @param interface_name The addr of the socket (like ipc://<path to file>
        or tcp://<iface>:<port>)
        """
        self.interface_name = interface_name
        if device_number not in self.packet_injecters:
            self.packet_injecters[device_number] = DataPlanePacketSourceNN(
                device_number, interface_name, self.RCV_TIMEOUT)
        self.packet_inject = self.packet_injecters[device_number]
        self.port_number = port_number
        self.device_number = device_number
        self.packet_inject.port_add(port_number)

    def __del__(self):
        if self.packet_inject:
            self.packet_inject.port_remove(self.port_number)

    def get_packet_source(self):
        """
        @retval An object implementing DataPlanePacketSourceIface
        """
        return self.packet_injecters[self.device_number]

    def send(self, packet):
        """
        Send a packet out this port.
        @param packet The packet data to send to the port
        @retval The number of bytes sent
        """
        return self.packet_injecters[self.device_number].send(
            self.port_number, packet)

    def down(self):
        """
        Bring the physical link down.
        """
        self.packet_injecters[self.device_number].port_bring_down(
            self.port_number)

    def up(self):
        """
        Bring the physical link up.
        """
        self.packet_injecters[self.device_number].port_bring_up(
            self.port_number)

    def mac(self):
        """
        Return mac address
        """
        return self.packet_injecters[self.device_number].get_mac(
            self.port_number)

    def nn_counters(self):
        """
        Return counters
        """
        return self.packet_injecters[self.device_number].get_nn_counters(
            self.port_number)


class DataPlanePort(DataPlanePortIface, DataPlanePacketSourceIface):
    """
    Uses raw sockets to capture and send packets on a network interface.
    """

    RCV_SIZE_DEFAULT = 4096
    ETH_P_ALL = 0x03
    RCV_TIMEOUT = 10000

    def __init__(self, interface_name, device_number, port_number):
        """
        @param interface_name The name of the physical interface like eth1
        """
        self.interface_name = interface_name
        self.device_number = device_number
        self.port_number = port_number
        self.socket = socket.socket(socket.AF_PACKET, socket.SOCK_RAW,
                                    socket.htons(self.ETH_P_ALL))
        self.socket.bind((interface_name, 0))
        netutils.set_promisc(self.socket, interface_name)
        self.socket.settimeout(self.RCV_TIMEOUT)

    def __del__(self):
        if self.socket:
            self.socket.close()

    def fileno(self):
        """
        Return an integer file descriptor that can be passed to select(2).
        """
        return self.socket.fileno()

    def recv(self):
        """
        Receive a packet from this port.
        @retval (device, port, packet data, timestamp)
        """
        pkt = self.socket.recv(self.RCV_SIZE_DEFAULT)
        return (self.device_number, self.port_number, pkt, time.time())

    def get_packet_source(self):
        """
        @retval An object implementing DataPlanePacketSourceIface
        """
        return self

    def send(self, packet):
        """
        Send a packet out this port.
        @param packet The packet data to send to the port
        @retval The number of bytes sent
        """
        return self.socket.send(packet)

    def down(self):
        """
        Bring the physical link down.
        """
        os.system("ifconfig %s down" % self.interface_name)

    def up(self):
        """
        Bring the physical link up.
        """
        os.system("ifconfig %s up" % self.interface_name)

    def mac(self):
        """
        Return mac address
        """
        return netutils.get_mac(self.interface_name)

class DataPlanePortPcap:
    """
    Alternate port implementation using libpcap. This is required for recent
    versions of Linux (such as Linux 3.2 included in Ubuntu 12.04) which
    offload the VLAN tag, so it isn't in the data returned from a read on a raw
    socket. libpcap understands how to read the VLAN tag from the kernel.
    """

    def __init__(self, interface_name, device_number, port_number):
        self.device_number = device_number
        self.port_number = port_number
        self.pcap = pcap.pcap(interface_name)
        self.pcap.setnonblock()

    def fileno(self):
        return self.pcap.fileno()

    def recv(self):
        (timestamp, pkt) = next(self.pcap)
        return (self.device_number, self.port_number, pkt[:], timestamp)

    def get_packet_source(self):
        return self

    def send(self, packet):
        return self.pcap.inject(packet, len(packet))

    def down(self):
        pass

    def up(self):
        pass

class DataPlane(Thread):
    """
    This class provides methods to send and receive packets on the dataplane.
    It uses the DataPlanePort class, or an alternative implementation of that
    interface, to do IO on a particular port. A background thread is used to
    read packets from the dataplane ports and enqueue them to be read by the
    test. The kill() method must be called to shutdown this thread.
    """

    MAX_QUEUE_LEN = 100

    def __init__(self, config=None):
        Thread.__init__(self)

        # dict from device number, port number to port object
        self.ports = {}

        # dict from device number, port number to list of (timestamp, packet)
        self.packet_queues = {}

        # counters of received packets (may include packets which were dropped due to queue overflow)
        self.rx_counters = defaultdict(int)

        # counters of transmited packets
        self.tx_counters = defaultdict(int)

        # cvar serves double duty as a regular top level lock and
        # as a condition variable
        self.cvar = Condition()

        # Used to wake up the event loop from another thread
        self.waker = ptfutils.EventDescriptor()
        self.killed = False

        self.logger = logging.getLogger("dataplane")
        self.pcap_writer = None


        if config is None:
            self.config = {}
        else:
            self.config = config;

        ############################################################
        #
        # The platform/config can provide a custom DataPlanePort class
        # here if you have a custom implementation with different
        # behavior.
        #
        # Set config.dataplane.portclass = MyDataPlanePortClass
        # where MyDataPlanePortClass has the same interface as the class
        # DataPlanePort defined here.
        #
        if self.config["platform"] == "nn":
            # assert is ok here because this is caught earlier in ptf
            assert(with_nnpy == True)
            self.dppclass = DataPlanePortNN
        elif "dataplane" in self.config and "portclass" in self.config["dataplane"]:
            self.dppclass = self.config["dataplane"]["portclass"]
        elif "linux" in sys.platform:
            self.dppclass = DataPlanePortLinux
        elif have_pypcap:
            self.dppclass = DataPlanePortPcap
        else:
            self.logger.warning("Missing pypcap, VLAN tests may fail. See README for installation instructions.")
            self.dppclass = DataPlanePort

        if "qlen" in self.config:
            self.qlen = self.config["qlen"]
        else:
            self.qlen = self.MAX_QUEUE_LEN

        self.start()

    def run(self):
        """
        Activity function for class
        """
        while not self.killed:
            sockets = set([p.get_packet_source() for p in self.ports.values()])
            sockets.add(self.waker)
            try:
                sel_in, sel_out, sel_err = select.select(sockets, [], [], 1)
            except:
                print sys.exc_info()
                self.logger.error("Select error, exiting")
                break

            with self.cvar:
                for sel in sel_in:
                    if sel == self.waker:
                        self.waker.wait()
                        continue
                    else:
                        # Enqueue packet
                        t = sel.recv()
                        if t is None:
                            continue
                        device_number, port_number, pkt, timestamp = t
                        self.logger.debug("Pkt len %d in on device %d, port %d",
                                          len(pkt), device_number, port_number)
                        if self.pcap_writer:
                            self.pcap_writer.write(pkt, timestamp,
                                                   device_number, port_number)
                        queue = self.packet_queues[(device_number, port_number)]
                        if len(queue) >= self.qlen:
                            # Queue full, throw away oldest
                            queue.pop(0)
                            self.logger.debug("Discarding oldest packet to make room")
                        queue.append((pkt, timestamp))
                        self.rx_counters[(device_number, port_number)] += 1
                self.cvar.notify_all()

        self.logger.info("Thread exit")

    def set_qlen(self, qlen):
        self.qlen = qlen

    def port_add(self, interface_name, device_number, port_number):
        """
        Add a port to the dataplane
        @param interface_name The name of the physical interface like eth1
        @param device_number The device id used to refer to the device
        @param port_number The port number used to refer to the port
        Stashes the port number on the created port object.
        """
        port_id = (device_number, port_number)
        self.ports[port_id] = self.dppclass(interface_name,
                                            device_number, port_number)
        self.ports[port_id]._port_number = port_number
        self.ports[port_id]._device_number = device_number
        self.packet_queues[port_id] = []
        # Need to wake up event loop to change the sockets being selected on.
        self.waker.notify()

    def send(self, device_number, port_number, packet):
        """
        Send a packet to the given port
        @param device_number, port_number The port to send the data to
        @param packet Raw packet data to send to port
        """
        self.logger.debug("Sending %d bytes to device %d, port %d" %
                          (len(packet), device_number, port_number))
        if self.pcap_writer:
            self.pcap_writer.write(packet, time.time(),
                                   device_number, port_number)
        bytes = self.ports[(device_number, port_number)].send(packet)
        self.tx_counters[(device_number, port_number)] += 1
        if bytes != len(packet):
            self.logger.error("Unhandled send error, length mismatch %d != %d" %
                     (bytes, len(packet)))
        return bytes

    def oldest_port_number(self, device):
        """
        Returns the port number with the oldest packet,
        or None if no packets are queued.
        """
        min_port_number = None
        min_time = float('inf')
        for (port_id, queue) in self.packet_queues.items():
            if port_id[0] != device:
                continue
            if queue and queue[0][1] < min_time:
                min_time = queue[0][1]
                min_port_number = port_id[1]
        return min_port_number

    # Dequeues and yields packets in the order they were received.
    # Yields (port, packet, received time).
    # If port is not specified yields packets from all ports.
    def packets(self, device, port=None):
        while True:
            if port is None:
                rcv_port = self.oldest_port_number(device)
            else:
                rcv_port = port

            if rcv_port == None:
                self.logger.debug("Out of packets on all ports")
                break

            queue = self.packet_queues[(device, rcv_port)]

            if len(queue) == 0:
                self.logger.debug("Out of packets on device %d, port %d",
                                  device, rcv_port)
                break

            pkt, time = queue.pop(0)
            yield (rcv_port, pkt, time)

    def poll(self, device_number=0, port_number=None, timeout=-1, exp_pkt=None, filters=[]):
        """
        Poll one or all dataplane ports for a packet

        If port_number is given, get the oldest packet from that port (and for
        that device).
        Otherwise, find the port with the oldest packet and return
        that packet.

        If exp_pkt is true, discard all packets until that one is found

        @param device_number Get packet from this device
        @param port_number If set, get packet from this port
        @param timeout If positive and no packet is available, block
        until a packet is received or for this many seconds
        @param exp_pkt If not None, look for this packet and ignore any
        others received.  Note that if port_number is None, all packets
        from all ports will be discarded until the exp_pkt is found
        @return The tuple device_number, port_number, packet, pkt_time where
        packet is received from device_number, port_number at time pkt_time.  If
        a timeout occurs, return None, None, None, None
        """

        def filter_check(pkt):
            for f in filters:
                if not f(pkt): return False
            return True

        if exp_pkt and (port_number is None):
            self.logger.warn("Dataplane poll with exp_pkt but no port number")

        # Retrieve the packet. Returns (device number, port number, packet, time).
        def grab():
            self.logger.debug("Grabbing packet")
            for (rcv_port_number, pkt, time) in self.packets(device_number, port_number):
                rcv_device_number = device_number
                self.logger.debug("Checking packet from device %d, port %d",
                                  rcv_device_number, rcv_port_number)
                if not filter_check(pkt):
                    self.logger.debug("Paket does not match filter, discarding")
                    continue
                if not exp_pkt or match_exp_pkt(exp_pkt, pkt):
                    return (rcv_device_number, rcv_port_number, pkt, time)
            self.logger.debug("Did not find packet")
            return None

        with self.cvar:
            ret = ptfutils.timed_wait(self.cvar, grab, timeout=timeout)

        if ret != None:
            return ret
        else:
            self.logger.debug("Poll time out, no packet from device %d, port %r",
                              device_number, port_number)
            return (None, None, None, None)

    def kill(self):
        """
        Stop the dataplane thread.
        """
        self.killed = True
        self.waker.notify()
        self.join()
        # Explicitly release ports to ensure we don't run out of sockets
        # even if someone keeps holding a reference to the dataplane.
        del self.ports

    def port_down(self, device_number, port_number):
        """Brings the specified port down"""
        self.ports[(device_number, port_number)].down()

    def port_up(self, device_number, port_number):
        """Brings the specified port up"""
        self.ports[(device_number, port_number)].up()

    def get_mac(self, device_number, port_number):
        """Get the specified mac"""
        return self.ports[(device_number, port_number)].mac()

    def get_counters(self, device_number, port_number):
        """Get the counters mac"""
        return self.rx_counters[(device_number, port_number)], \
               self.tx_counters[(device_number, port_number)]

    def get_nn_counters(self, device_number, port_number):
        """Get the specified port counters from nn agent """
        return self.ports[(device_number, port_number)].nn_counters()

    def flush(self):
        """
        Drop any queued packets.
        """
        for port_id in self.packet_queues.keys():
            self.packet_queues[port_id] = []

    def start_pcap(self, filename):
        assert(self.pcap_writer == None)
        self.pcap_writer = PcapWriter(filename)

    def stop_pcap(self):
        if self.pcap_writer:
            with self.cvar:
                self.pcap_writer.close()
                self.pcap_writer = None
                self.cvar.notify_all()
