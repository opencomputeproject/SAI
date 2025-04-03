### IP Multicast Group with members

Allow IP Multicast Group to be created/modified by specifying the list of members

### Motivation

The existing IPMC workflow
* Create a IPMC group
* Add/remove members to this group

We describe a sample workflow
* A is added to the group
* B is added to the group
* The path to A goes down, so an alternate member A' is added
* Subsequently the primary path to A is restored, so A' needs to be swapped with A

We have two ways to acheive this.
* remove A' and then add A leading to a small window where no traffic is received by the receiver.
* add A and then remove A' leading to a small window wheret duplicate traffic is received.

We'd like to avoid both of these scenarios.

### Proposal

Specify the full current list of multicast group members at create and update time.

* Introduce an attribute specifying that the IPMC group members are specified upfront.
* Add a IPMC group attribute for the list of members

