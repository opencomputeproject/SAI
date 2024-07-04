# Tunnel decapsulation model

The tunnel decapsulation stage is to be assumed at beginning of the pipeline after the Pre-ingress ACL and ingress interface type deduction, but before any L2/L3 flow.
In order for a packet to be admitted for tunnel decapsulation lookup, it has to have a propper L3 outer format (it should have a router DST MAC or a matching entry in mymac table).

The flow is defined in [tunnel decap](https://github.com/opencomputeproject/SAI/blob/master/doc/behavioral%20model/pipeline_v10.vsdx) page of the behavioral model.

## ACL behavior

Pre-ingress ACL takes effect before the tunnel decapsulation.
Ingress ACL takes effect after tunnel decapsulation.

Any changes to the outer fields in the Pre-ingress stage will be overriden in case tunnel termination happens. (E.g. Set SIP will be overriden by inner SIP).

Below are the examples of ACL behavior with different scenarios:

|Bind point type|Key|Stage|Match|Action|Tunnel termination type|Packet outer DIP|Packet inner DIP|Result|
|---------------|---|-----|-----|------|-----------------------|----------------|----------------|------|
|Port|Dip|Ingress|A|Drop|IPinIP|A|B|Miss|
|Port|Dip|Ingress|A|Drop|IPinIP|B|A|Hit, drop|
|Port|Dip|Ingress|A|Drop|VxLAN|B|A|Hit, drop|
|Port|Dip|Pre-ingress|A|Drop|IPinIP|A|B|Hit, drop|
|Port|Dip|Pre-ingress|A|Drop|IPinIP|B|A|Miss|
|Port|Dip|Ingress|A|Counter|IPinIP|A|A|Counter +1|
|Port|Dip|Egress|A|Mirror|N/A|A|N/A|Miss|
|Port|Dip|Ingress|A|Drop|N/A|A|N/A|Hit, drop|
|Port|Dip|Pre-ingress|A|Drop|N/A|A|N/A|Hit, drop|
|Port|Dip|Ingress|A|Drop|N/A|B|N/A|A|Miss|
|Port|Dip|Pre-ingress|A|Drop|N/A|B|A|Miss|
