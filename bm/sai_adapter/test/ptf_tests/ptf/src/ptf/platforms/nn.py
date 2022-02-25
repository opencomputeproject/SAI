"""
nn platform

This platform uses nanomsg sockets (both IPC and TCP are supported) to send and
receive packets. Unlike for other platforms, the '--interface' option is
ignored, you instead have to use '--device-socket'. This is because there has to
be a 1-1 mapping between the devices and the nanomsg sockets.

For example:
--device-socket 0-[1,2,5-8]@<socket addr>
In this case, ports 1, 2 and 5 through 8 (included) are enabled on device 0.

The socket address must be either:
ipc://<path to file>
tcp://<iface>:<port>
"""


def platform_config_update(config):
    """
    Update configuration for the nn platform

    @param config The configuration dictionary to use/update
    """

    port_map = {}

    for (device, ports, socket_addr) in config["device_sockets"]:
        for port in ports:
            port_map[(device, port)] = socket_addr

    # no default configuration for this platform

    config['port_map'] = port_map
