# MACSec and IPSec FIPS Compliance
-------------------------------------------------------------------------------
 Title       | MACSec and IPSec FIPS Compliance
-------------|-----------------------------------------------------------------
 Authors     | Jai Kumar (Broadcom Inc.)
 Status      | In review
 Type        | Standards track
 Created     | 2025-03-31
 SAI-Version | 1.17
-------------------------------------------------------------------------------

## 1.0  Introduction

This document describes the SAI API interaction and enhancements for triggering POST for FIPS 140-3 standard compliance to overall security level 1.


Networking Operating System (NOS) need to have cryptographic software components. "Library" which is a module providing cryptographic algorithms implies “FIPS inside” approach in the NOS. The “FIPS inside” is a way of designating one or more crypto modules with smaller boundaries, instead of one product-wise perimeter, which would encompass non-cryptographic components inside, all of them having code freeze caveat. 

This document proposes a SAI specification that will be at the MACSec/IPSec engine boundary where a single engine may be serving multiple MACSec/IPSec ports or there may be a one to one binding of the engine to the MACSec/IPSec port.

Current SAI specification already supports the concept of MACSec and IPSec objects and ports. It also supports the concept of one to one binding of engine to port and/or engine servicing to more than one ports, where engine is the MACSec/IPSec module and the port is corresponding MACSec/IPSec port.

Bring up sequence of MACSec and IPSec engine at a object boundary is as follows

1. Switch create
2. MACSec/IPSec object create
3. MACSec/IPSec port object create
4. Once the ports are created and SA association and other attributes are set, the port is ready to rx/tx traffic.

This document introduces a stage called as Pre-Operational Self-Test "POST" before the port is operational to rx/tx traffic.

FIPS-103 compliance requires that POST is executed on each port and only if POST completes with ‘success’, port must be enabled for admitting traffic, else port will remain in down state.

There are two variations of MACSec/IPSec engine and ports
1. Each MACSec/IPSec emgine serving ‘n’ number of ports (1::n)
2. Each port has its corresponding MACSec/IPSec engine (1::1)

![Fig-2](./ipsecFig2.png)

SAI_OBJECT_TYPE_MACSEC and SAI_OBJECT_TYPE_ IPSEC already exists
SAI_MACSEC_ATTR_SUPPORTED_PORT_LIST and SAI_IPSEC_ATTR_SUPPORTED_PORT_LIST provides the list of ports being service by an instance of MACSEC and IPSEC objects respectively

SAI workflow always first initialize the MACSEC/IPSEC engine/object before triggering POST on all the ports being hosted by the MACSEC/IPSEC engine

Till the POST is complete all the MACSEC/IPSEC ports are in a ’DOWN’ state.



## 2.0 Enhancements

New attribute SAI_MACSEC_ATTR_ENABLE_POST/SAI_IPSEC_ATTR_ENABLE_POST is introduced to start the POST and read the status of POST once it is completed.

### 2.1 MACSec Engine
Each MACSec engine is represented by a MACSec SAI object. Single MACSec object can be serving one or more MACSec ports.

New READ only SAI_MACSEC_ATTR_POST_STATUS  attribute is introduced to read the completion status of the MACSec object. Note that status reflects the aggregate status for all the ports served by this MACSec object. Even if a single port fails the POST, status for the MACSec object will be returned as SAI_MACSEC_POST_STATUS_FAIL.

Subsequently NOS has to query individual ports served by the MACSec object to figure out which port has failed the POST. This approach works for both the kind of MACSec engines 1::1 or 1::N


```
/**
 * @brief Attribute data for #SAI_MACSEC_ATTR_POST_STATUS,
 */
typedef enum _sai_macsec_post_status_t
{
    /** Unknown */
    SAI_MACSEC_POST_STATUS_UNKNOWN,

    /** Pass */
    SAI_MACSEC_POST_STATUS_PASS,

    /** In Progress */
    SAI_MACSEC_POST_STATUS_IN_PROGRESS,

    /** Fail */
    SAI_MACSEC_POST_STATUS_FAIL,
} sai_macsec_post_status_t;

/**
 * @brief Attribute Id for sai_macsec
 */
typedef enum _sai_macsec_attr_t
{
    . . .
    /**
     * @brief MACSEC POST status
     * Attribute to query the status of POST for a MACSEC engine
     *
     * @type sai_macsec_post_status_t
     * @flags READ_ONLY
     */
    SAI_MACSEC_ATTR_POST_STATUS,

    /**
     * @brief Setting the value to true will start the post on all the ports serviced by this MACSEC engine
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_MACSEC_ATTR_ENABLE_POST,
    . . .

} sai_macsec_attr_t;
```


#### 2.1.1 MACSec Port

New READ only attribute SAI_MACSEC_PORT_ATTR_POST_STATUS is introduced to read the status of POST for a given MACSec port.
This attribute can be read after the POST is completed for the MACSec object hosting this port.

```
/**
 * @brief Attribute data for #SAI_MACSEC_PORT_ATTR_POST_STATUS
 */
typedef enum _sai_macsec_port_post_status_t
{
    /** Unknown */
    SAI_MACSEC_PORT_POST_STATUS_UNKNOWN,

    /** Pass */
    SAI_MACSEC_PORT_POST_STATUS_PASS,

    /** Fail */
    SAI_MACSEC_PORT_POST_STATUS_FAIL,

} sai_macsec_port_post_status_t;

/**
 * @brief Attribute Id for sai_macsec_port
 */
typedef enum _sai_macsec_port_attr_t
{
    . . .

    /**
     * @brief MACSEC Port POST completion status
     *
     * Attribute to query the status of POST for a MACSEC port
     *
     * @type sai_macsec_port_post_status_t
     * @flags READ_ONLY
     */
    SAI_MACSEC_PORT_ATTR_POST_STATUS,
    
    . . .

} sai_macsec_port_attr_t;
```


### 2.2 IPSec Engine

Each IPSec engine is represented by an IPSec SAI object. Single IPSec object can serve one or more IPSec ports.

New READ only SAI_IPSEC_ATTR_POST_STATUS  attribute is introduced to read the completion status of the IPSec object. Note that status reflects the aggregate status for all the ports served by this IPSec object. Even if a single port fails the POST, status for IPSec objects will be returned as SAI_IPSEC_POST_STATUS_FAIL.

Subsequently NOS has to query individual ports served by the IPSec object to figure out which port has failed the POST. This approach works for both the kind of MACSec engines (1::1 or 1::N).

```
/**
 * @brief Attribute data for #SAI_IPSEC_ATTR_POST_STATUS,
 */
typedef enum _sai_ipsec_post_status_t
{
    /** Unknown */
    SAI_IPSEC_POST_STATUS_UNKNOWN,

    /** Pass */
    SAI_IPSEC_POST_STATUS_PASS,

    /** In Progress */
    SAI_IPSEC_POST_STATUS_IN_PROGRESS,

    /** Fail */
    SAI_IPSEC_POST_STATUS_FAIL,
} sai_ipsec_post_status_t;

/**
 * @brief Attribute Id for sai_ipsec
 */
typedef enum _sai_ipsec_attr_t
{
    . . .

    /**
     * @brief IPSEC POST status
     * Attribute to query the status of POST for an IPSEC engine
     *
     * @type sai_ipsec_post_status_t
     * @flags READ_ONLY
     */
    SAI_IPSEC_ATTR_POST_STATUS,

    /**
     * @brief Setting the value to true will start the post on all the ports serviced by this IPSEC engine
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_IPSEC_ATTR_ENABLE_POST,


```

#### 2.2.1 IPSec Port

New READ only attribute SAI_IPSEC_PORT_ATTR_POST_STATUS is introduced to read the status of POST for a given IPSec port.
This attribute can be read after the POST is completed for the IPSec object hosting this port.

```
/**
 * @brief Attribute data for #SAI_IPSEC_PORT_ATTR_POST_STATUS
 */
typedef enum _sai_ipsec_port_post_status_t
{
    /** Unknown */
    SAI_IPSEC_PORT_POST_STATUS_UNKNOWN,

    /** Pass */
    SAI_IPSEC_PORT_POST_STATUS_PASS,

    /** Fail */
    SAI_IPSEC_PORT_POST_STATUS_FAIL,

} sai_ipsec_port_post_status_t;

/**
 * @brief Attribute Id for sai_ipsec_port
 */
typedef enum _sai_ipsec_port_attr_t
{ 
    . . .

    /**
     * @brief IPSEC Port POST completion status
     *
     * Attribute to query the status of POST for a IPSEC port
     *
     * @type sai_ipsec_port_post_status_t
     * @flags READ_ONLY
     */
    SAI_IPSEC_PORT_ATTR_POST_STATUS,
    . . .

} sai_ipsec_port_attr_t;

```


### 3.0 Completion Callback

Single aggregate callback function is provided to return the status of POST status for the entire MACSec or IPSec engine. 

If the engine is servicing a single port then this callback essentially becomes a per port callback.

If the engine is servicing multiple ports then each port needs to be queried for its POST status using the READ only attribute of the port.

#### 3.1 MACSec POST Completion Callback

```
/**
 * @brief MACSEC post status notification
 *
 * @objects switch_id SAI_OBJECT_TYPE_MACSEC
 *
 * @param[in] macsec_id MACSEC Id
 * @param[in] macsec_post_status MACSEC post status
 */
typedef void (*sai_macsec_post_status_notification_fn)(
        _In_ sai_object_id_t macsec_id,
        _In_ sai_macsec_post_status_t macsec_post_status);

/**
 * @brief Attribute Id in sai_set_switch_attribute() and
 * sai_get_switch_attribute() calls.
 */
typedef enum _sai_switch_attr_t
{
    /**
     * @brief Callback for completion status of all the MACSEC ports serviced by this MACSEC engine
     *
     * Use sai_macsec_post_status_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_macsec_post_status_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_MACSEC_POST_STATUS_NOTIFY,

```


#### 3.2 IPSec POST Completion Callback
 
```
/**
 * @brief IPSEC post status notification
 *
 * @objects switch_id SAI_OBJECT_TYPE_IPSEC
 *
 * @param[in] ipsec_id IPSEC Id
 * @param[in] ipsec_post_status IPSEC post status
 */
typedef void (*sai_ipsec_post_status_notification_fn)(
        _In_ sai_object_id_t ipsec_id,
        _In_ sai_ipsec_post_status_t ipsec_post_status);

/**
 * @brief Attribute Id in sai_set_switch_attribute() and
 * sai_get_switch_attribute() calls.
 */
typedef enum _sai_switch_attr_t
{
    /**
     * @brief Callback for completion status of all the IPSEC ports serviced by this IPSEC engine
     *
     * Use sai_ipsec_post_status_notification_fn as notification function.
     *
     * @type sai_pointer_t sai_ipsec_post_status_notification_fn
     * @flags CREATE_AND_SET
     * @default NULL
     */
    SAI_SWITCH_ATTR_IPSEC_POST_STATUS_NOTIFY,
```


### 4.0 Example Workflow

Following steps are completed before enabling POST on the MACSec engine.


**Step 1:**
Switch create is complete and POST completion callback registraton SAI_SWITCH_ATTR_MACSEC_POST_STATUS_NOTIFY is done.

**Step 2: **
MACSec Object creation is complete

**Step 3: **
MACSec port object creation is complete
- sai_switch_create_fn()
- sai_create_macsec_fn()
- sai_create_macsec_port_fn()


**Step 4:** 
Create a MACSec object with POST attribute set as true. This will trigger POST on all the ports hosted by the engine.

```
 attrs.clear();

 sai_object_id_t macsec_obj;

 attr.id = SAI_MACSEC_ATTR_ENABLE_POST;
 attr.value.bool = true;
 attrs.push_back(attr);


 sai_macsec_api->create_macsec(
     &macsec_obj,
     switch_id,
     attrs.size(),
     attrs.data());
```

**Step 5:** 
Once the POST is completed on all the ports hosted by the MACSec engine, registered post macsec callback will be called by the SAI adapter.
If the status is SAI_MACSEC_POST_STATUS_FAIL, NOS MUST read the post status of all the ports hosted by the engine to find out which port POST has failed.

for all macsec_port_obj in macsec_obj->SAI_MACSEC_ATTR_SUPPORTED_PORT_LIST:
   read(macsec_port_obj->SAI_MACSEC_PORT_ATTR_POST_STATUS)

**Step 6: **
Set the POST enable flag in the macsec object to false. This is mainly for the hw where POST can be triggered runtime after the initialization.

```
 attrs.clear();

 sai_object_id_t macsec_obj;

 attr.id = SAI_MACSEC_ATTR_ENABLE_POST;
 attr.value.bool = false;
 attrs.push_back(attr);


 sai_macsec_api->create_macsec(
     &macsec_obj,
     switch_id,
     attrs.size(),
     attrs.data());
```
 


