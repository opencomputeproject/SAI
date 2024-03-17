# SAI Counter enhancement
-------------------------------------------------------------------------------
 Title       | SAI counter enahncement
-------------|-----------------------------------------------------------------
 Authors     |<code> Rajkumar P R, Marvell Technology Inc <br>          Ravindranath C K, Marvell Technology Inc </code>
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

##### Create object:
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
##### Set Object:
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
##### Get statistics:
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
##### Create Object:
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
##### Set Object:
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
##### Get statistics:
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
This is an optimization to option 2 to reduce the number of attributes.

Introduce a new attribute value type holding list of stat_id to counter object.

**saitypes.h**
```
typedef struct
{
      /** Object type of the stat enum*/
      sai_object_type_t  object_type;

      /** Stat enum value */
      sai_stat_id_t      stat_enum;

      /** Counter ObjectId associated with stat enum */
      sai_object_id_t    counter_id;

} sai_counter_id_t;

typedef struct _sai_counter_list_t
{
    /** Number of stats */
    uint32_t count;

    /** List of stat-id to counter object */
    sai_counter_id_t *list;

} sai_counter_list_t;


typedef union _sai_attribute_value_t
{


    /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_STATID_COUNTER_LIST */
    sai_counter_list_t statidcounterlist;

} sai_attribute_value_t;


```
**saimetadatypes.h**

```
typedef enum _sai_attr_value_type_t
{

    /**
     * @brief Attribute value is STAT enum to COUNTER object list.
     */
    SAI_ATTR_VALUE_TYPE_STATID_COUNTER_LIST

} sai_attr_value_type_t;
```

Introduce an attribute  for stat-id counter object list in each of the object file.

**sairouterinterface.h**
```
 * @brief Router interface counter objects
 * Counter Object list for supported router interface stats enum
 * @type sai_counter_list_t
 * @flags CREATE_AND_SET
 * @allownull true
 * @default SAI_NULL_OBJECT_ID
 */
 SAI_ROUTER_INTERFACE_ATTR_COUNTER_IDS

```
**saivlan.h**
```
 * @brief Vlan counter objects
 * Counter Object list for supported vlan stats enum
 * @type sai_counter_list_t
 * @flags CREATE_AND_SET
 * @allownull true
 * @default SAI_NULL_OBJECT_ID
 */
 SAI_VLAN_ATTR_COUNTER_IDS

```
**saitunnel.h**
```
 * @brief Tunnel counter objects
 * Counter Object list for supported tunnel stats enum
 * @type sai_counter_list_t
 * @flags CREATE_AND_SET
 * @allownull true
 * @default SAI_NULL_OBJECT_ID
 */
 SAI_TUNNEL_ATTR_COUNTER_IDS

```
**saiqueue.h**
```
 * @brief Queue counter objects
 * Counter Object list for supported queue stats enum
 * @type sai_counter_list_t
 * @flags CREATE_AND_SET
 * @allownull true
 * @default SAI_NULL_OBJECT_ID
 */
 SAI_QUEUE_ATTR_COUNTER_IDS

```
**saiport.h**
```
 * @brief Port counter objects
 * Counter Object list for supported port stats enum
 * @type sai_counter_list_t
 * @flags CREATE_AND_SET
 * @allownull true
 * @default SAI_NULL_OBJECT_ID
 */
 SAI_PORT_ATTR_COUNTER_IDS

```
#### Sample workflow
Query and save the stats capability during initialization.

```
    uint32_t ROUTER_INTERFACE_MAX_STAT_ID = 0;

    vector <sai_int32_t> countArr;
    vector <sai_object_id_t> countOid; /* per routerinterface object */
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
        }
    }
    else
    {
        /* All stats-id enums are counted */
    }
```
##### Create Object:
```c
sai_status_t status = SAI_STATUS_SUCCESS;
sai_stat_capability_list_t rif_stats_capability;
sai_attr_capability_t rif_capability;
sai_attribute_t  attr;
vector<sai_attribute_t> attrs;

/* Default counting when not supported by vendor SAI, must return
   "rif_capability.create_implemented = false" */
status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE,                     SAI_ROUTER_INTERFACE_ATTR_COUNTER_IDS, &rif_capability);
if (status == SAI_STATUS_SUCCESS && rif_capability.create_implemented)
{
    vector<sai_counter_id_t> oid_list(ROUTER_INTERFACE_MAX_STAT_ID); //Allocate place holder for all stats-ids
    attr.value.statidcounterlist.count = static_cast<uint32_t>(ROUTER_INTERFACE_MAX_STAT_ID);
    attr.value.statidcounterlist.list = oid_list.data();

    for(int32_t idx = 0; idx < ROUTER_INTERFACE_MAX_STAT_ID; idx ++)
    {
        /* Create counter object */
        cnt_attr.id = SAI_COUNTER_ATTR_TYPE;
        cnt_attr.value.s32 = SAI_COUNTER_TYPE_REGULAR;
        cnt_attrs.push_back(cnt_attr);
        stat_id = countArr[idx];
        status = sai_counter_api->sai_create_counter(&countOid[idx], gSwitchId, 1, cnt_attrs.data());

        attr.value.objlist.list[idx].object_type = SAI_OBJECT_TYPE_ROUTER_INTERFACE;
        attr.value.objlist.list[idx].stat_enum = stat_id;
        attr.value.objlist.list[idx].counter_id = countOid[idx];
    }
    attr.id = SAI_ROUTER_INTERFACE_ATTR_COUNTER_IDS;
    attrs.push_back(attr);
}

sai_status_t status = sai_router_intfs_api->create_router_interface(&port.m_rif_id, gSwitchId, (uint32_t)attrs.size(), attrs.data());
```
##### Set Object:
```c
/* Example flow to set SAI_ROUTER_INTERFACE_STAT_IN_PACKETS*/

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
            /* Create counter object for IN_COUNTER */
            cnt_attr.id = SAI_COUNTER_ATTR_TYPE;
            cnt_attr.value.s32 = SAI_COUNTER_TYPE_REGULAR;
            cnt_attrs.push_back(cnt_attr);
            status = sai_counter_api->sai_create_counter(&countOid[idx], gSwitchId, 1, cnt_attrs.data());
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
        attr.value.objlist.list[idx].object_type = SAI_OBJECT_TYPE_ROUTER_INTERFACE;
        attr.value.objlist.list[idx].stat_enum = countArr[idx];
        attr.value.objlist.list[idx].counter_id = countOid[idx];
    }
}

sai_status_t status =
        sai_router_intfs_api->set_router_interface_attribute(port.m_rif_id, &attr);

```
##### Get statistics:
```c
/* Assume, countOid array is updated for the specific router-interface as part of create/set operation.*/

status = sai_query_attribute_capability(gSwitchId, SAI_OBJECT_TYPE_ROUTER_INTERFACE, SAI_ROUTER_INTERFACE_ATTR_COUNTER_IDS, &rif_capability);

if (status == SAI_STATUS_SUCCESS && rif_capability.get_implemented)
{
    sai_stat_id_t stat_ids[] = { SAI_COUNTER_STAT_PACKETS, SAI_COUNTER_STAT_BYTES };
    uint64_t stats[2];
    for(int32_t idx = 0;i < SAI_ROUTER_INTERFACE_STAT_END; i++)
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
else
{
    /* Existing Behavior */

    sai_stat_capability_list_t stats_capability;
    std::vector<uint64_t> stats;
    std::vector<sai_stat_capability_t> stats;
    status = sai_query_stats_capability(
        gSwitchId,
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

```
#### Pros
- Number of attributes reduced to 1
- Flexiblity to count selective statistics.

#### Cons

- New workflow with more attributes per object.

**Prefered: Option 3**

## Warmboot Implications
None

