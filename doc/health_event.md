# Switch health events 

Health event is a way for SAI adapter to inform NOS about HW/SW health issues. When some error occurs on the switch, SAI should in some way inform the NOS about the status. In order to provide flexibility, depending on different types, SAI generates health event with several parameters that describes that event. Hence, to inform NOS - sai_switch_asic_sdk_health_event_notification_fn callback is invoked. It takes 6 parameters:

 1. _In_ sai_object_id_t switch_id - Switch object identifier
 2. _In_ sai_switch_asic_sdk_health_severity_t severity - severity of an issue (fatal, warning, notice)
 3. _In_ sai_timespec_t timestamp - time when issue occurred
 4. _In_ sai_switch_asic_sdk_health_category_t category - category of health issue
 5. _In_ sai_switch_health_data_t data - specific data for that event
 6. _In_ const sai_u8_list_t description - JSON-encoded description string with information delivered from SDK event/trap

 Example of possible descritption:
 {
    "switch_id": "0x00000000000000AB”,
    "severity": “2”,
    "timestamp” : {
        “tv_sec“ : “22429”,
        “tv_nsec” : “3428724”
    },
    "category": "3",
    “data : {
        data_type : “0”
    },
    "additional_data": "Some additional information"
 }


These fields provide the ability to add as much information as needed about the event.

For health severity and health category there are two respective enums: sai_switch_asic_sdk_health_severity_t and sai_switch_asic_sdk_health_category_t.

For data parameter we have struct that contains two fields: type of the data and data itself. New type of data can be added if needed.

## Event registration
NOS provides an event callback to SAI adapter, through SAI_SWITCH_ATTR_SWITCH_ASIC_SDK_HEALTH_EVENT_NOTIFY

NOS can choose which categories to register to, per each severity
For example, to register for SW and FW categories, NOS will set
SAI_SWITCH_ATTR_REG_WARNING_SWITCH_ASIC_SDK_HEALTH_CATEGORY
as s32list.count = 2, s32list.list = {SAI_SWITCH_ASIC_SDK_HEALTH_CATEGORY_SW, SAI_SWITCH_ASIC_SDK_HEALTH_CATEGORY_FW}
The default is empty category list per severity

## Extending the health data
In order to add new information in the data field, several steps should be completed:

1) Data type should be added to enum sai_health_data_type_t

2) For all types that have additional data, a struct should be created. The first time a new type that has actual struct data will be created, union should be created as well. Each struct should be added to that union.

For example if we want to add a SER item (https://github.com/opencomputeproject/SAI/pull/1307), the flow will be :

```
typedef enum _sai_health_data_type_t
{
    /** General health data type */
    SAI_HEALTH_DATA_TYPE_GENERAL,

    /** SER health data type */
    SAI_HEALTH_DATA_TYPE_SER

} sai_health_data_type_t;

typedef struct _sai_ser_health_data_t
{
    ...
    /* SER specific fields */

} sai_ser_health_data_t;

/**
* @extraparam sai_health_data_type_t data_type
*/
typedef union _sai_health_data_t
{
    /** @validonly data_type == SAI_HEALTH_DATA_TYPE_SER */
    sai_ser_health_data_t ser;
} sai_health_data_t;

typedef struct _sai_health_t
{
    /** Type of health data */
    sai_health_data_type_t data_type;

    /** @passparam data_type */
    sai_health_data_t data;
} sai_health_t;
```

