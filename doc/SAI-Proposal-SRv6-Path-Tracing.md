Overview
Path tracing provides a record of the packet path as a sequence of interface ids. In addition, it provides a record of end-to-end delay, per-hop delay, and load on each egress interface along the packet delivery path.

This document covers SAI attributes to configure path tracing for midpoint node as per IETF Draft
https://datatracker.ietf.org/doc/draft-filsfils-spring-path-tracing/

When midpoint node that is path tracing enabled receives an IPv6 packet that contains an IPv6 HbH-PT option, records it's path tracing information into the HbH-PT header. This information is known as Midpoint Compressed Data (MCD).

MCD.OIF (Outgoing Interface ID): An 8-bit or 12-bit interface ID associated with the egress physical interface of the router SAI_PORT_ATTR_PATH_TRACING_INTF is proposed to represent the output interface id.

MCD.TTS (Truncated Timestamp): An 8-bit timestamp encoding the time at which the packet egress the router. Each egress interface in the device is configured with a TTS template. The TTS template defines the position of 8-bits to be selected from the egress timestamp. SAI_PORT_ATTR_PATH_TRACING_TIMESTAMP_TEMPLATE is proposed to represent the timestamp template. sai_port_pt_timestamp_template_type is proposed to define the possible template values.

