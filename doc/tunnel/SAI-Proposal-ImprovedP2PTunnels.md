# Introduction
As of SAI version 1.5 the “tunnel” has a p2mp connotation. It holds the VTEP SIP whereas there is no DIP. The DIP is specified as part of the FDB entry or as part of Next Hop entry.

It is proposed to add the DIP as part of the `sai_tunnel_attr_t` structure as an optional parameter.

The following are the motivations for introducing a DIP in the tunnel structure and to model it as a p2p entity.

- Support Head End Replication as the method for handling BUM traffic.
- Support per remote IP and per remote IP+VNI Tx and Rx counters.
- Support the notion of operational status per remote IP.
- Support flushing FDB per remote IP.
- Support learning enable/disable per tunnel.

# Head End Replication

All the remote members as well as the local members are part of a broadcast domain.

In a generic scenario the following cases are applicable.
- Forwarding from local to remote members, remote to local members, local to local members should be allowed.
- Forwarding from remote to remote members should not be allowed.
- In a DCI case forwarding from the DCI tunnels to intra-DC tunnels and vice versa should be allowed.
- Forwarding from one remote to another remote need to be controlled by per member configuration like it being hub/spoke or a generic split horizon group.
- The remote end points can be of any encapsulation like VXLAN, MPLS, L2GRE etc and a single broadcast domain could have local members as well as remote members of different encapsulations.

To handle all these scenarios it is proposed to model the remote members on similar lines and as a point to point entity.

- For a dot1q bridge,
  - A tunnel is created for each remote passing the DIP as the newly introduced attribute.
  - A bridge_port is created with the following attributes.
    - Bridge port type as `SAI_BRIDGE_PORT_TYPE_TUNNEL`
    - `SAI_BRIDGE_PORT_ATTR_TUNNEL_ID` as the tunnel created above.
    - `SAI_BRIDGE_PORT_ATTR_BRIDGE_ID` as the default dot1q bridge.
    - `SAI_BRIDGE_PORT_ATTR_ISOLATION_GROUP` can be set appropriately depending on the application.
  - vlan_member objects are created with bridge port attribute as the above created entity.

- For a dot1d bridge, the steps are similar except that the bridge port is created for each dot1d bridge, using the same tunnel created. There are no vlan_members created as the bridgeports serve that purpose.

# Per Remote IP Tx and Rx Counters

By modeling the tunnel as a p2p entity the `sai_get_tunnel_stats_ext_fn` can be used to fetch the per remote IP stats.

# Per VNI Per Remote IP Tx and Rx counters

In a dot1d bridge case the bridgeport counters can also be used to fetch per remote VNI, per remote IP counters.

# Per Remote IP Operational Status

With a p2p modelling it is possible to associate the notion of an operational status. The operational status could be based on the underlay IP reachability to the remote IP, other device specific constraints or resource availability considerations.

# Per Remote IP FDB Flush

With a p2p modelling it is possible to flush the FDB entries associated with a remote IP. The `SAI_FDB_FLUSH_ATTR_BRIDGE_PORT_ID` can be re-used for tunnel bridgeports when modelled as p2p.

The flush might happen,
- As a result of operational status going down as above.
- Admin initiated.
- Due to a VXLAN BFD session going down.

# Per Remote IP Learning enable/disable

One more benefit is to enable learning enable/disable per tunnel bridge port. Learning will need to be enabled for static tunnels whereas disabled for EVPN tunnels.
