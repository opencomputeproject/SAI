Switch Abstraction Interface Change Proposal

=====================
Title    | Unified Object Model
-------- | ---
Authors  | Microsoft
Status   | In review
Type     | Standards track
Created  | 04/12/2016
SAI-Version | 0.9.5

----------

## Overview


SAI is to provide unified API to configure switch ASIC resources. Every type of ASIC resources is represented by a type of SAI object with four operations: create, remove, set and get. With all these SAI objects being defined, the ASIC state can now be represented using a set of SAI objects, e.g., port, routes, vlan, nexthop.  

This set of SAI objects (later referred as SAI view) can be used for many purposes. For example, we can use these SAI objects to capture current ASIC states and later used for restore the ASIC states after ASIC re-initialization. We can also compare two sets of SAI objects and find their differences and then make transition from one ASIC state for another ASIC state. 

Most of SAI objects are now using Unified Object ID and CRUD API to manage them. However, some SAI objects are still having specific APIs to manage. For example, VLAN has add/remove ports API to add/remove ports from VLAN. These APIs complicates the operations needed to restore ASIC states from a SAI view, or transition from one SAI view to another SAI view, because the program needs to use these specific APIs to restore or make a transition. This also applies to port break out operation where specific attributes is used for such operation.

This proposal here is to unified the object model where all SAI objects should only use CRUD APIs to manage, which provides unified API to perform SAI view restoration and transition. For example, we proposal to use VLAN member to replace existing VLAN port add/remove API. Previously, we need to use add/remove APIs to change VLAN port membership. Now, we are using same VLAN member create call to achieve same goal. Consider the SAI transition problem, if we find any difference in two SAI view, in this case the vlan membership difference. In current API, we need to use object specific API to change the vlan membership. Now, we can use same create function to achieve. Same applies to port breakout. This is similar to use same C++ template for all SAI objects where all of SAI objects provide same API for management. 

### Hidden SAI Objects after switch initialization

By default, some SAI objects are created during switch initialization, such as port, default VLAN. In order for the SAI view to capture the full ASIC states, such objects must be queryable through SAI API. For this purpose, we will be using sai\_get\_object\_key which was purpose in the warmboot proposal for this purpose.

## Examples

## Spec

Following changes to current SAI spec are proposed.

- Add VLAN member object
- Add ECMP group member object
- Change TRAP ID to unified object ID, add Create/Remove API for trap id
- Add Port Create/Remove API to handle port breakout/break-in 
- Add Queue Create/Remove API for dynamic queue creation/deletion 

### VLAN member
    
Add VLAN member object and create/remove/get/set API. 

    typedef enum _sai_vlan_member_attr_t {
 
        /** READ_WRITE */
 
        /** VLAN ID [sai_vlan_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
        SAI_VLAN_MEMBER_ATTR_VLAN_ID,
 
        /** logical port ID [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
        SAI_VLAN_MEMBER_ATTR_PORT_ID,
 
        /** VLAN tagging mode [sai_vlan_tagging_mode_t] (CREATE_AND_SET)
         * (default to SAI_VLAN_PORT_UNTAGGED) */
        SAI_VLAN_MEMBER_ATTR_TAGGING_MODE,
 
        /** custom range base value */
        SAI_VLAN_MEMBER_ATTR_CUSTOM_RANGE_BASE  = 0x10000000
 
    } sai_vlan_member_attr_t;

    typedef sai_status_t (*sai_create_vlan_member_fn)(
        _Out_ sai_object_id_t* vlan_member_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list
        );

    typedef sai_status_t (*sai_remove_vlan_member_fn)(
        _In_ sai_object_id_t vlan_member_id
       );

    typedef sai_status_t (*sai_set_vlan_member_attribute_fn)(
       _In_ sai_object_id_t vlan_member_id,
       _In_ const sai_attribute_t *attr
       );
 
    typedef sai_status_t (*sai_get_vlan_member_attribute_fn)(
       _In_ sai_object_id_t vlan_member_id,
       _In_ const uint32_t attr_count,
       _Inout_ sai_attribute_t *attr_list
       );

In the VLAN object, add SAI_VLAN_ATTR_MEMBER_LIST to query the vlan members. remove SAI_VLAN_ATTR_PORT_LIST. 
 
    typedef enum _sai_vlan_attr_t
    {
        /** READ-ONLY */

	    /** List of vlan member ports in a VLAN [sai_object_list_t] */
	    SAI_VLAN_ATTR_MEMBER_LIST,
    }

### Next hop group member

Add next hop group member object and create/remove/get/set API.

    typedef enum _sai_next_hop_group_member_attr_t {
 
        /** READ_WRITE */
 
        /** NEXT HOP GROUP ID [sai_vlan_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
        SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID,
 
        /** logical port ID [sai_object_id_t] (MANDATORY_ON_CREATE|CREATE_ONLY) */
        SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID,
 
        /** NEXT HOP GROUP Weights (CREATE_AND_SET)
         * (default to 1) */
        SAI_NEXT_HOP_GROUP_MEMBER_ATTR_WEIGHT,
 
        /** custom range base value */
        SAI_NEXT_HOP_GROUP_MEMBER_ATTR_CUSTOM_RANGE_BASE  = 0x10000000
 
    } sai_next_hop_group_member_attr_t;


    typedef sai_status_t (*sai_create_next_hop_group_member_fn)(
        _Out_ sai_object_id_t* next_hop_group_member_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list
        );

    typedef sai_status_t (*sai_remove_next_hop_group_member_fn)(
        _In_ sai_object_id_t next_hop_group_member_id
       );

    typedef sai_status_t (*sai_set_next_hop_group_member_attribute_fn)(
       _In_ sai_object_id_t next_hop_group_member_id,
       _In_ const sai_attribute_t *attr
       );
 
    typedef sai_status_t (*sai_get_next_hop_group_member_attribute_fn)(
       _In_ sai_object_id_t next_hop_group_member_id,
       _In_ const uint32_t attr_count,
       _Inout_ sai_attribute_t *attr_list
       );

In the nexthopgroup object, remove SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST attribute. 

### HOSTIF TRAP

Change host interface trap id to to unified object ID, add create/remove hostif trap API. SAI_HOSTIF_TRAP_ATTR_TRAP_ID is added to specify which trap ID to create. This attribute has a KEY annotation, which means this attribute must be unique. The user cannot create two trap objects with same TRAP_ID. When the user tries to create a second trap object with the same TRAP_ID, SAI_STATUS_ITEM_ALREADY_EXISTS should be returned.

When a HOSTIF TRAP is not created, ASIC should do NOOP operation to the related packets in the pipeline. Duration SAI initialization, some HOSTIF TRAP can be created by default to put the switch into a default working state, for example, trap TTL_ERROR/L3_MTU_ERROR packets, drop STP/LACP/EAPOL/LLDP/PVRST packets as currently defined in the SAI spec. As stated earlier, such trap object must be queryable after switch initialization.

    typedef enum _sai_hostif_trap_attr_t
    {
        /** Host interface trap ID [sai_hostif_trap_id_t]
         * (CREATE_ONLY|MANDATORY_ON_CREATE|KEY) */
        SAI_HOSTIF_TRAP_ATTR_TRAP_ID,

        ...
    }

    typedef sai_status_t(*sai_create_hostif_trap_fn)(
        _Out sai_object_id_t hostif_trapid,
        _In_ _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list
        );

    typedef sai_status_t(*sai_remove_hostif_trap_fn)(
        _In_ sai_object_id_t hostif_trapid,
        );

    typedef sai_status_t(*sai_set_hostif_trap_attribute_fn)(
        _In_ sai_object_id_t hostif_trapid,
        _In_ const sai_attribute_t *attr
        );

    typedef sai_status_t(*sai_get_hostif_trap_attribute_fn)(
        _In_ sai_object_id_t hostif_trapid,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list
        );

### PORT

Use the port creation/removal API to do port breakout/break-in operation. SAI_PORT_ATTR_HW_LANE_LIST and SAI_PORT_ATTR_SPEED are mandatory attributes for port creation. Breakout can be achieved using following sequence, 1) remove a port, 2) create port for each lane.

    typedef enum _sai_port_attr_t
    {
        /** READ-WRITE */
       
		/** Hardware Lane list [sai_u32_list_t] 
         * (CREATE_ONLY|MANDATORY_ON_CREATE) */
        SAI_PORT_ATTR_HW_LANE_LIST,

        /** Speed in Mbps [uint32_t]
         * (MANDATORY_ON_CREATE|CREATE_AND_SET) */
        SAI_PORT_ATTR_SPEED,
    }

    typedef sai_status_t(*sai_create_port_fn)(
        _Out_ sai_object_id_t* port_id,
        _In_ uint32_t attr_count,
        _In_ sai_attribute_t *attr_list
       );

    typedef sai_status_t(*sai_remove_port_fn)(
        _In_ sai_object_id_t  port_id
        );

### QUEUE

Use the queue create/remove API to support dynamic queue creation. SAI_QUEUE_ATTR_TYPE and SAI_QUEUE_ATTR_INDEX are mandatory attributes for queue creation.

    typedef enum _sai_queue_attr_t
    {
        /** READ-WRITE */
        /** Queue type [sai_queue_type_t] 
         * (CREATE_ONLY|MANDATORY_ON_CREATE) */
        SAI_QUEUE_ATTR_TYPE = 0x00000000,
 
        /* Queue index [sai_uint8_t] 
         * (CREATE_ONLY|MANDATORY_ON_CREATE) */
        SAI_QUEUE_ATTR_INDEX,
    }

    typedef sai_status_t(*sai_create_queue_fn)(
        _Out_ sai_object_id_t* queue_id,
        _In_ uint32_t attr_count,
        _In_ sai_attribute_t *attr_list
       );

    typedef sai_status_t(*sai_remove_queue_fn)(
        _In_ sai_object_id_t  queue_id
        );