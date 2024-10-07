# SAI Counter enhancement
-------------------------------------------------------------------------------
 Title       | SAI counter enahncement
-------------|-----------------------------------------------------------------
 Authors     |<code> Rajkumar P R, Marvell Technology Inc <br> Ravindranath C K, Marvell Technology Inc </code>
 Status      | In review
 Type        | Standards track
 Created     | 01/07/2024

-------------------------------------------------------------------------------

## Problem Statement
Within the current framework of SAI, objects such as vlan, router-interface, tunnel, queue, port, and others come with built-in statistical enumeration. The expectation is that these statistics will be counted by default upon the creation of objects. This proposal contributes to achieving efficient utilization of flexible counters based on the counting requirements of deployments. It enables platforms operating within scaled environments to seamlessly map these flexible counters, thereby facilitating the realization of this objective.

## Overview
When some SAI objects are created, the default statistics enum defined in each these object file are expected to be counted. Current model has no option to disable/enable statistics counting. As part of this proposal, we would like to add the flexibility to manage the counting at run-time. With this,
- User has flexibility to decide which object should support counting.
- Helps to optimally use the hardware counting resources.

## Options
1. A new SAI attribute introduced in each of these object to determine counting mode.
2. In parallel to existing design, introduce a new counter ID attribute per statistics type to count packet, byte.
3. Using saicounter and counting mode per object.

## SAI Spec Enhancement
### Option 1

**New enum defined for the counting mode.**
```c
typedef enum _sai_stats_count_mode_t
{
 
    /** Count packet and byte */
    SAI_STATS_COUNT_MODE_PACKET_AND_BYTE,

    /** Count only packet */
    SAI_STATS_COUNT_MODE_PACKET,

    /** Count only byte */
    SAI_STATS_COUNT_MODE_BYTE,    

     /** Counting is disabled */
    SAI_STATS_COUNT_MODE_NONE

} sai_stats_count_mode_t;
```
**New attribute introduced to update the counting mode. Default is set to SAI_STATS_COUNT_MODE_PACKET_AND_BYTE**

saiport.h
```c
typedef enum _sai_port_attr_t
{
    /**
     * @brief set port statistics
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_PORT_ATTR_STAT_MODE

} sai_port_attr_t;
```
sairouterinterface.h
```c
typedef enum _sai_router_interface_attr_t
{
    /**
     * @brief set router interface statistics
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_ROUTER_INTERFACE_ATTR_STAT_MODE

} sai_router_interface_attr_t;
```
saivlan.h
```c
typedef enum _sai_vlan_attr_t
{
 /**
     * @brief set vlan statistics
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_VLAN_ATTR_STAT_MODE

} sai_vlan_attr_t;
```
saitunnel.h
```c
typedef enum _sai_tunnel_attr_t
{
    /**
     * @brief set tunnel statistics
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_TUNNEL_ATTR_STAT_MODE

} sai_tunnel_attr_t;
```
saiqueue.h
```c
typedef enum _sai_queue_attr_t
{
 /**
     * @brief set queue interface statistics
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_QUEUE_ATTR_STAT_MODE

} sai_queue_attr_t;
```
#### Sample Workflow 
NOS can query the attribute capability to decide the counting enable/disable during object creation time. Implementation not supporting this attribute will have existing behavior.

##### CREATE object
```c
sai_status_t status = SAI_STATUS_SUCCESS;
sai_attr_capability_t rif_capability;
sai_attribute_t  attr;
vector<sai_attribute_t> attrs;
sai_stats_count_mode_t rifCntMode = SAI_STATS_COUNT_MODE_PACKET_AND_BYTE;

status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE, SAI_ROUTER_INTERFACE_ATTR_STAT_MODE, &rif_capability);
if (status == SAI_STATUS_SUCCESS && rif_capability.create_implemented)
{
    attr.id = SAI_ROUTER_INTERFACE_ATTR_STAT_MODE;
    /* Read and set the platform default behavior */
    if (user_has_set_packet_or_byte_mode)
    {
      rifCntMode = user_config_mode;
    }
    attr.value.s32 = rifCntMode;
    attrs.push_back(attr);
}
else
{
    /** Existing Behavior. All enums will be counted.*/
}
sai_status_t status = sai_router_intfs_api->create_router_interface(&port.m_rif_id, gSwitchId, (uint32_t)attrs.size(), attrs.data());

```
##### SET Object
```c
sai_status_t status = SAI_STATUS_SUCCESS;
sai_attr_capability_t rif_capability;
sai_attribute_t  attr;
sai_stats_count_mode_t rifCntMode = SAI_STATS_COUNT_MODE_PACKET_AND_BYTE;

status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE, SAI_ROUTER_INTERFACE_ATTR_STAT_MODE, &rif_capability);

if (status == SAI_STATUS_SUCCESS && rif_capability.set_implemented)
{
    attr.id = SAI_ROUTER_INTERFACE_ATTR_STAT_MODE;
    /* Read and set the user configured value */
    if (user_has_set_packet_or_byte_mode)
    {
      rifCntMode = user_config_mode;
    }
    attr.value.s32 = rifCntMode;
}
else
{
     /** Existing Behavior */
}
sai_status_t status =
        sai_router_intfs_api->set_router_interface_attribute(port.m_rif_id, &attr);

```
##### GET statistics
```c
std::vector<uint32_t> supported_counter_ids;
sai_stat_capability_list_t stats_capability;
std::vector<uint64_t> stats;
bool enable = true;
std::vector<sai_stat_capability_t> stats;
status = sai_query_stats_capability(
       gSwitchId,
       SAI_OBJECT_TYPE_ROUTER_INTERFACE,
       stats_capability.data());

stats_capability.count = SAI_ROUTER_INTERFACE_STAT_OUT_ERROR_PACKETS+1;
stats_capability.list = stats.data();

for (uint32_t cnt = 0; cnt < stats_capability.count; cnt++)       
{
    supported_counter_ids.push_back(stats_capability.list[cnt]);
}

status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE, 
                                        SAI_ROUTER_INTERFACE_ATTR_STAT_MODE,
                                        &rif_capability);
if (status == SAI_STATUS_SUCCESS && 
   (rif_capability.create_implemented || rif_capability.set_implemented))
{    
    sai_attribute_t attr;
    attr.id = SAI_ROUTER_INTERFACE_ATTR_STAT_MODE;
    status = sai_router_intfs_api->get_router_interface_attr(port.m_rif_id,1, &attr); 

    if(status == SAI_STATUS_SUCCESS && attr.value.s32 == SAI_STATS_COUNT_MODE_NONE)
    {
        enable = false;
    }
    else
    {
        /* Return code SAI_STATUS_NOT_IMPLEMENTED if attr not supported. */
    }
}

if(enable)
{
    status = sai_router_intfs_api->get_router_interface_stats(
                    rid,
                    (uint32_t(supported_counter_ids.size()),
                    (sai_stat_id_t *)supported_counter_ids.data(),
                    stats.data());
}
```

#### Pros 
- Minimal change to existing work flow.
#### Cons
- No Flexiblity to count selective statistics.

### Option 2
Introduce counter ID attribute that is holding the counter oid of specific statistic enum. 
Lets take an example of sairouterinterface. Below statistics enums are defined in sairouterinterface,
```c
typedef enum _sai_router_interface_stat_t
{
    /** Ingress byte stat count */
    SAI_ROUTER_INTERFACE_STAT_IN_OCTETS,

    /** Ingress packet stat count */
    SAI_ROUTER_INTERFACE_STAT_IN_PACKETS,

    /** Egress byte stat count */
    SAI_ROUTER_INTERFACE_STAT_OUT_OCTETS,

    /** Egress packet stat count */
    SAI_ROUTER_INTERFACE_STAT_OUT_PACKETS,

    /** Byte stat count for packets having errors on router ingress */
    SAI_ROUTER_INTERFACE_STAT_IN_ERROR_OCTETS,

    /** Packet stat count for packets having errors on router ingress */
    SAI_ROUTER_INTERFACE_STAT_IN_ERROR_PACKETS,

    /** Byte stat count for packets having errors on router egress */
    SAI_ROUTER_INTERFACE_STAT_OUT_ERROR_OCTETS,

    /** Packet stat count for packets having errors on router egress */
    SAI_ROUTER_INTERFACE_STAT_OUT_ERROR_PACKETS

} sai_router_interface_stat_t;

```
Above enums will be mapped to new counter ID attributes.
```c
 /**
     * @brief Attach a counter
     *
     * Ingress byte, packet stat count
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    
    SAI_ROUTER_INTERFACE_ATTR_IN_COUNTER,
    /**
     * @brief Attach a counter
     *
     * Egress byte, packet stat count
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    
    SAI_ROUTER_INTERFACE_ATTR_OUT_COUNTER,
    /**
     * @brief Attach a counter
     *
     * Ingress byte, packet stat count for packets having errors on router ingress
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    
    SAI_ROUTER_INTERFACE_ATTR_IN_ERROR_COUNTER,
    /**
     * @brief Attach a counter
     *
     * Egress byte, packet stat count for packets having errors on router ingress
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    
    SAI_ROUTER_INTERFACE_ATTR_OUT_ERROR_COUNTER,

```
#### Sample workflow
##### CREATE Object
```c
sai_status_t status = SAI_STATUS_SUCCESS;
sai_attr_capability_t rif_capability;
sai_attribute_t  attr;
vector<sai_attribute_t> attrs;
sai_int32_t countArr[4] = {SAI_ROUTER_INTERFACE_ATTR_IN_COUNTER,
                               SAI_ROUTER_INTERFACE_ATTR_OUT_COUNTER,
                               SAI_ROUTER_INTERFACE_ATTR_IN_ERROR_COUNTER,
                               SAI_ROUTER_INTERFACE_ATTR_OUT_ERROR_COUNTER };

sai_object_id_t countOid[4] = {SAI_NULL_OBJECT_ID}; /* per routerinterface object */

for(int32_t idx = 0; idx < 4; idx ++)
{
    status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE, countArr[idx], 
    &rif_capability);
    if (status == SAI_STATUS_SUCCESS && rif_capability.create_implemented)
    {
        /* Create counter object */
        cnt_attr.id = SAI_COUNTER_ATTR_TYPE;
        cnt_attr.value.s32 = SAI_COUNTER_TYPE_REGULAR;
        cnt_attrs.push_back(cnt_attr);
        status = sai_counter_api->sai_create_counter(&countOid[idx], gSwitchId, 1, cnt_attrs.data());

        attr.id = countArr[idx];
        attr.value.oid = countOid[idx];
        attrs.push_back(attr);
    }
}

sai_status_t status = sai_router_intfs_api->create_router_interface(&port.m_rif_id, gSwitchId, (uint32_t)attrs.size(), attrs.data());
```
##### SET Object
```c
/* Example flow to set SAI_ROUTER_INTERFACE_ATTR_IN_COUNTER*/

sai_status_t status = SAI_STATUS_SUCCESS;
sai_attr_capability_t rif_capability;
sai_attribute_t  attr;
sai_object_id_t inCountOid = SAI_NULL_OBJECT_ID;

status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE, countArr[0], &rif_capability);

if (status == SAI_STATUS_SUCCESS && rif_capability.set_implemented)
{
    if(enable) /* Is specific enum enabled from north-bound */
    {
        if(countOid[0] == SAI_NULL_OBJECT_ID)
        {
            /* Create counter object for IN_COUNTER */
            cnt_attr.id = SAI_COUNTER_ATTR_TYPE;
            cnt_attr.value.s32 = SAI_COUNTER_TYPE_REGULAR;
            cnt_attrs.push_back(cnt_attr);
            status = sai_counter_api->sai_create_counter(&countOid[0], gSwitchId, 1, cnt_attrs.data());
        }
    }
    else
    {
        if(countOid[0])
        {
            status = sai_counter_api->sai_remove_counter(&countOid[0]);
        }
        countOid[0] = SAI_NULL_OBJECT_ID;
    }
    
    attr.id = countArr[0]; //SAI_ROUTER_INTERFACE_ATTR_IN_COUNTER
    attr.value.oid = countOid[0];
}

sai_status_t status =
        sai_router_intfs_api->set_router_interface_attribute(port.m_rif_id, &attr);

```
##### Get statistics
```c
/* Assume, countOid array is updated for the specific router-interface as part of create/set operation.*/

std::vector<uint32_t> supported_counter_ids;
std::vector<uint64_t> stats;

for(int32_t idx = 0; idx < 4; idx ++)
{
    if(countOid[idx] != SAI_NULL_OBJECT_ID)
    {
        supported_counter_ids.push_back(countOid[idx]);
    }
}

if(supported_counter_ids.empty())
{
    /* Existing Behavior */

    sai_stat_capability_list_t stats_capability;
    std::vector<uint64_t> stats;
    std::vector<sai_stat_capability_t> stats;
    status = sai_query_stats_capability(
        gSwitchId,
        SAI_OBJECT_TYPE_ROUTER_INTERFACE,
        stats_capability.data());

    stats_capability.count = SAI_ROUTER_INTERFACE_STAT_OUT_ERROR_PACKETS+1;
    stats_capability.list = stats.data();

    for (uint32_t cnt = 0; cnt < stats_capability.count; cnt++)       
    {
        supported_counter_ids.push_back(stats_capability.list[cnt]);
    }
    
    status = sai_router_intfs_api->get_router_interface_stats(
                    rid,
                    (uint32_t)supported_counter_ids.size(),
                    (sai_stat_id_t *)supported_counter_ids.data(),
                    stats.data());
}
else
{
    sai_stat_id_t stat_ids[] = { SAI_COUNTER_STAT_PACKETS, SAI_COUNTER_STAT_BYTES };
    uint64_t stats[2];
    for(int32_t idx = 0;i < 4; i++)
    {
        if(countOid[idx] != SAI_NULL_OBJECT_ID)
        {
            status = sai_counter_api->get_counter_stats(
                            countOid[idx],
                            2,/*count packet and byte */
                            stat_ids,
                            stats);
        }
    }
}
```
#### Pros

- Flexiblity to count selective statistics.

#### Cons

- New workflow with more attributes per object.

### Option 3
This is an optimization to option 1 and 2 to reduce the number of attributes.
- Introduce a new counter type (selective) and counter attributes to saicounter. This will allow user to have fine grain control on which statistics to be counted.
- Introduce new attribute to update the counting mode at object level. Default counting mode is set to SAI_STATS_COUNT_MODE_PACKET_AND_BYTE. With this, user can count supported packet/byte/both stats per object.


**saicounter.h**
```c
typedef enum _sai_counter_type_t
     /** Regular */
     SAI_COUNTER_TYPE_REGULAR,

+    /** Selective Counter */
+    SAI_COUNTER_TYPE_SELECTIVE,
+
 } sai_counter_type_t;

```

```c
typedef enum _sai_counter_attr_t
{
    :
    :

    /**
     * @brief Enable/disable packet count
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default true
     */
    SAI_COUNTER_ATTR_ENABLE_PACKET_COUNT,

    /**
     * @brief Enable/disable byte count
     *
     * @type bool
     * @flags CREATE_ONLY
     * @default true
     */
    SAI_COUNTER_ATTR_ENABLE_BYTE_COUNT,

    /**
     * @brief Object Type of the stat-id
     *
     * @type sai_object_type_t
     * @flags MANDATORY_ON_CREATE | CREATE_ONLY
     * @condition SAI_COUNTER_ATTR_TYPE == SAI_COUNTER_TYPE_SELECTIVE
     */
    SAI_COUNTER_ATTR_OBJECT_TYPE,

    /**
     * @brief Stat id list
     *
     * List of statistics enum mapped to this counter
     *
     * @type sai_s32_list_t
     * @flags CREATE_AND_SET
     * @default empty
     * @validonly SAI_COUNTER_ATTR_TYPE == SAI_COUNTER_TYPE_SELECTIVE
     */
    SAI_COUNTER_ATTR_STAT_ID_LIST,

    /**
     * @brief End of attributes
     */
    SAI_COUNTER_ATTR_END,

    /** Custom range base value */
    SAI_COUNTER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_COUNTER_ATTR_CUSTOM_RANGE_END

} sai_counter_attr_t;

```
**New enum defined for the counting mode.**

**saitypes.h**
```c
typedef enum _sai_stats_count_mode_t
{
    /** Count packet and byte */
    SAI_STATS_COUNT_MODE_PACKET_AND_BYTE,

    /** Count only packet */
    SAI_STATS_COUNT_MODE_PACKET,

    /** Count only byte */
    SAI_STATS_COUNT_MODE_BYTE,

    /** Counting is disabled */
    SAI_STATS_COUNT_MODE_NONE

} sai_stats_count_mode_t;

```
**sairouterinterface.h**
 ```c
    /**
     * @brief Set router interface statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_ROUTER_INTERFACE_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * counter object should be of type Selective,
     * fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE)
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_ROUTER_INTERFACE_ATTR_SELECTIVE_COUNTER_LIST,

```
**saivlan.h**
 ```c
    /**
     * @brief Set vlan statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_VLAN_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * counter object should be of type Selective,
     * fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE)
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_VLAN_ATTR_SELECTIVE_COUNTER_LIST,
```
**saitunnel.h**
 ```c
     /**
     * @brief Set tunnel statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_TUNNEL_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * counter object should be of type Selective,
     * fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE)
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_TUNNEL_ATTR_SELECTIVE_COUNTER_LIST,
```
**saiqueue.h**
 ```c
     /**
     * @brief Set queue statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_QUEUE_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * counter object should be of type Selective,
     * fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE)
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_QUEUE_ATTR_SELECTIVE_COUNTER_LIST,
```
**saiport.h**
 ```c
    /**
     * @brief Set port statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_PORT_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * counter object should be of type Selective,
     * fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE)
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_PORT_ATTR_SELECTIVE_COUNTER_LIST,
```
**saibfd.h**
```c
    /**
     * @brief Set BFD session statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_BFD_SESSION_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_BFD_SESSION_ATTR_SELECTIVE_COUNTER_LIST,
```
**saibridge.h**
```c
    /**
     * @brief Set bridge port statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_BRIDGE_PORT_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_BRIDGE_PORT_ATTR_SELECTIVE_COUNTER_LIST,


    /**
     * @brief Set bridge statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_BRIDGE_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_BRIDGE_ATTR_SELECTIVE_COUNTER_LIST,
```
**saibuffer.h**
```c
    /**
     * @brief Set ingress priority group statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_INGRESS_PRIORITY_GROUP_ATTR_SELECTIVE_COUNTER_LIST,
    /**
     * @brief Set buffer pool statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_BUFFER_POOL_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_BUFFER_POOL_ATTR_SELECTIVE_COUNTER_LIST,
```
**saimacsec.h**
```c
    /**
     * @brief Set macsec port statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_MACSEC_PORT_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
    * @default empty
     */
    SAI_MACSEC_PORT_ATTR_SELECTIVE_COUNTER_LIST,

    /**
     * @brief Set macsec flow statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_MACSEC_FLOW_ATTR_STATS_COUNT_MODE,
    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_MACSEC_FLOW_ATTR_SELECTIVE_COUNTER_LIST,
```
**saipolicer.h**
```c
    /**
     * @brief Set policer statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_POLICER_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_POLICER_ATTR_SELECTIVE_COUNTER_LIST,
```
**saiswitch.h**
```c
    /**
     * @brief Set switch statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_SWITCH_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_SWITCH_ATTR_SELECTIVE_COUNTER_LIST,
```
**saiipsec.h**
```c
    /**
     * @brief Set IPSEC port statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_IPSEC_PORT_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_IPSEC_PORT_ATTR_SELECTIVE_COUNTER_LIST,

     /**
     * @brief Set IPSEC SA statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_IPSEC_SA_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_IPSEC_SA_ATTR_SELECTIVE_COUNTER_LIST,
```
**saisrv6.h**
```c
    /**
     * @brief Set SRV6 SID List statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_SRV6_SIDLIST_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with  #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_SRV6_SIDLIST_ATTR_SELECTIVE_COUNTER_LIST,

```
**saitwamp.h**
```c
    /**
     * @brief Set TWAMP session statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_TWAMP_SESSION_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_TWAMP_SESSION_ATTR_SELECTIVE_COUNTER_LIST,
```
**saiicmpecho.h**
```c
    /**
     * @brief Set ICMP echo statistics counting mode
     *
     * @type sai_stats_count_mode_t
     * @flags CREATE_AND_SET
     * @default SAI_STATS_COUNT_MODE_PACKET_AND_BYTE
     */
    SAI_ICMP_ECHO_ATTR_STATS_COUNT_MODE,

    /**
     * @brief Attach counter object list
     *
     * Counter object should be of type Selective.
     * Fill (#SAI_COUNTER_ATTR_TYPE with #SAI_COUNTER_TYPE_SELECTIVE).
     *
     * @type sai_object_list_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_COUNTER
     * @default empty
     */
    SAI_ICMP_ECHO_ATTR_SELECTIVE_COUNTER_LIST,
```
#### Sample workflow
Query and save the stats capability during initialization.

```c
    uint32_t ROUTER_INTERFACE_MAX_STAT_ID = 0;
    uint32_t pktStatIdCnt = 0;
    uint32_t byteStatIdCnt = 0;

    vector <sai_int32_t> count_arr_pkt, count_arr_byte; // Array of supported stat-ids
    vector <sai_object_id_t> countOid; // Selective counter object mapped to stat-ids.
    vector<sai_stat_capability_t> statList;
    sai_stat_capability_list_t values = { .count = 0, .list = nullptr };

    auto status = sai_query_stats_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE, &values);
    if ((status != SAI_STATUS_SUCCESS) && (status != SAI_STATUS_BUFFER_OVERFLOW))
    {
        return status;
    }

    ROUTER_INTERFACE_MAX_STAT_ID = value.count;
    statList.resize(values.count);
    countArr.resize(values.count);
    values.list = statList.data();

    /* vendor to return actual count with supported stats-id list */
    status = sai_query_stats_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE,
                                        &values);
    if (status == SAI_STATUS_SUCCESS)
    {
        for(int32_t idx = 0; idx < values.count; idx ++)
        {
            countArr[idx] = values.list[idx].stat_enum;
            /* IS_BYTE_COUNTER macro identifies sairouterinterface byte/octet stat enum. */
            if (IS_BYTE_COUNTER(values.list[idx].stat_enum))
            {
                count_arr_byte[byteStatIdCnt++] = values.list[idx].stat_enum;
            }
            else
            {
                count_arr_pkt[pktStatIdCnt++] = values.list[idx].stat_enum;
            }
        }
    }
    else
    {
        /* All stats-id enums are counted */
    }
```
##### CREATE Object
```c
sai_status_t status = SAI_STATUS_SUCCESS;
sai_attr_capability_t rif_capability;
sai_attribute_t  attr;
vector<sai_attribute_t> attrs;

sai_attr_capability_t rif_count_mode_capability;
sai_stats_count_mode_t rifCntMode = SAI_STATS_COUNT_MODE_PACKET_AND_BYTE; //Default

status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE, SAI_ROUTER_INTERFACE_ATTR_STATS_COUNT_MODE, &rif_count_mode_capability);
if (status == SAI_STATUS_SUCCESS && rif_count_mode_capability.create_implemented)
{
    attr.id = SAI_ROUTER_INTERFACE_ATTR_STATS_COUNT_MODE;
    /* Read and set the platform default behavior */
    if (user_has_set_packet_or_byte_mode)
    {
        rifCntMode = user_config_mode;
    }
    attr.value.s32 = rifCntMode;
    attrs.push_back(attr);
}

/* Default counting when not supported by vendor SAI, must return
"rif_capability.create_implemented = false" */
status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE,
                 SAI_ROUTER_INTERFACE_ATTR_SELECTIVE_COUNTER_LIST , &rif_capability);
if (status == SAI_STATUS_SUCCESS && rif_capability.create_implemented && rifCntMode != SAI_STATS_COUNT_MODE_NONE)
{
    if (/*user-statid-list is empty*/)
    {
        vector<sai_object_id_t> oid_list(ROUTER_INTERFACE_MAX_STAT_ID); //Allocate place holder for all stats-ids
        attr.value.objlist.count = static_cast<uint32_t>(ROUTER_INTERFACE_MAX_STAT_ID);
        attr.value.objlist.list = oid_list.data();

        for(int32_t idx = 0; idx < ROUTER_INTERFACE_MAX_STAT_ID; idx ++)
        {
            /* Create selective counter object */
            cnt_attr.id = SAI_COUNTER_ATTR_TYPE;
            cnt_attr.value.s32 = SAI_COUNTER_TYPE_SELECTIVE;
            cnt_attrs.push_back(cnt_attr);
            cnt_attr.id = SAI_COUNTER_ATTR_OBJECT_TYPE;
            cnt_attr.value.s32 = SAI_OBJECT_TYPE_ROUTER_INTERFACE;
            cnt_attrs.push_back(cnt_attr);
            cnt_attr.id = SAI_COUNTER_ATTR_STAT_ID_LIST;
            cnt_attr.value.s32list.count = 1;
            cnt_attr.value.s32list.list = new int;
            cnt_attr.value.s32list.list[0] = countArr[idx];
            cnt_attrs.push_back(cnt_attr);
            if((rifCntMode == SAI_STATS_COUNT_MODE_PACKET_AND_BYTE || rifCntMode == SAI_STATS_COUNT_MODE_PACKET) &&
                    !IS_BYTE_COUNTER(countArr[idx]))
            {
                cnt_attr.id = SAI_COUNTER_ATTR_ENABLE_PACKET_COUNT;
                cnt_attr.value.booldata = true;
                cnt_attrs.push_back(cnt_attr);
                cnt_attr.id = SAI_COUNTER_ATTR_ENABLE_BYTE_COUNT;
                cnt_attr.value.booldata = false;
                cnt_attrs.push_back(cnt_attr);
            }
            else if((rifCntMode == SAI_STATS_COUNT_MODE_PACKET_AND_BYTE || rifCntMode == SAI_STATS_COUNT_MODE_BYTE) &&
                    IS_BYTE_COUNTER(countArr[idx]))
            {
                cnt_attr.id = SAI_COUNTER_ATTR_ENABLE_BYTE_COUNT;
                cnt_attr.value.booldata = true;
                cnt_attrs.push_back(cnt_attr);
                cnt_attr.id = SAI_COUNTER_ATTR_ENABLE_PACKET_COUNT;
                cnt_attr.value.booldata = false;
                cnt_attrs.push_back(cnt_attr);
            }
            status = sai_counter_api->sai_create_counter(&countOid[idx], gSwitchId, 5, cnt_attrs.data());

            attr.value.objlist.list[idx] = countOid[idx];
        }
    }
    else
    {
        /* Create Counter objets with user configured stat-ids */
    }
    attr.id = SAI_ROUTER_INTERFACE_ATTR_SELECTIVE_COUNTER_LIST;
    attrs.push_back(attr);
}

sai_status_t status = sai_router_intfs_api->create_router_interface(&port.m_rif_id, gSwitchId, (uint32_t)attrs.size(), attrs.data());
```
**Note that multiple stat-ids can map to same counter object.**

In below example, all packet statistics enums are mapped to one counter object and byte stats to another counter object.
```c
sai_stats_count_mode_t rifCntMode = SAI_STATS_COUNT_MODE_PACKET_AND_BYTE; //Default

status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE, SAI_ROUTER_INTERFACE_ATTR_STATS_COUNT_MODE,
                                        &rif_count_mode_capability);
if (status == SAI_STATUS_SUCCESS && rif_count_mode_capability.create_implemented)
{
    attr.id = SAI_ROUTER_INTERFACE_ATTR_STATS_COUNT_MODE;
    /* Read and set the platform default behavior */
    if (user_has_set_packet_or_byte_mode)
    {
        rifCntMode = user_config_mode;
    }
    attr.value.s32 = rifCntMode;
    attrs.push_back(attr);
}
    /* Default counting when not supported by vendor SAI, must return
    "rif_capability.create_implemented = false" */
status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE,
                        SAI_ROUTER_INTERFACE_ATTR_SELECTIVE_COUNTER_LIST , &rif_capability);
if (status == SAI_STATUS_SUCCESS && rif_capability.create_implemented && rifCntMode != SAI_STATS_COUNT_MODE_NONE)
{
    if (/*user-statid-list is empty*/)
    {
        vector<sai_object_id_t> oid_list(2); //Allocate place holder for all stats-ids
        attr.value.objlist.count = static_cast<uint32_t>(1);
        attr.value.objlist.list = oid_list.data();

        /* Create selective counter object for packet statistics Ids */
        cnt_attr.id = SAI_COUNTER_ATTR_TYPE;
        cnt_attr.value.s32 = SAI_COUNTER_TYPE_SELECTIVE;
        cnt_attrs.push_back(cnt_attr);
        cnt_attr.id = SAI_COUNTER_ATTR_OBJECT_TYPE;
        cnt_attr.value.s32 = SAI_OBJECT_TYPE_ROUTER_INTERFACE;
        cnt_attrs.push_back(cnt_attr);
        cnt_attr.id = SAI_COUNTER_ATTR_ENABLE_PACKET_COUNT;
        cnt_attr.value.booldata = true;
        cnt_attrs.push_back(cnt_attr);
        cnt_attr.id = SAI_COUNTER_ATTR_ENABLE_BYTE_COUNT;
        cnt_attr.value.booldata = false;
        cnt_attrs.push_back(cnt_attr);

        cnt_attr.id = SAI_COUNTER_ATTR_STAT_ID_LIST;
        cnt_attr.value.s32list.count = pktStatIdCnt;
        cnt_attr.value.s32list.list = new int(pktStatIdCnt);
        for(int32_t idx = 0; idx < pktStatIdCnt; idx ++)
        {
            cnt_attr.value.s32list.list[idx] = count_arr_pkt[idx];
        }
        cnt_attrs.push_back(cnt_attr);

        status = sai_counter_api->sai_create_counter(&countOid[0], gSwitchId, 5, cnt_attrs.data());

        /* Create selective counter object for byte statistics Ids */
        cnt_attr.id = SAI_COUNTER_ATTR_TYPE;
        cnt_attr.value.s32 = SAI_COUNTER_TYPE_SELECTIVE;
        cnt_attrs.push_back(cnt_attr);
        cnt_attr.id = SAI_COUNTER_ATTR_OBJECT_TYPE;
        cnt_attr.value.s32 = SAI_OBJECT_TYPE_ROUTER_INTERFACE;
        cnt_attrs.push_back(cnt_attr);

        cnt_attr.id = SAI_COUNTER_ATTR_ENABLE_BYTE_COUNT;
        cnt_attr.value.booldata = true;
        cnt_attrs.push_back(cnt_attr);
        cnt_attr.id = SAI_COUNTER_ATTR_ENABLE_PACKET_COUNT;
        cnt_attr.value.booldata = false;
        cnt_attrs.push_back(cnt_attr);

        cnt_attr.id = SAI_COUNTER_ATTR_STAT_ID_LIST;
        cnt_attr.value.s32list.count = byteStatIdCnt;
        cnt_attr.value.s32list.list = new int(byteStatIdCnt);
        for(int32_t idx = 0; idx < byteStatIdCnt; idx ++)
        {
            cnt_attr.value.s32list.list[idx] = count_arr_byte[idx];
        }
        cnt_attrs.push_back(cnt_attr);

        status = sai_counter_api->sai_create_counter(&countOid[1], gSwitchId, 5, cnt_attrs.data());
        attr.value.objlist.list[1] = countOid[1];
    }
    else
    {
        /* Create Counter objets with user configured stat-ids */
    }
    attr.id = SAI_ROUTER_INTERFACE_ATTR_SELECTIVE_COUNTER_LIST;
    attrs.push_back(attr);
}
sai_status_t status = sai_router_intfs_api->create_router_interface(&port.m_rif_id, gSwitchId, (uint32_t)attrs.size(), attrs.data());


```
##### SET Object
**Example flow to set only SAI_ROUTER_INTERFACE_ATTR_STATS_COUNT_MODE.**
```c
/* Example flow to set only SAI_ROUTER_INTERFACE_ATTR_STATS_COUNT_MODE.
   Assume rifCntMode is updated with current mode configured in create flow */
status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE, SAI_ROUTER_INTERFACE_ATTR_STATS_COUNT_MODE,
                                        &rif_count_mode_capability);

if (status == SAI_STATUS_SUCCESS && rif_count_mode_capability.set_implemented)
{
    attr.id = SAI_ROUTER_INTERFACE_ATTR_STATS_COUNT_MODE;
    /* Read and set the user configured value */
    if (user_has_set_packet_or_byte_mode)
    {
      rifCntMode = user_config_mode;
    }
    attr.value.s32 = rifCntMode;
}
sai_status_t status =
        sai_router_intfs_api->set_router_interface_attribute(port.m_rif_id, &attr);
```
##### Assumption
Note that user can modify the count mode (say initial config is "COUNT_MODE_PACKTE_AND_BYTE" and set to "COUNT_MODE_PACKET"). In such scenario, for optimal usage of HW counting resources, vendor SAI implementation may disable/delete speicific stat-id counter objects in HW based on count mode updated on the object. And susbsequently clean-up stat-id counter object up on removal of stat-id from user.

Say for e.g if current mode=SAI_STATS_COUNT_MODE_PACKET_AND_BYTE and selective counter list has statids={IN_PKT,IN_BYTE,OUT_PKT,OUT_BYTE} and user updates to new mode to SAI_STATS_COUNT_MODE_PACKET. In this case, internally disable/delete counting of all BYTE speicific counter in HW.
And when user deselects stat-id IN_BYTE and OUT_BYTE stat-ids, corresponding SAI counter objects can be removed.

If mode is NONE and counter-list has stat-ids={IN_PKT,IN_BYTE,OUT_PKT,OUT_BYTE}, none should be counted. If mode is set to PACKET, only "IN_PKT and OUT_PKT" should be counted.

**Example flow to set speicific stat-id.**

```c
/* Example flow to set speicific stat-id say SAI_ROUTER_INTERFACE_STAT_IN_PACKETS from north bound*/

sai_status_t status = SAI_STATUS_SUCCESS;
sai_attr_capability_t rif_capability;
sai_attribute_t  attr;
sai_object_id_t inCountOid = SAI_NULL_OBJECT_ID;
bool isStatIdFound = false;

for(int32_t idx = 0; idx < ROUTER_INTERFACE_MAX_STAT_ID; idx ++)
{
    if (countArr[idx] == SAI_ROUTER_INTERFACE_STAT_IN_PACKETS)
    {
        isStatIdFound = true;
        break;
    }
}

status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE, SAI_ROUTER_INTERFACE_ATTR_COUNTER_IDS, &rif_capability);

if (status == SAI_STATUS_SUCCESS && rif_capability.set_implemented && isStatIdFound)
{
    if(enable) /* Is specific enum enabled from north-bound */
    {
        if(countOid[idx] == SAI_NULL_OBJECT_ID)
        {
             /* Create selective counter object */
            cnt_attr.id = SAI_COUNTER_ATTR_TYPE;
            cnt_attr.value.s32 = SAI_COUNTER_TYPE_SELECTIVE;
            cnt_attrs.push_back(cnt_attr);
            cnt_attr.id = SAI_COUNTER_ATTR_OBJECT_TYPE;
            cnt_attr.value.s32 = SAI_OBJECT_TYPE_ROUTER_INTERFACE;
            cnt_attrs.push_back(cnt_attr);
            cnt_attr.id = SAI_COUNTER_ATTR_STAT_ID_LIST;
            cnt_attr.value.s32list.count = 1;
            cnt_attr.value.s32list.list = new int;
            cnt_attr.value.s32list.list[0] = countArr[idx]; 
            cnt_attrs.push_back(cnt_attr);
            if((rifCntMode == SAI_STATS_COUNT_MODE_PACKET_AND_BYTE || rifCntMode == SAI_STATS_COUNT_MODE_PACKET) &&
                !IS_BYTE_COUNTER(countArr[idx]))
            {
                cnt_attr.id = SAI_COUNTER_ATTR_ENABLE_PACKET_COUNT;
                cnt_attr.value.booldata = true;
                cnt_attrs.push_back(cnt_attr);
                cnt_attr.id = SAI_COUNTER_ATTR_ENABLE_BYTE_COUNT;
                cnt_attr.value.booldata = false;
                cnt_attrs.push_back(cnt_attr);
            }
            else if((rifCntMode == SAI_STATS_COUNT_MODE_PACKET_AND_BYTE || rifCntMode == SAI_STATS_COUNT_MODE_BYTE) &&
                IS_BYTE_COUNTER(countArr[idx]))
            {
                cnt_attr.id = SAI_COUNTER_ATTR_ENABLE_PACKET_COUNT;
                cnt_attr.value.booldata = false;
                cnt_attrs.push_back(cnt_attr);
                cnt_attr.id = SAI_COUNTER_ATTR_ENABLE_BYTE_COUNT;
                cnt_attr.value.booldata = true;
                cnt_attrs.push_back(cnt_attr);
            }
            status = sai_counter_api->sai_create_counter(&countOid[idx], gSwitchId, 5,  cnt_attrs.data());
        }
    }
    else
    {
        if(countOid[idx])
        {
            status = sai_counter_api->sai_remove_counter(&countOid[idx]);
        }
        countOid[idx] = SAI_NULL_OBJECT_ID;
    }

    /* Push the updated list to SAI */
    vector<sai_object_id_t> oid_list(ROUTER_INTERFACE_MAX_STAT_ID); //Allocate place holder for all stats-ids
    attr.value.objlist.count = static_cast<uint32_t>(ROUTER_INTERFACE_MAX_STAT_ID);
    attr.value.objlist.list = oid_list.data();
    attr.id = SAI_ROUTER_INTERFACE_ATTR_COUNTER_IDS;
    for(int32_t idx = 0; idx < ROUTER_INTERFACE_MAX_STAT_ID; idx ++)
    {
        attr.value.objlist.list[idx] = countOid[idx];
    }
}

sai_status_t status =
        sai_router_intfs_api->set_router_interface_attribute(port.m_rif_id, &attr);

```
In case of same selective counter Object mapped to multiple stat-ids, do sai_set_counter_attribute() with updated stat-id list and no operation required in sairouterinterface.

##### GET statistics
```c
/* Assume, countOid array is updated for the specific router-interface as part of create/set operation.*/

rifCntMode = SAI_STATS_COUNT_MODE_PACKET_AND_BYTE;
status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE,
                                        SAI_ROUTER_INTERFACE_ATTR_STATS_COUNT_MODE,
                                        &rif_count_mode_capability);
if (status == SAI_STATUS_SUCCESS && rif_count_mode_capability.get_implemented )
{
    sai_attribute_t attr;
    attr.id = SAI_ROUTER_INTERFACE_ATTR_STATS_COUNT_MODE;
    status = sai_router_intfs_api->get_router_interface_attr(port.m_rif_id,1, &attr);

    if(status == SAI_STATUS_SUCCESS)
    {
        rifCntMode = attr.value.s32;
    }

}

if(rifCntMode == SAI_STATS_COUNT_MODE_NONE)
{
    /* Counting Disabled */
    return;
}

status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE, SAI_ROUTER_INTERFACE_ATTR_COUNTER_IDS,
                                            &rif_capability);
/* Retrieve Specific stat-ids if supported */
if (status == SAI_STATUS_SUCCESS && rif_capability.get_implemented)
{
    sai_stat_id_t stat_id;
    uint64_t stats;
    for(int32_t idx = 0;i < SAI_ROUTER_INTERFACE_STAT_END; i++)
    {
        if(countOid[idx] != SAI_NULL_OBJECT_ID)
        {
            if((rifCntMode == SAI_STATS_COUNT_MODE_PACKET_AND_BYTE || rifCntMode == SAI_STATS_COUNT_MODE_PACKET) &&
                    !IS_BYTE_COUNTER(countArr[idx]))
            {
                stat_id = SAI_COUNTER_STAT_PACKETS;
            }
            else if((rifCntMode == SAI_STATS_COUNT_MODE_PACKET_AND_BYTE || rifCntMode == SAI_STATS_COUNT_MODE_BYTE) &&
                    IS_BYTE_COUNTER(countArr[idx]))
            {
                stat_id = SAI_COUNTER_STAT_BYTES;
            }
            status = sai_counter_api->get_counter_stats_ext(
                                countOid[idx],
                                1,/*count packet or byte */
                                &stat_id,
                                SAI_STATS_MODE_READ,
                                &stats);
        }
    }
}
else
{
    /* Only counter enable/disable at object level. Retrieve supported Packet or Byte stats based on count mode. */
    if (rif_count_mode_capability.get_implemented)
    {
        /* Get supported packet counters */
        std::vector<uint64_t> stats;
        if( rifCntMode == SAI_STATS_COUNT_MODE_PACKET_AND_BYTE || rifCntMode == SAI_STATS_COUNT_MODE_PACKET)
        {
            status = sai_router_intfs_api->get_router_interface_stats(
                        rid,
                        pktStatIdCnt,
                        count_arr_pkt,
                        stats.data());
        }
        /* Get supported byte counters */
        if( rifCntMode == SAI_STATS_COUNT_MODE_PACKET_AND_BYTE || rifCntMode == SAI_STATS_COUNT_MODE_BYTE)
        {
            status = sai_router_intfs_api->get_router_interface_stats(
                    rid,
                    byteStatIdCnt,
                    count_arr_byte,
                    stats.data());
        }
    }
    else
    {
        /* Existing Behavior. Retrieve all supported Packet or Byte stats*/

        sai_stat_capability_list_t stats_capability;
        std::vector<sai_stat_capability_t> stats;
        status = sai_query_stats_capability( gSwitchId,
                                            SAI_OBJECT_TYPE_ROUTER_INTERFACE,
                                            stats_capability.data());

        stats_capability.count = SAI_ROUTER_INTERFACE_STAT_END;
        stats_capability.list = stats.data();

        for (uint32_t cnt = 0; cnt < stats_capability.count; cnt++)
        {
            supported_counter_ids.push_back(stats_capability.list[cnt]);
        }

        status = sai_router_intfs_api->get_router_interface_stats(
                        rid,
                        (uint32_t)supported_counter_ids.size(),
                        (sai_stat_id_t *)supported_counter_ids.data(),
                        stats.data());
    }
}
```
#### Assumptions
- When both attributes ATTR_STATS_COUNT_MODE and ATTR_SELECTIVE_COUNTER_LIST is supported by vendor SAI, decision to count packet or byte stat-id ( mentioned in ATTR_SELECTIVE_COUNTER_LIST) is based on ATTR_STATS_COUNT_MODE value.
- Generic statistics count mode should be categorized as "SAI_STATS_COUNT_MODE_PACKET".
For example stat-ids like "SAI_PORT_STAT_PRBS_ERROR_COUNT/SAI_PORT_STAT_IF_OUT_FABRIC_DATA_UNITS" should be incremented under packet counters.

#### Pros
- Flexibility to enable/disable packet/byte/both counting per object.
- Flexiblity to count selective statistics per object.
- sai debug counter can use this feature to selectively count stat-ids mapped to debug-counter per object (port/switch).

#### Cons

- New workflow

**Prefered: Option 3**

## Warmboot Implications
None

