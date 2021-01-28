SAI Flex State Complex Data Type
-------------------------------------------------------------------------------
 Title       | SAI Flex State: Complex Data Type
-------------|-----------------------------------------------------------------
 Authors     | Jai Kumar, Broadcom Inc.
 Status      | In review
 Type        | Standards track
 Created     | 28/01/2021: Initial Draft
 SAI-Version | TBD
-------------------------------------------------------------------------------

This spec talks about a new complex data type.
Flex data type is introduced to handle I/O operations which are atomic in nature and/or involves multiple field updates simultaneously. 

# Flex State IO Operation
#### Usecase 1:
There are many “clear on read” states in hardware. Sometimes these are individual standalone fields. Many a times HW combines multiple fields in a single atomic read. 
Given that SAI API attributes are defined at sub field granularity, SAI adapter has to emulate “clear on read” for sub field queries. Flex state complex data type allows read operation on the combination of such attributes. This makes SAI adapter implementation easy by just making a single query to hardware. Hardware natively takes care of clear on read for all the subfields with in the field.

#### Usecase 2:
Some hardware allows reading of multiple physically separate fields as a single query using bilk PIO or DMA commands.
Flex state complex data type allows SAI to create an unconstrained list of fields for atomic read.

#### Usecase 3:
Flex state data type can also be used to perform an atomic Write in HW.

# Flex State Complex Data Type
---
Flex state Data type is complex data type and is extensible. Features can add more data fields in the flex state data type as needed.
```
typedef struct _sai_flex_state_t
{
    sai_flex_data_type_t type;

    /** @passparam type */
    sai_flex_status_t status;

    /** @passparam type */
    sai_flex_data_t data;
} sai_flex_state_t;

/**
 * @extraparam sai_flex_data_type_t type
 */
typedef union _sai_flex_status_t
{
    /** @validonly type == SAI_FLEX_DATA_TYPE_PRBS */
    sai_port_prbs_rx_status_t rx_status;

} sai_flex_status_t;

/**
 * @extraparam sai_flex_data_type_t type
 */
typedef union _sai_flex_data_t
{
    /** @validonly type == SAI_FLEX_DATA_TYPE_PRBS */
    uint32_t count;

} sai_flex_data_t;

```

# Workflow Example
SAI Port query for PRBS RX status.
```
sai_flex_state_t prbs_state;
sai_attribute_t attr;

attr.id = SAI_PORT_ATTR_PRBS_RX_STATE;
attr.value = &prbs_state

sai_rc = sai_get_port_attribute(port1_oid, &attr)

This API will return prbs_state data type populated as follows for lock with errors

prbs_state.type -> SAI_FLEX_DATA_TYPE_PRBS
prbs_state.status.rx_status -> SAI_PRBS_RX_STATUS_LOCK_WITH_ERRORS
prbs_state.data.count -> 20
```

