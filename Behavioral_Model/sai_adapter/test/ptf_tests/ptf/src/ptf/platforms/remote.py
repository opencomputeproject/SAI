"""
Remote platform

This platform uses physical ethernet interfaces.
"""

# Update this dictionary to suit your environment.
remote_port_map = {
    (0, 0) : "eth0",
    (0, 1) : "eth1",
    (0, 2) : "eth2",
    (0, 3) : "eth3",
    (0, 4) : "eth4",
    (0, 5) : "eth5",
    (0, 6) : "eth6",
    (0, 7) : "eth7",
    (0, 8) : "eth8",
    (0, 9) : "eth9",
    (0, 10) : "eth10",
    (0, 11) : "eth11",
    (0, 12) : "eth12",
    (0, 13) : "eth13",
    (0, 14) : "eth14",
    (0, 15) : "eth15",
    (0, 16) : "eth16",
    (0, 17) : "eth17",
    (0, 18) : "eth18",
    (0, 19) : "eth19",
    (0, 20) : "eth20",
    (0, 21) : "eth21",
    (0, 22) : "eth22",
    (0, 23) : "eth23",
    (0, 24) : "eth24",
    (0, 25) : "eth25",
    (0, 26) : "eth26",
    (0, 27) : "eth27",
    (0, 28) : "eth28",
    (0, 29) : "eth29",
    (0, 30) : "eth30",
    (0, 31) : "eth31"
}

def platform_config_update(config):
    """
    Update configuration for the remote platform

    @param config The configuration dictionary to use/update
    """
    global remote_port_map
    config["port_map"] = remote_port_map.copy()
    config["caps_table_idx"] = 0
