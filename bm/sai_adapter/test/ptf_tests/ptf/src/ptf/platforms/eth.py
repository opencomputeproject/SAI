"""
Eth platform

This platform uses the --interface command line option to choose the ethernet interfaces.
"""

def platform_config_update(config):
    """
    Update configuration for the local platform

    @param config The configuration dictionary to use/update
    """

    port_map = {}

    for (device, port, interface) in config["interfaces"]:
        port_map[(device, port)] = interface

    # Default to a veth configuration compatible with the reference switch
    if not port_map:
        port_map = {
            (0, 0): 'veth1',
            (0, 1): 'veth3',
            (0, 2): 'veth5',
            (0, 3): 'veth7',
        }

    config['port_map'] = port_map
