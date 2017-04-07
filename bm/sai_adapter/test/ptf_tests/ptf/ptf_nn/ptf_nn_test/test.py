import ptf
from ptf.base_tests import BaseTest
from ptf import config
import ptf.testutils as testutils

class DataplaneBaseTest(BaseTest):
    def __init__(self):
        BaseTest.__init__(self)

    def setUp(self):
        self.dataplane = ptf.dataplane_instance
        self.dataplane.flush()
        if config["log_dir"] != None:
            filename = os.path.join(config["log_dir"], str(self)) + ".pcap"
            self.dataplane.start_pcap(filename)

    def tearDown(self):
        if config["log_dir"] != None:
            self.dataplane.stop_pcap()

class OneTest(DataplaneBaseTest):
    def __init__(self):
        DataplaneBaseTest.__init__(self)

    def runTest(self):
        pkt = "ab" * 20
        testutils.send_packet(self, (0, 1), str(pkt))
        print "packet sent"
        testutils.verify_packet(self, pkt, (1, 1))

class GetMacTest(DataplaneBaseTest):
    def __init__(self):
        DataplaneBaseTest.__init__(self)

    def runTest(self):
        def check_mac(device, port):
            mac = self.dataplane.get_mac(device, port)
            self.assertIsNotNone(mac)
            self.assertEqual(mac.count(":"), 5)

        check_mac(0, 1)
        pkt = "ab" * 20
        testutils.send_packet(self, (0, 1), str(pkt))
        print "packet sent"
        testutils.verify_packet(self, pkt, (1, 1))
        check_mac(1, 1)


class GetCountersTest(DataplaneBaseTest):
    def __init__(self):
        DataplaneBaseTest.__init__(self)

    def runTest(self):
        def check_counters(device, port):
            counters = self.dataplane.get_nn_counters(device, port)
            self.assertIsNotNone(counters)
            self.assertTrue(type(counters) is tuple)
            self.assertEqual(len(counters), 2)

            return counters

        counters_01_b = check_counters(0, 1)
        counters_11_b = check_counters(1, 1)
        print "Counters:"
        print " (0, 1) %d:%d" % counters_01_b
        print " (1, 1) %d:%d" % counters_11_b
        pkt = "ab" * 20
        testutils.send_packet(self, (0, 1), str(pkt))
        print "packet sent"
        testutils.verify_packet(self, pkt, (1, 1))
        counters_01_e = check_counters(0, 1)
        counters_11_e = check_counters(1, 1)
        print "Counters:"
        print " (0, 1) %d:%d" % counters_01_e
        print " (1, 1) %d:%d" % counters_11_e
        self.assertTrue(counters_01_e[1] > counters_01_b[1])
        self.assertTrue(counters_11_e[0] > counters_11_b[0])
