import packet as scapy

class Mask:
    def __init__(self, exp_pkt):
        self.exp_pkt = exp_pkt
        self.size = len(str(exp_pkt))
        self.valid = True
        self.mask = [0xff] * self.size

    def set_do_not_care(self, offset, bitwidth):
        # a very naive but simple method
        # we do it bit by bit :)
        for idx in xrange(offset, offset + bitwidth):
            offsetB = idx / 8
            offsetb = idx % 8
            self.mask[offsetB] = self.mask[offsetB] & (~(1 << (7 - offsetb)))

    def set_do_not_care_scapy(self, hdr_type, field_name):
        if hdr_type not in self.exp_pkt:
            self.valid = False
            print "Unknown header type"
            return
        try:
            fields_desc = hdr_type.fields_desc
        except:
            self.valid = False
            return
        hdr_offset = self.size - len(self.exp_pkt[hdr_type])
        offset = 0
        bitwidth = 0
        for f in fields_desc:
            try:
                bits = f.size
            except:
                bits = 8 * f.sz
            if f.name == field_name:
                bitwidth = bits
                break
            else:
                offset += bits
        self.set_do_not_care(hdr_offset * 8 + offset, bitwidth)

    def is_valid(self):
        return self.valid

    def pkt_match(self, pkt):
        # just to be on the safe side
        pkt = str(pkt)
        if len(pkt) != self.size:
            return False
        exp_pkt = str(self.exp_pkt)
        for i in xrange(self.size):
            if (ord(exp_pkt[i]) & self.mask[i]) != (ord(pkt[i]) & self.mask[i]):
                return False
        return True

    def __str__(self):
        assert(self.valid)
        return ''.join([chr(b) for b in self.mask])

def utest():
    p = scapy.Ether() / scapy.IP() / scapy.TCP()
    m = Mask(p)
    assert(m.pkt_match(p))
    p1 = scapy.Ether() / scapy.IP() / scapy.TCP(sport=97)
    assert(not m.pkt_match(p1))
    m.set_do_not_care_scapy(scapy.TCP, "sport")
    assert(not m.pkt_match(p1))
    m.set_do_not_care_scapy(scapy.TCP, "chksum")
    assert(m.pkt_match(p1))

utest()
