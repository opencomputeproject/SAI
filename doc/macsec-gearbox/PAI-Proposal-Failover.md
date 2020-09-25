![](RackMultipart20200925-4-s340pz_html_6e4eda26a3e5fd14.png)

# External PHY Failover

# Proposal

| **Title** | **External PHY Abstraction Interface** |
| --- | --- |
| **Authors** | **Broadcom** |
| **Status** | **In review** |
| **Type** | **Standards track** |
| **Created** | **8/31/2020** |
| **SAI-Version** | **0.1** |


# Overview

The purpose of this document is to describe PHY failover (FO) functionality and interface to manage failover functionality of the PHY. PHY is the connector between MAC (SerDes) and physical medium such as optical fiber or copper transceivers. PHY failover feature uses two physical port configurations: primary and secondary. This can be enabled on system/host or line/client side of a port. It provides the ability to switch from primary port to secondary port or vice versa without losing data/link.

![](RackMultipart20200925-4-s340pz_html_6fd49d19699540a6.png)

In the above multi-port configuration, failover mode is configured on the system side of external PHY. Primary and secondary ports are physical connections between MAC and PHY. The FO configuration uses primary and secondary port connections on selected side (system side or line side).

The PHY failover mode enables a port to broadcast data on both primary and secondary ports. The MAC would be configured to receive the data on either primary or secondary ports. PHY failover active ingress port can be configured as either primary or secondary port. When the link between MAC and PHY goes down on primary port, user can switch to secondary port.

## Failover Configuration on PHY

Failover feature can be configured on all ports of the PHY. Failover ports can operate in either hitless (the ability to switch over port without losing link) or non-hitless mode.

Additions to saiport.h

/\*\*

\* @brief Attribute data for #SAI\_PORT\_CONNECTOR\_ATTR\_FAILOVER\_MODE

\* Used for Failover mode configuration on port

\*/

typedef enum \_sai\_port\_connector\_failover\_mode\_e

{

/\*\* Failover mode disable \*/

SAI\_PORT\_CONNECTOR\_FAILOVER\_MODE\_DISABLE,

/\*\* Configure Failover mode on primary port \*/

SAI\_PORT\_CONNECTOR\_FAILOVER\_MODE\_PRIMARY,

/\*\* Configure Failover mode on secondary port \*/

SAI\_PORT\_CONNECTOR\_FAILOVER\_MODE\_SECONDARY

} sai\_port\_connector\_failover\_mode\_t;

/\*\*

\* @brief Configure the failover mode on port

\*

\* @type sai\_port\_connector\_failover\_mode\_t

\* @flags CREATE\_AND\_SET

\* @default SAI\_PORT\_CONNECTOR\_FAILOVER\_MODE\_DISABLE

\*/

SAI\_PORT\_CONNECTOR\_ATTR\_FAILOVER\_MODE,

/\*\*

\* @brief System Side Port ID

\*

\* @type sai\_object\_id\_t

\* @flags CREATE\_ONLY

\* @objects SAI\_OBJECT\_TYPE\_PORT

\*/

SAI\_PORT\_CONNECTOR\_ATTR\_SYSTEM\_SIDE\_FAILOVER\_PORT\_ID,

/\*\*

\* @brief Line Side Port ID

\*

\* @type sai\_object\_id\_t

\* @flags CREATE\_ONLY

\* @objects SAI\_OBJECT\_TYPE\_PORT

\*/

SAI\_PORT\_CONNECTOR\_ATTR\_LINE\_SIDE\_FAILOVER\_PORT\_ID,


## Additions to saiswitch.h

/\*\*

\* @brief Attribute data for #SAI\_SWITCH\_ATTR\_FAILOVER\_CONFIG\_MODE

\* Used for Failover configuration mode

\* In case of primary port failure, hitless enables the switch over to secondary

\* port without losing link. It allows uninterrupted data transmission

\*/

typedef enum \_sai\_switch\_failover\_config\_mode\_t

{

/\*\* Ports are configured but do not operate in hitless \*/

SAI\_SWITCH\_FAILOVER\_CONFIG\_MODE\_NO\_HITLESS,

/\*\* Ports are configured and operate in hitless \*/

SAI\_SWITCH\_FAILOVER\_CONFIG\_MODE\_HITLESS

} sai\_switch\_failover\_config\_mode\_t;

/\*\*

\* @brief Failover configuration mode

\*

\* @type sai\_switch\_failover\_config\_mode\_t

\* @flags CREATE\_AND\_SET

\* @default SAI\_SWITCH\_FAILOVER\_CONFIG\_MODE\_NO\_HITLESS

\*/

SAI\_SWITCH\_ATTR\_FAILOVER\_CONFIG\_MODE,

/\*\*

\* @brief Query for Failover mode support

\*

\* @type bool

\* @flags READ\_ONLY

\*/

SAI\_SWITCH\_ATTR\_SUPPORTED\_FAILOVER\_MODE,

# Configuration Example

Following example shows how to setup failover configuration on a system side port. In this example a 16 lane PHY is used.

/\* Create System side, Line side, and System Side Failover ports \*/

sys\_attr[0].value.u32list.list = sys\_lane\_list;

line\_attr[0].value.u32list.list = line\_lane\_list;

failover\_attr[0].value.u32list.list = failover\_lane\_list;

line\_attr[1].id = sys\_attr[1].id = SAI\_PORT\_ATTR\_SPEED;

line\_attr[1].value.u32= sys\_attr[1].value.u32 = 100000;

line\_attr[2].id = sys\_attr[2].id = SAI\_PORT\_ATTR\_INTERFACE\_TYPE;

line\_attr[2].value.u32 = sys\_attr[2].value.u32 = SAI\_PORT\_INTERFACE\_TYPE\_KR;

line\_attr[3].id = sys\_attr[3].id = SAI\_PORT\_ATTR\_FEC\_MODE;

line\_attr[3].value.u32 = sys\_attr[3].value.u32 = SAI\_PORT\_FEC\_MODE\_RS;

line\_attr[4].id = sys\_attr[4].id = SAI\_PORT\_ATTR\_LINK\_TRAINING\_ENABLE;

line\_attr[4].value.booldata = sys\_attr[4].value.booldata = 1;

line\_attr[5].id = sys\_attr[5].id = SAI\_PORT\_ATTR\_ADMIN\_STATE;

line\_attr[5].value.booldata = sys\_attr[5].value.booldata = 1;

for (phy\_index = 0; phy\_index \&lt; PAI\_MAX\_PHY; phy\_index ++) {

for (index = 0; index \&lt; 2; index ++) {

line\_lane\_list[index] = (phy\_index\*16)+ port\_index + index;

sys\_lane\_list[index] = (phy\_index\*16)+16 + port\_index + index;

failover\_lane\_list[index] = (phy\_index\*16)+20 + port\_index + index;

}

rv = pai\_port\_apis\_ptr-\&gt;create\_port(&amp;sys\_port\_id[phy\_index][port\_index],

switch\_id[phy\_index], COUNTOF(sys\_attr), sys\_attr);

if (SAI\_STATUS\_SUCCESS != rv) {

printf(&quot;System side port creation failed with error:%d\n&quot;, rv);

return rv;

}

rv = pai\_port\_apis\_ptr-\&gt;create\_port(&amp;fail\_over\_port\_id[phy\_index][port\_index],

switch\_id[phy\_index], COUNTOF(fail\_over\_attr), fail\_over\_attr);

if (SAI\_STATUS\_SUCCESS != rv) {

printf(&quot;System side failover port creation failed with error:%d\n&quot;, rv);

return rv;

}

rv = pai\_port\_apis\_ptr-\&gt;create\_port(&amp;line\_port\_id[phy\_index][port\_index],

switch\_id[phy\_index], COUNTOF(line\_attr), line\_attr);

if (SAI\_STATUS\_SUCCESS != rv) {

printf(&quot;Line side port creation failed with error:%d\n&quot;, rv);

return rv;

}

/\* Populate failover port IDs \*/

port\_conn[0].id = SAI\_PORT\_CONNECTOR\_ATTR\_SYSTEM\_SIDE\_PORT\_ID;

port\_conn[0].value.oid = sys\_port\_id[phy\_index][port\_index];

port\_conn[1].id = SAI\_PORT\_CONNECTOR\_ATTR\_LINE\_SIDE\_PORT\_ID;

port\_conn[1].value.oid = line\_port\_id[phy\_index][port\_index];

port\_conn[2].id = SAI\_PORT\_CONNECTOR\_ATTR\_SYSTEM\_SIDE\_FAILOVER\_PORT\_ID;

port\_conn[2].value.oid = fail\_over\_port\_id[phy\_index][port\_index];

port\_conn[3].id = SAI\_PORT\_CONNECTOR\_ATTR\_FAILOVER\_MODE;

port\_conn[3].value.u32 = SAI\_PORT\_CONNECTOR\_FAILOVER\_MODE\_PRIMARY;

rv = pai\_port\_apis\_ptr-\&gt;create\_port\_connector(&amp;port\_conn\_id[phy\_index][port\_index],

switch\_id[phy\_index], COUNTOF(port\_conn), port\_conn);

if (SAI\_STATUS\_SUCCESS != rv) {

printf(&quot;Port connector creation failed with error: %d\n&quot;, rv);

return rv;

}

printf(&quot;Port connector: 0x%lx.\n&quot;, port\_conn\_id[phy\_index][port\_index]);

}

/\* Get current state of failover mode \*/

for (port\_index = 0; port\_index \&lt; 1; port\_index ++) {

for (phy\_index = 0; phy\_index \&lt; PAI\_MAX\_PHY; phy\_index ++) {

attr\_count = 1;

memset(&amp;port\_conn\_attr\_get, 0, sizeof(port\_conn\_attr\_get));

port\_conn\_attr\_get[0].id = SAI\_PORT\_CONNECTOR\_ATTR\_FAILOVER\_MODE;

rv = pai\_port\_apis\_ptr-\&gt;get\_port\_connector\_attribute(

sys\_port\_id[phy\_index][port\_index], attr\_count, port\_conn\_attr\_get);

if (SAI\_STATUS\_SUCCESS != rv) {

printf(&quot;Get port connector attribute failed with error: %d\n&quot;, rv);

return rv;

}

printf(&quot;PAI Port Connector failover mode get attribute values :%d\n&quot;,

port\_conn\_attr\_get[0].value.u32);

}

}

/\* Configure primary failover port \*/

for (port\_index = 0; port\_index \&lt; 1; port\_index ++) {

for (phy\_index = 0; phy\_index \&lt; PAI\_MAX\_PHY; phy\_index ++) {

port\_conn\_attr\_set.id = SAI\_PORT\_CONNECTOR\_ATTR\_FAILOVER\_MODE;

port\_conn\_attr\_set.value.u32 = SAI\_PORT\_CONNECTOR\_FAILOVER\_MODE\_PRIMARY;

rv = pai\_port\_apis\_ptr-\&gt;set\_port\_connector\_attribute(

sys\_port\_id[phy\_index][port\_index], &amp;port\_conn\_attr\_set);

if (SAI\_STATUS\_SUCCESS != rv) {

printf(&quot;Set Port Connector Attribute failed with error: %d\n&quot;, rv);

return rv;

}

}

}

/\* Failover switch attribute configuration applicable for all ports on PHY with no hitless) \*/

if (pai\_switch\_apis\_ptr-\&gt;set\_switch\_attribute) {

sai\_attribute\_t sai\_set\_attr;

sai\_set\_attr.id = SAI\_SWITCH\_ATTR\_FAILOVER\_CONFIG\_MODE;

sai\_set\_attr.value.u32 = SAI\_SWITCH\_FAILOVER\_CONFIG\_MODE\_NO\_HITLESS;

rv = pai\_switch\_apis\_ptr-\&gt;set\_switch\_attribute(switch\_id[0], &amp;sai\_set\_attr);

if (SAI\_STATUS\_SUCCESS != rv) {

printf(&quot;Set switch attribute failed: Error: %d\n&quot;, rv);

return rv;

}

}

if (pai\_switch\_apis\_ptr-\&gt;get\_switch\_attribute) {

for (switch\_index = 0;switch\_index \&lt; PAI\_MAX\_PHY; switch\_index ++) {

sai\_get\_attr.id = SAI\_SWITCH\_ATTR\_FAILOVER\_CONFIG\_MODE;

rv = pai\_switch\_apis\_ptr-\&gt;get\_switch\_attribute(switch\_id[switch\_index],

attr\_count, &amp;sai\_get\_attr);

if (SAI\_STATUS\_SUCCESS != rv) {

printf(&quot;Get switch attribute failed: Error: %d\n&quot;, rv);

return rv;

}

printf(&quot;failover config mode: %d\n&quot;, sai\_get\_attr.value.u32);

}

}
