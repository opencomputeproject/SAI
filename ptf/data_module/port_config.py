from data_module.data_obj import auto_str

@auto_str
class PortConfig(object):
    """
    Represent the PortConfig Object
    Attrs:
        name: interface name
        lanes: lanes
        speed: port speed
        fec: fec mode
        alias: alias
        index: index
        mtu: mtu
        pfc_asym: pfc_asym
        tpid: tpid
    """

    def __init__(self,
    name=None,
    lanes=None,
    speed=None,
    fec=None,
    alias=None,
    index=None,
    mtu=None,
    pfc_asym=None,
    tpid=None):
        self.name = name
        self.lanes = lanes
        self.alias = alias
        self.index = index
        self.mtu = mtu
        self.pfc_asym = pfc_asym
        self.speed = speed
        self.fec = fec
        self.tpid = tpid
