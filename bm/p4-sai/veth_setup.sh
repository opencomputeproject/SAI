#!/bin/bash
sudo -v
for idx in 0 1 2 3 4; do
    intf0="sw_port$(($idx))"
    intf1="host_port0"
    net="host$(($idx))"
    ip="10.0.0.$(($idx))/24"
    if ! ip link show $intf0 &> /dev/null; then
        sudo ip link add name $intf0 type veth peer name $intf1
        sudo ip link set dev $intf0 up
    	sudo ip netns add $net
        sudo ip link set dev $intf1 netns $net
    	sudo ip netns exec $net ip link set dev $intf1 up
    	sudo ip netns exec $net ip address add $ip dev $intf1
    fi
done

# Last host will have 3 ports, to enable LACP and lag
net="host5"
sudo ip netns add $net
for port_idx in 0 1 2; do
	idx=port_idx+5
    intf0="sw_port$(($idx))"
    intf1="host_port$(($port_idx))"
    ip="10.0.0.$(($idx))/24"
    if ! ip link show $intf0 &> /dev/null; then
        sudo ip link add name $intf0 type veth peer name $intf1
        sudo ip link set dev $intf0 up
        sudo ip link set dev $intf1 netns $net
    	sudo ip netns exec $net ip link set dev $intf1 up
    	sudo ip netns exec $net ip address add $ip dev $intf1
    fi
done

intf0="cpu_port"
intf1="host_port"
net="hostif_net"
sudo ip link add name $intf0 type veth peer name $intf1
sudo ip link set dev $intf0 up
sudo ip netns add $net
sudo ip link set dev $intf1 netns $net
sudo ip netns exec $net ip link set dev $intf1 up

