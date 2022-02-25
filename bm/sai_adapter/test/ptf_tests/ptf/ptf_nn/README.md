# PTF Nanomsg Agent

---

This agent is able to "bridge" a nanomsg socket with network interfaces. It can
be useful to enable PTF to send and receive packets on a remote host (i.e. not
the host running PTF).

---

## Dependencies

We rely on [nanomsg](http://nanomsg.org/) (a messaging library) to forward
packets between the PTF agent and the PTF test runner. You will therefore need
to install the following:

  - [nanomsg](https://github.com/nanomsg/nanomsg/releases): we recommend
    installing the `1.0.0` production release.
  - [nnpy](https://github.com/nanomsg/nnpy): these are the Python bindings for
    nanomsg. You may use the provided (install-nnpy.sh)[install-nnpy.sh] script
    to install nnpy. It will install a version of nnpy that we have tested.

We provide a [check-nnpy.py](check-nnpy.py) script that you can run to check
that nanomsg and nnpy are running properly.

## Overview

![PTF nanomsg overview](resources/ptf_nn.png)

In the above setup, we are able to capture and send packets on two different
machines (the PTF host and a remote host). Each agent acts as an intermediary
between a set of interfaces (connected to the switch) and the PTF
tester. Packets received on an interface (from the switch) will be tagged with
the port number and forwarded to the PTF tester. Packets received from the PTF
tester will be forwarded to the switch using the appropriate
interface. Communications between the PTF tester and each agent are done over
TCP using the nanomsg messaging library.

## Demo

Create the required 2 veth pairs (veth0-veth1 and veth2-veth3) with
`./veth_setup.sh`.

All of the following 4 steps need to be done in separate terminals. We assume
that PTF has been installed in a system location (i.e. the `ptf` binary is in
the `PATH`).

Start the first PTF agent with `sudo python ptf_nn_agent.py --device-socket
0@tcp://127.0.0.1:10001 -i 0-1@veth0`.

Start the second PTF agent with `sudo python ptf_nn_agent.py --device-socket
1@tcp://127.0.0.1:10002 -i 1-1@veth3`.

Start the dummy "test switch" with `sudo python ptf_nn_test_bridge.py -ifrom
veth1 -ito veth2`. This tool will "copy" any packet received on `veth1` to
`veth2`.

Run the PTF test with `sudo ptf --test-dir ptf_nn_test --device-socket
0-{0-64}@tcp://127.0.0.1:10001 --device-socket 1-{0-64}@tcp://127.0.0.1:10002
--platform nn`.

Now let's explain what's happening. We can consider that the first PTF agent
runs on the PTF host. The second PTF agent runs on the remote host. For each
host we need to use a separate device id (0 for the PTF host, 1 for the remote
host). The "switch" is connected to the PTF host through veth0-veth1 and to the
remote host through veth2-veth3. When running `ptf`, we need to use the `nn`
platform and provide the nanomsg TCP address for each of the 2 devices.

In our test, we send a packet to port 1 of device 0 and receive the exact same
packet on port 1 of device 1, as expected.

Of course, the remote host needs to be reachable by the PTF host, or the TCP
connection is not possible.
