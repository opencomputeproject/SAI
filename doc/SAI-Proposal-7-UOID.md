Switch Abstraction Interface Change Proposal
=====================

Title    | Unified SAI object id
-------- | ---
Authors  | Microsoft
Status   | In review
Type     | Standards track
Created  | 02/25/2015
SAI-Version | 0.9.2


----------

## Overview

SAI (Switch Abstraction Interface) defines a collection of APIs to create, delete, set and get attributes of SAI objects, such as port, lag, next hop, next hop groups. All these SAI objects are identified either using match key or object id. Match key is passed from upper application to SAI in the SAI create function call, whereas object id is returned by the SAI when an object is created during SAI create function calls. In current design, SAI route, fdb entry and neighbor entry are identified via match keys. All other SAI objects are identified using object id. The upper application can use both object id and match key to manage its attributes as well as object deletion. Their difference between them is that the object id can also be used as a reference to create association between two SAI objects, e.g., a SAI next hop object id referenced by a SAI route object, while there is no need to reference a match key based SAI object. Besides, duplicated match keys are not allowed.

> Since VLAN has well-defined range and user application need to explicitly specify the VLAN id, sai_vlan_id_t is given by user application instead of being allocated by SAI.

This proposal focuses on SAI object id. In current design, every object id based SAI object is identified using a unique object id type. For example, SAI port object is defined as *sai_port_id_t*, and SAI next hop object is defined as *sai_next_hop_id_t*. It does not assume that the object id itself alone be used to identify a SAI object. When user need to refer to another SAI object in a SAI attribute, he needs to specify both the SAI object type and the object ID. For example, the SAI route object current has two attributes, *SAI_ROUTE_ATTR_NEXT_HOP_ID* and *SAI_ROUTE_ATTR_NEXT_HOP_GROUP_ID*. When user wants to associate a SAI route to a SAI next hop object, he needs to use *SAI_ROUTE_ATTR_NEXT_HOP_ID*, while he needs to use *SAI_ROUTE_ATTR_NEXT_HOP_GROUP_ID* for a SAI next hop group object. In the future, as we may define new SAI objects as tunnel object and want to associate them with the SAI route object, we need to add more SAI route attributes to allow us to build such associations. Every time when we try to establish a new connection between different SAI object, we need to add attributes correspondingly. Such approach becomes inflexible to extend.

The fundamental problem here is because the SAI object id itself cannot be used to identify an SAI oject unique in current spec. Once the SAI object id can be used to uniquely identify a SAI object, the function call can then take a SAI object id, identify its type and then setup the association accordingly based on its object type. Therefore, we propose to use a single object id type (*sai_object_id_t*) to identify all SAI objects. In the above example, we will have only one attribute SAI_ROUTE_ATTR_NEXT_HOP_ID and we can pass different SAI objects such as SAI next hop id, next hop group id or tunnel id (if defined in the future) using this attribute. In this approach, the API gives enough flexibility to associate different SAI objects (even those new SAI objects yet to be defined) with the SAI route object while the implementation can decide what are supported and what are not.

The design here is similar to file descriptors defined in Unix. In Unix-like system, file descriptors can refer to any Unix file type named in a file system, such as regular files, Unix domain sockets, named pipes, network sockets and etc. on. The Unix APIs allow user to create a file descriptor of any file type and then perform common operations such as read/write on those descriptors. The file descriptor alone can be used to uniquely identify a file object. 

## Specification
 
- The *sai_object_id_t* will be used for all SAI object id returned by the SAI create function call. 
- The *sai_object_id_t* is used to uniquely identify a SAI object created. User can use the object id for set/get attribute calls, object deletion call, and an attribute value for other SAI objects.
- The *sai_object_id_t* is opaque to upper application.
- The *sai_object_id_t* type is *uint64_t*. 


> **Implementation note:**

> - For a specific SAI object set/get attribute function call, the implementation needs to validate whether the SAI object passed to the function call is a correct type. For example, the next hop set attribute function call should return SAI_STATUS_INVALID_OBJECT_TYPE when a non next hop object passed to this call.
> - In terms of SAI object association, the attribute allows the application to pass any SAI object. It's up to each platform/implementation to determine whether such associate is feasible or not, and return success or SAI_STATUS_NOT_SUPPORTED accordingly.
> - The possible implementation can be embed the SAI object type into the object id. For example, the implementation can use upper 16 bits as the object type and lower 48 bits as the object id. 
> - It can embed the object id returned by the vendor (usually *uint32_t*) into the lower 48 bits. 
> - Upper 16 bits allow the SAI to define 65536 SAI object types.

## Examples
### sai_object_id_t and attribute value

    typedef uint64_t sai_object_id_t;
    typedef union {
       ...
       sai_object_id_t oid;
       ...
    } sai_attribute_value_t;
    
### Create API

    typedef sai_status_t (*sai_create_next_hop_fn)(
        _Out_ sai_object_id_t* next_hop_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);
        
### Remove API

    typedef sai_status_t (*sai_remove_next_hop_fn)(
        _In_ sai_object_id_t next_hop_id);

### Set/Get API
    typedef sai_status_t (*sai_set_next_hop_attribute_fn)(
        _In_ sai_object_id_t next_hop_id,
        _In_ const sai_attribute_t *attr);
    typedef sai_status_t (*sai_get_next_hop_attribute_fn)(
        _In_ sai_object_id_t next_hop_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list);

### SAI object association
    /*
    *  Attribute Id for sai route object
    */
    typedef enum _sai_route_attr_t
    {
        /* READ-WRITE */
        /* Packet action [sai_packet_action_t]
           (default to SAI_PACKET_ACTION_FORWARD) */
        SAI_ROUTE_ATTR_PACKET_ACTION,
        
        /* Packet priority for trap/log actions [uint8_t]
           (default to 0) */
        SAI_ROUTE_ATTR_TRAP_PRIORITY,

        /* Next hop sai object id for the packet [sai_object_id_t],
         * can be next hop, next hop group objects and etc. on */
        SAI_ROUTE_ATTR_NEXT_HOP_ID,

        /* Custom range base value */
        SAI_ROUTE_ATTR_CUSTOM_RANGE_BASE  = 0x10000000
    } sai_route_attr_t;
