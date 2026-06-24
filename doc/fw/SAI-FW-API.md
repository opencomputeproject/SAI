# Firmware Instances SAI Specification
-------------------------------------------------------------------------------
 Title       | SAI Firmware API
:-------------|:-----------------------------------------------------------------
 Authors     | Jai Kumar, Broadcom Inc
 Status      | In review
 Type        | Standards track
 Created     | 06/24/2026: Initial Draft
 SAI-Version | 1.19
-------------------------------------------------------------------------------


## 1.0 Introduction
Modern switches support one or more embedded controllers for low latency performance sensitive functions. These functions are implemented as embedded software modules aka firmware running on these controllers. There can be one or more set of firmwares running on one or more controllers. This specification introduces set of APIs to manage the firmware running on the controller.


## 2.0 Overview
The SAI Firmware API provides a standardized mechanism for managing firmware instances associated with a switch object. The API supports:

* Firmware object creation
* Firmware lifecycle management including start and stop of firmware operations
* Firmware state monitoring
* Runtime firmware configuration

The API is exposed through the `sai_fw_api_t` method table, using a new FW object.

### 2.1 Firmware Object Lifecycle

There are two life cycles that need to be supported.

#### 2.1.1 Firmware loaded during switch create
Depending on the functionality provided, a firmware may need to be loaded during the switch creation process itself.

A create only switch attribute SAI_SWITCH_ATTR_FW_LIST is added to specify the details of firmware during switch create time.

NOS will specify the list of firmwares in sai_fw_list_t data structure. Each element in a list specifies
- name of the firmware with fully the qualified path
- logfile name with the fully qualified path
- core id on which it is running. IDs are linear integer name space 0, 1,2 and so on
- administrative state the firmware

```
    /**
     * @brief Firmware list
     *
     * @type sai_fw_list_t
     * @flags CREATE_ONLY
     * @default internal
     */
    SAI_SWITCH_ATTR_FW_LIST,

    /**
     * @brief Firmware enabled on the switch
     *
     * @type sai_object_list_t
     * @flags READ_ONLY
     * @objects SAI_OBJECT_TYPE_FW
     */
    SAI_SWITCH_ATTR_FW,

    /**
     * @brief Maximum number of cores supported
     *
     * @type sai_uint8_t
     * @flags READ_ONLY
     */
    SAI_SWITCH_ATTR_MAX_FW_CORES,
```

New data type specifies the necessary information for loading and starting the firware.
```
/**
 * @brief SAI firmware administrative state
 */
typedef enum _sai_fw_admin_state_t
{
    /** Firmware admin state is automatic loading and running */
    SAI_FW_ADMIN_STATE_AUTO,

    /** Firmware admin state to start the firmware */
    SAI_FW_ADMIN_STATE_START_FW,

    /** Firmware admin state is stop the firmware */
    SAI_FW_ADMIN_STATE_STOP_FW,

    /** Firmware admin state is load the firmware */
    SAI_FW_ADMIN_STATE_LOAD_FW,

    /** Firmware admin state is unload the firmware */
    SAI_FW_ADMIN_STATE_UNLOAD_FW,
} sai_fw_admin_state_t;

/**
 * @brief Defines a firmware instance
 */
typedef struct _sai_fw_inst_t
{
    /** Firmware path */
    sai_s8_list_t fw_path_name;

    /** Firmware log file path */
    sai_s8_list_t log_path_name;

    /** Firmware core id */
    uint8_t  core_id;

    /** Firmware admin state */
    sai_fw_admin_state_t admin_state;
} sai_fw_inst_t;

/**
 * @brief Defines a list of firmware instances
 */
typedef struct _sai_fw_list_t
{
    /** Number of firmware instances */
    uint32_t count;

    /** List of firmware instances */
    sai_fw_inst_t *list;
} sai_fw_list_t;
```

Sequence of sets to load and start the firmware during switch create:

```text
Set SAI_SWITCH_ATTR_FW_LIST attribute
          |
          v
    [sai_s8_list_t fw_path_name]
    [sai_s8_list_t log_path_name]
    [uint8_t       core_id]
    [sai_fw_admin_state_t admin_state]
          |
          v
Set Other Switch Attributes
          |
          v
Create Switch Object
          |
          v
SDK Start Firmware based on configured admin state during switch create
          |
          v
+---------+---------+---------+---------+
|         |         |         |         |
v         v         v         v         v
Auto    Start      Stop      Load       UnLoad
 
```

> In this workflow SAI allocates the new instance of SAI_OBJECT_TYPE_FW object, based on the attribute SAI_SWITCH_ATTR_FW_LIST. 

#### 2.1.2 Runtime loading of firmware 
This is the case where system is up and running and a firmware need to be loaded on one of the controllers.

New object type SAI_OBJECT_TYPE_FW is introduced to manage the loading or unloading of firnware instances. NOS must create a SAI_OBJECT_TYPE_FW for each firmware. Based on the admin state configured SAI adapter will either just load the firmware or start it.

```
typedef enum _sai_fw_attr_t
{
    /**
     * @brief Start of Attributes
     */
    SAI_FW_ATTR_START,

    /**
     * @brief Firmware Log File and Path Name [char[SAI_FW_NAME_SIZE]]
     *
     * The maximum number of characters for the name is SAI_FW_NAME_SIZE - 1 since
     * it needs the terminating null byte ('\0') at the end.
     *
     * @type char
     * @flags CREATE_AND_SET
     * @default ""
     */
    SAI_FW_ATTR_LOG_FILE_AND_PATH_NAME = SAI_FW_ATTR_START,

    /**
     * @brief Firmware File and Path Name [char[SAI_FW_NAME_SIZE]]
     *
     * The maximum number of characters for the name is SAI_FW_NAME_SIZE - 1 since
     * it needs the terminating null byte ('\0') at the end.
     *
     * @type char
     * @flags CREATE_AND_SET
     * @default ""
     */
    SAI_FW_ATTR_FW_FILE_AND_PATH_NAME,

    /**
     * @brief Firmware to be loaded on the core
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_FW_ATTR_CORE_ID,

    /**
     * @brief Firmware admin state
     *
     * @type sai_fw_admin_state_t
     * @flags CREATE_AND_SET
     * @default SAI_FW_ADMIN_STATE_AUTO
     */
    SAI_FW_ATTR_FW_ADMIN_STATE,

    /**
     * @brief Firmware operational state
     *
     * @type sai_fw_op_state_t
     * @flags READ_ONLY
     */
    SAI_FW_ATTR_FW_OP_STATE,

    /**
     * @brief Firmware major version
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_FW_ATTR_FW_MAJOR_VERSION,

    /**
     * @brief Firmware minor version
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_FW_ATTR_FW_MINOR_VERSION,

    /**
     * @brief End of Performance Monitoring attributes
     */
    SAI_FW_ATTR_END,

    /** Custom range base value */
    SAI_FW_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_FW_ATTR_CUSTOM_RANGE_END

} sai_fw_attr_t;

```

### 2.2 Complete Life Cycle of Firmware

Following transition captures the simple linear life cycle of firmware object.


```text
Create Firmware
      |
      v
Firmware Loaded
      |
      v
Start Firmware
      |
      v
Running
      |
      v
Monitor State
      |
      v
Stop Firmware
      |
      v
Stopped
      |
      v
Remove Firmware
      |
      v
Completed
```
---

## 3.0  Firmware States

### 3.1 Administrative State

Followin administrative states are defined to manage the life cycle of the firmware. Admin state is specified in the SAI_FW_ATTR_FW_INST attribute which is of data type sai_fw_inst_t.

Administrative state controls firmware behavior.

| State                       | Description                         |
| --------------------------- | ----------------------------------- |
| SAI_FW_ADMIN_STATE_AUTO     | Automatically load and run firmware |
| SAI_FW_ADMIN_STATE_START_FW | Explicitly start firmware           |
| SAI_FW_ADMIN_STATE_STOP_FW  | Explicitly stop firmware            |
| SAI_FW_ADMIN_STATE_LOAD_FW  | Only load the firmware              |
| SAI_FW_ADMIN_STATE_UNLOAD_FW| Only unload the firmware.           |


---

### 3.2 Operational State

Once firmware is loaded, NOS need to know its operational state. Operational state is queried using the attribute SAI_FW_ATTR_FW_OP_STATE which is of the data type sai_fw_op_state_t.
Operational state indicates actual firmware status.

| State                      | Description                     |
| -------------------------- | ------------------------------- |
| SAI_FW_OP_STATE_LOADED     | Firmware loaded but not started |
| SAI_FW_OP_STATE_NOT_LOADED | Firmware unavailable and not loaded|
| SAI_FW_OP_STATE_RUNNING    | Firmware loaded and running     |
| SAI_FW_OP_STATE_STOPPED    | Firmware loaded but stopped     |
| SAI_FW_OP_STATE_ERROR      | Firmware loaded and encountered an error   |

```
/**
 * @brief SAI firmware operational state
 */
typedef enum _sai_fw_op_state_t
{
    /** Firmware operational state is firmware is loaded but not started */
    SAI_FW_OP_STATE_LOADED,

    /** Firmware operational state is firmware is not loaded */
    SAI_FW_OP_STATE_NOT_LOADED,

    /** Firmware operational state is running */
    SAI_FW_OP_STATE_RUNNING,

    /** Firmware operational state is stop */
    SAI_FW_OP_STATE_STOPPED,

    /** Firmware operational state is in error */
    SAI_FW_OP_STATE_ERROR,
} sai_fw_op_state_t;
```

---

## 4.0 Example Workflow

This section describes workflow for various scenarios.

### 4.1 Workflow for Cold Boot and Start two firmware during switch create
This is the case where firmware is loaded during switch create and switch attributes are specified.

#### 4.1.1 Configure the switch attribute for loading firmware

```
// Specify the TAM event type for packet drops
sai_attr_list[0].id = ﻿﻿SAI_SWITCH_ATTR_FW_LIST;
sai_attr_list[0].count = ﻿2;

sai_attr_list[0].list[0].value.fw_path_name = /mnt/fw/mon-fw.srec;
sai_attr_list[0].list[0].value.log_path_name = /mnt/log/fw/mon-fw.log;
sai_attr_list[0].list[0].value.core_id = 0;
sai_attr_list[0].list[0].value.admin_state = SAI_FW_ADMIN_STATE_AUTO;

sai_attr_list[0].list[1].value.fw_path_name = /mnt/fw/api-perf-fw.srec;;
sai_attr_list[0].list[1].value.log_path_name = /mnt/log/fw/api-perf-fw.log;
sai_attr_list[0].list[1].value.core_id = 1;
sai_attr_list[0].list[1].value.admin_state = SAI_FW_ADMIN_STATE_AUTO;
```

> SAI Adapter will create two oids of type SAI_OBJECT_TYPE_FW e.g. OID-FW-1 and OID-FW-2

#### 4.1.2 Read the configured FW objects in the system
NOS will do a get API call for attribute SAI_SWITCH_ATTR_FW.
This will return two OIDs as OID-FW-1 and OID-FW-2.

```
sai_attribute_t attr;

attr.id = SAI_SWITCH_ATTR_FW_LIST;

status = fw_api->get_switch_attribute(
            switchid,
            1,
            &attr);

Result:
attr.count = 2
attr.list[0] = OID-FW-1
attr.list[1] = OID-FW-2 
```

#### 4.1.3 Read the operational state of the firmware 
NOS will perform a GET for the operational state for each OID.

```
sai_attribute_t attr;

attr.id = SAI_FW_ATTR_FW_OP_STATE;

status = fw_api->get_fw_attribute(
            OID-FW-1,
            1,
            &attr);
Result:
SAI_FW_OP_STATE_RUNNING

status = fw_api->get_fw_attribute(
            OID-FW-2,
            1,
            &attr);
Result:
SAI_FW_OP_STATE_RUNNING
```
### 4.2 Workflow for Runtime configuration of two firmware
This is the case where firmware is loaded after the switch create is done. This workflow is applicable for frmware function that can be started after the switch create and init is done and have no dependency on switch init state.

#### 4.1.1 Create SAI_OBJECT_TYPE_FW objects for each firmware

```
// Specify the firmware log file name and path
sai_attr_list[0].id = ﻿﻿SAI_FW_ATTR_LOG_FILE_AND_PATH_NAME;
sai_attr_list[0].value.char = ﻿"/mnt/log/fw/mon-fw.log\0";

// Specify the firmware image file name and path
sai_attr_list[1].id = ﻿SAI_FW_ATTR_FW_FILE_AND_PATH_NAME;
sai_attr_list[1].value.char = ﻿"/mnt/log/fw/mon-fw.srec\0";

// Specify the administrative state of the firmware
sai_attr_list[2].id = ﻿SAI_FW_ATTR_FW_ADMIN_STATE;
sai_attr_list[2].value.s32 = SAI_FW_ADMIN_STATE_AUTO;

// Specify the core id
sai_attr_list[3].id = ﻿SAI_FW_ATTR_CORE_ID;
sai_attr_list[3].value.u8 = ﻿0;

// Create firmware object
attr_count = 4;
create_fw(
	&sai_mon_fw_obj,
	switch_id, 
	attr_count, 
	sai_attr_list);

// Create firmware object for 2nd firmware

// Specify the firmware log file name and path
sai_attr_list[0].id = ﻿﻿SAI_FW_ATTR_LOG_FILE_AND_PATH_NAME;
sai_attr_list[0].value.char = ﻿"/mnt/log/fw/api-perf-fw.log\0";

// Specify the firmware image file name and path
sai_attr_list[1].id = ﻿SAI_FW_ATTR_FW_FILE_AND_PATH_NAME;
sai_attr_list[1].value.char = ﻿"/mnt/log/fw/api-perf-fw.srec\0";

// Specify the administrative state of the firmware
sai_attr_list[2].id = ﻿SAI_FW_ATTR_FW_ADMIN_STATE;
sai_attr_list[2].value.s32 = SAI_FW_ADMIN_STATE_AUTO;

// Specify the core id
sai_attr_list[3].id = ﻿SAI_FW_ATTR_CORE_ID;
sai_attr_list[3].value.u8 = ﻿0;

// Create firmware object
attr_count = 4;
create_fw(
	&sai_api_perf_fw_obj,
	switch_id, 
	attr_count, 
	sai_attr_list);
```
#### 4.2.2 Read the operational state and the version of the firmware 
NOS will perform a GET for the operational state for each OID.

```
sai_attribute_t attr;

attr[0].id = SAI_FW_ATTR_FW_OP_STATE;
attr[1].id = SAI_FW_ATTR_FW_MAJOR_VERSION;
attr[2].id = SAI_FW_ATTR_FW_MINOR_VERSION;

status = fw_api->get_fw_attribute(
            sai_mon_fw_obj,
            3,
            &attr);
Result:
SAI_FW_OP_STATE_RUNNING
1
12

status = fw_api->get_fw_attribute(
            sai_mon,
            3,
            &attr);
Result:
SAI_FW_OP_STATE_RUNNING
1
18
```

### 4.3 Workflow for Warmboot transition from no firmware to two firmwares
This is the case where new set of firmware need to be installed after the warmboot.
If the firmware is such that it can be installed after the switch create then no new workflow is required as it will be considered a post warmboot configuration change. But if the requirement is to load the firmware immediately after warmboot then it must be configured as part of swtich create.

This will follow the same steps as in section 4.1

### 4.4 Workflow for Warmboot transition from one firmware to two firmwares
This is the case where another firmware is added after the warmboot.
This is same as section 4.3 where NOS need to retain the previously configured firmware and add another one.

> Section 4.3 and 4.4 will create new OID post warmboot and any implications of this need to be handled by orchagent.

### 4.4 Workflow for Warmboot transition from two firmwares of ver1 to two firmwares of ver2
This is a simpler case where there are no new OIDs are created post warmboot but only the attributes are changed from ver 1 to ver 2 post warmboot.


### 4.5 Workflow for Create and Auto-Start Firmware

#### 4.5.1 Sequence Diagram

```text
Application
    |
    | create_fw()
    v
SAI Layer
    |
    | Validate Attributes
    v
Vendor SDK
    |
    | Load Firmware
    v
Hardware
    |
    | Start Firmware
    v
Running State
```

#### 4.5.2 Example

```c
sai_attribute_t attrs[2];

... other attributes are set ...

attrs[1].id = SAI_FW_ATTR_FW_ADMIN_STATE;
attrs[1].value.s32 = SAI_FW_ADMIN_STATE_AUTO;

sai_object_id_t fw_id;

status = fw_api->create_fw(
            &fw_id,
            switch_id,
            2,
            attrs);
```
#### 4.5.3 Expected Result

```text
Firmware Object Created
Firmware Loaded
Firmware Running
Operational State = RUNNING
```

---

### 4.6 Workflow to manage Firmware lifecycle manually

#### 4.6.1 Create Firmware with admin state as LOAD

This will only load the firmware. If there is a previously loaded firmware and is not unloaded, this API sequence should return error.

```c
attrs[1].id = SAI_FW_ATTR_FW_ADMIN_STATE;
attrs[1].value.s32 = SAI_FW_ADMIN_STATE_LOAD_FW;

fw_api->set_fw_attribute(
    fw_id,
    &attr);
```

#### 4.6.1.1 Result

```text
Firmware Loaded
Operational State = LOADED
```

---

#### 4.6.2 Workflow to Start Firmware
This will start running the loaded firmware

```c
sai_attribute_t attr;

attr.id = SAI_FW_ATTR_FW_ADMIN_STATE;
attr.value.s32 = SAI_FW_ADMIN_STATE_START_FW;

fw_api->set_fw_attribute(
    fw_id,
    &attr);
```

#### 4.6.2.1 Result

```text
Firmware Running
Operational State = RUNNING
```

---

### 4.6.3  Workflow to Stop Firmware
This will stop the running firmware but not unload it.

```c
sai_attribute_t attr;

attr.id = SAI_FW_ATTR_FW_ADMIN_STATE;
attr.value.s32 = SAI_FW_ADMIN_STATE_STOP_FW;

fw_api->set_fw_attribute(
    fw_id,
    &attr);
```
#### 4.6.3.1 Result

```text
Firmware Running
Operational State = STOPPED
```
---
#### 4.6.4 Create Firmware with admin state as LOAD

This will unload the firmware. If there is a no previously loaded firmware, this API sequence should return status.

```c
attrs[1].id = SAI_FW_ATTR_FW_ADMIN_STATE;
attrs[1].value.s32 = SAI_FW_ADMIN_STATE_UNLOAD_FW;

fw_api->set_fw_attribute(
    fw_id,
    &attr);
```

#### 4.6.4.1 Result

```text
Firmware UnLoaded
Operational State = UNLOADED
```

---

### 4.6.5 Workflow to Query Firmware Status


```c
sai_attribute_t attr;

attr.id = SAI_FW_ATTR_FW_OP_STATE;

status = fw_api->get_fw_attribute(
            fw_id,
            1,
            &attr);
```

#### 4.6.5.1 Result

```c
switch(attr.value.s32)
{
    case SAI_FW_OP_STATE_LOADED:
        printf("Firmware Loaded\n");
        break;
    case SAI_FW_OP_STATE_UNLOADED:
        printf("Firmware Unloaded\n");
        break;
    case SAI_FW_OP_STATE_RUNNING:
        printf("Firmware Running\n");
        break;

    case SAI_FW_OP_STATE_STOPPED:
        printf("Firmware Stopped\n");
        break;

    case SAI_FW_OP_STATE_ERROR:
        printf("Firmware Error\n");
        break;
}
```

