# Configurable Default CPU Port Buffer Pools

Title       | Configurable Default CPU Port Buffer Pools
------------|--------------------------------------------
Authors     | Nvidia
Status      | Draft
Type        | Standards track
Created     | 2025-05-13
SAI-Version | 1.16

## Overview

Introducing read-only attributes to the SAI switch object which allow the user
to get SAI object IDs for default CPU Port Buffer Pools.

An adapter which does not support this feature or does not have default
internal Buffer Pools for the CPU Port may return a null object ID.

This follows the existing pattern for exposing internal default objects, like
default virtual router ID (`SAI_SWITCH_ATTR_DEFAULT_VIRTUAL_ROUTER_ID`).

## Motivation

Switches may handle traffic to and from CPU port with specific
internally-managed buffer pools, present from start-of-day with some default
configuration. The new attributes introduced in this change allow an adapter to
expose those default buffer pools to the user so they can be configured and
used in QoS configuration.

SAI-internal buffer pools could be queried using the existing `get_object_key`
API, however the existing buffer bool attributes are not sufficient to identify
buffer pools intended for use with the CPU Port.

The proposed change therefore covers these two requirements (querying initial
buffer pool state, and differentiating buffer pools for use with the CPU Port)
by following the existing pattern for exposing specific internal default
objects.

## Technical Specification

### New SAI switch attributes

```c
typedef enum _sai_switch_attr_t
{
    // ... existing attributes ...

    /**
     * @brief Default Ingress Buffer Pool for CPU Port usage.
     *
     * This object id references an internal SAI-managed default buffer
     * pool for the CPU Port. It may be used to configure the buffer pool
     * attributes or reference the buffer pool from other QoS objects.
     *
     * The object id is read only, while the object attributes can be modified.
     * Must return #SAI_NULL_OBJECT_ID if no such internal default buffer pool exists.
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_BUFFER_POOL
     * @allownull true
     * @default internal
     */
    SAI_SWITCH_ATTR_DEFAULT_CPU_INGRESS_BUFFER_POOL,

    /**
     * @brief Default Egress Buffer Pool for CPU Port usage.
     *
     * This object id references an internal SAI-managed default buffer
     * pool for the CPU Port. It may be used to configure the buffer pool
     * attributes or reference the buffer pool from other QoS objects.
     *
     * The object id is read only, while the object attributes can be modified.
     * Must return #SAI_NULL_OBJECT_ID if no such internal default buffer pool exists.
     *
     * @type sai_object_id_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_BUFFER_POOL
     * @allownull true
     * @default internal
     */
    SAI_SWITCH_ATTR_DEFAULT_CPU_EGRESS_BUFFER_POOL,

    // ... existing attributes ...
} sai_switch_attr_t;
```

## API flexibility

The proposed API is suitably flexible:

* The attributes of the default buffer pools are implementation specific. This
  does not introduce any restrictions on the default system behaviour.
* If no such default buffer pool exists, or this API is not supported, SAI
  simply returns a Null object ID, and the user may create and manage buffer
  pools as normal.
* Adapters are not required to support both ingress and egress attributes. They
  can support both, one, or neither.
* The default object pools may be removed, if supported by the adapter. Notably
  this allows the user to replace the default buffer pool if required (e.g. to
  change the immutable SAI_BUFFER_POOL_ATTR_THRESHOLD_MODE).


## Usage Example

```c
// Get default egress buffer pool
sai_attribute_t attr;
attr.id = SAI_SWITCH_ATTR_DEFAULT_CPU_EGRESS_BUFFER_POOL;

sai_status_t status = sai_switch_api->get_switch_attribute(
    switch_id,
    1,
    &attr);
sai_object_id egress_pool_id = attr.value.oid;

// Configure it
attr.id = SAI_BUFFER_POOL_ATTR_SIZE;
attr.value.u64 = 10000000;
status = sai_buffer_api->set_buffer_pool_attribute(egress_pool_id, &attr);

// Use it in a Buffer Profile
sai_object_id buffer_profile_id;
sai_attribute_t attrs[3];
attrs[0].id = SAI_BUFFER_PROFILE_ATTR_POOL_ID;
attrs[0].value.oid = egress_pool_id;
attrs[1].id = SAI_BUFFER_PROFILE_ATTR_BUFFER_SIZE;
attrs[1].value.u64 = 1000000;
attrs[2].id = SAI_BUFFER_PROFILE_ATTR_SHARED_DYNAMIC_TH;
attrs[2].value.s8 = 2;
status = sai_buffer_api->create_buffer_profile(&buffer_profile_id, switch_id, 3, attrs);

// Assign it to CPU port queues
sai_object_id queue_id;
sai_attribute_t attrs[5];
attrs[0].id = SAI_QUEUE_ATTR_TYPE;
attrs[0].value.s32 = SAI_QUEUE_TYPE_UNICAST;
attrs[1].id = SAI_QUEUE_ATTR_PORT;
attrs[1].value.oid = cpu_port_id;
attrs[2].id = SAI_QUEUE_ATTR_PARENT_SCHEDULER_NODE;
attrs[2].value.oid = cpu_port_id;
attrs[3].id = SAI_QUEUE_ATTR_BUFFER_PROFILE_ID;
attrs[3].value.oid = buffer_profile_id;
attrs[4].id = SAI_QUEUE_ATTR_INDEX;
attrs[4].value.u8 = 0;
sai_queue_api->create_queue(&queue_id, switch_id, 5, attrs));
```
