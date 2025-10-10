# [SAI] PRBS Per Lane Enhancements

-------------------------------------------------------------------------------

 Title       | PRBS Per Lane Enhancements
-------------|-----------------------------------------------------------------
 Authors     | Spandan Dasgupta, Chris Nitin Adonis Petrus, Rajkumar P R, Ravindranath C K (Marvell)
 Status      | In review
 Type        | Standards track
 Created     | 2025-09-10
 SAI-Version | 1.17

-------------------------------------------------------------------------------

## 1.0  Introduction

PRBS (Pseudo-Random Bit Sequence) is used to test the integrity of high-speed serial links by generating and checking random data patterns. It helps validate serdes performance and physical connectivity.

SAI currently provides the following port attributes to fetch PRBS related status.

```c
    /**
     * @brief Attribute data for #SAI_PORT_ATTR_PRBS_LOCK_STATUS
     *
     * PRBS lock status: 1 for locked, 0 for unlocked
     *
     * @type bool
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PRBS_LOCK_STATUS,

    /**
     * @brief Attribute data for #SAI_PORT_ATTR_PRBS_RX_STATUS
     *
     * @type sai_port_prbs_rx_status_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PRBS_RX_STATUS,

    /**
     * @brief Attribute data for #SAI_PORT_ATTR_PRBS_RX_STATE
     * Used for clear on read status/count register.
     * Adapter should return SAI_STATUS_NOT_SUPPORTED if not supported.
     *
     * @type sai_prbs_rx_state_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PRBS_RX_STATE,
```

In addition, SAI also provides **SAI_PORT_STAT_PRBS_ERROR_COUNT** as a port stat to fetch error count for PRBS.

```c
   /** PRBS Error Count */
   SAI_PORT_STAT_PRBS_ERROR_COUNT,
```

## 2.0  Problem Statement

Link quality and error characteristics can vary significantly between lanes due to manufacturing variations, signal integrity issues, or physical connectivity problems. Existing SAI PRBS (Pseudo-Random Bit Sequence) support provides only port-level statistics, which can mask lane-specific issues and hinder effective diagnostics.

To address this limitation, there is a need for per-lane PRBS status and error reporting. By exposing PRBS lock status, receive state, and error counters for each individual lane, operators and diagnostic tools can more accurately pinpoint the source of link degradation and perform targeted corrective actions.

However, simply reporting raw error counts per lane is insufficient for comprehensive link assessment. Error counts are affected by test duration and link speed, making it difficult to compare results across links or over time. To provide a normalized, meaningful metric, Bit Error Rate (BER) is commonly used. 

In practice, BER values are extremely small  and are typically represented as floating-point numbers. However, SAI does not support floating-point data types. To enable accurate and portable BER representation, a new datatype is introduced.


## 3.0 Proposed SAI Enhancement

1) New structure for a Lane specific PRBS Rx state and status and list of each:

   ```c

    typedef struct _sai_prbs_per_lane_rx_status_t
    {
        uint32_t lane;
        sai_port_prbs_rx_status_t rx_status;
    } sai_prbs_per_lane_rx_status_t;

   /**
    * @brief Defines PRBS Rx stateus for list of all serdes lanes
    */
    typedef struct _sai_prbs_per_lane_rx_status_list_t
    {
        uint32_t count;
        sai_prbs_per_lane_rx_status_t *list;
    } sai_prbs_per_lane_rx_status_list_t;

    typedef struct _sai_prbs_per_lane_rx_state_t
    {
        uint32_t lane;
        sai_prbs_rx_state_t rx_state;
    } sai_prbs_per_lane_rx_state_t;

    /**
     * @brief Defines PRBS Rx states for list of all serdes lanes
     */
    typedef struct _sai_prbs_per_lane_rx_state_list_t
    {
        uint32_t count;
        sai_prbs_per_lane_rx_state_t *list;
    } sai_prbs_per_lane_rx_state_list_t;


   ```

   Also, include the new list to **sai_attribute_value_t** union.

   ```c
   ...Existing union members
      /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_PRBS_PER_LANE_RX_STATUS_LIST */
      sai_prbs_per_lane_rx_status_list_t prbs_rx_status_list;
      /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_PRBS_PER_LANE_RX_STATE_LIST */
      sai_prbs_per_lane_rx_state_list_t prbs_rx_state_list;
   } sai_attribute_value_t;

   ```

2) New port attributes for per lane alternative of existing attributes **SAI_PORT_ATTR_PRBS_LOCK_STATUS**, **SAI_PORT_ATTR_PRBS_RX_STATUS** and **SAI_PORT_ATTR_PRBS_RX_STATE**:

   ```c
   /**
     * @brief Per Lane PRBS Lock Status
     *
     * Per lane list of lock status for PRBS.
     * The values are of type sai_port_lane_latch_status_list where the count is the number of lanes in
     * a port and the list specifies list of lane id and lock status for each lane
     * Lock status will have both lock status and changed status.
     *
     * @type sai_port_lane_latch_status_list
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PRBS_PER_LANE_LOCK_STATUS_LIST,

    /**
     * @brief Per Lane PRBS Rx Status
     *
     * Per lane list of Rx status for PRBS.
     * The values are of type sai_prbs_per_lane_rx_status_list_t where the count is the number of lanes in
     * a port and the list specifies list of values of type sai_port_prbs_rx_status_t and the lane id
     * for each lane.
     *
     * @type sai_prbs_per_lane_rx_status_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PRBS_PER_LANE_RX_STATUS_LIST,

    /**
     * @brief Per Lane PRBS Rx State
     *
     * Per lane list of Rx state for PRBS.
     * The values are of type sai_prbs_per_lane_rx_state_list_t where the count is the number
     * of lanes in a port and the list specifies list of values of type sai_prbs_rx_state_t
     * for each lane and its lane id.
     * Used for clear on read status/count register.
     * Adapter should return SAI_STATUS_NOT_SUPPORTED if not supported.
     *
     * @type sai_prbs_per_lane_rx_state_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PRBS_PER_LANE_RX_STATE_LIST,
   ```

3) New port stats for PRBS error count per lane for relative lanes 0 to 15:

   For this we are reserving 256 values for future lane expansions
   ```c

    /** Per Lane PRBS Error Count Range Start */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_RANGE_BASE = 0x00004000,

    /** Per Lane PRBS Error Count For lane in index 0 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_0 = SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_RANGE_BASE,

    /** Per Lane PRBS Error Count For lane in index 1 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_1,

    /** Per Lane PRBS Error Count For lane in index 2 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_2,

    /** Per Lane PRBS Error Count For lane in index 3 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_3,

    /** Per Lane PRBS Error Count For lane in index 4 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_4,

    /** Per Lane PRBS Error Count For lane in index 5 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_5,

    /** Per Lane PRBS Error Count For lane in index 6 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_6,

    /** Per Lane PRBS Error Count For lane in index 7 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_7,

    /** Per Lane PRBS Error Count For lane in index 8 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_8,

    /** Per Lane PRBS Error Count For lane in index 9 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_9,

    /** Per Lane PRBS Error Count For lane in index 10 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_10,

    /** Per Lane PRBS Error Count For lane in index 11 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_11,

    /** Per Lane PRBS Error Count For lane in index 12 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_12,

    /** Per Lane PRBS Error Count For lane in index 13 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_13,

    /** Per Lane PRBS Error Count For lane in index 14 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_14,

    /** Per Lane PRBS Error Count For lane in index 15 */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_15,

    /** Per Lane PRBS Error Count Range END */
    SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_RANGE_END = 0x000040ff,
   ```

4) New datatype to represent PRBS Bit Error Rate (BER):

   Bit error rate (BER) offers a more precise evaluation of PRBS than error counts, as it normalizes errors against the total number of transmitted bits. This enables consistent and meaningful assessment of link quality across varying data rates and test durations.

   BER is typically a floating point number. SAI does not support floating point data types. Hence, introduce a new datatype to represent the exponent and mantissa information.

   ```c
   /* 
    *Represents BER as: mantissa * 10^(-exponent)
    */
   typedef struct _sai_prbs_bit_error_rate_t
   {
      uint8_t  exponent; /* Negative exponent as in 10^-exponent */
      uint64_t mantissa; /* Significant digits of the BER, to be multiplied by 10^(-exponent) */
   } sai_prbs_bit_error_rate_t;
   ```

   Consider an example BER of 1.83258 x 10^-14. **mantissa** can be 183528 and **exponent** can be 19, i.e. **mantissa x 10^exponent**. The **mantissa** can contain at max 19 digits as 10^21 > MAX_UINT64 > 10^20.

   Also, introduce a per lane version and its list for the above datatype.

   ```c
    typedef struct _sai_prbs_per_lane_bit_error_rate_t
    {
        uint32_t lane;
        sai_prbs_bit_error_rate_t ber;
    } sai_prbs_per_lane_bit_error_rate_t;

    typedef struct _sai_prbs_per_lane_bit_error_rate_list_t
    {
        uint32_t count;
        sai_prbs_per_lane_bit_error_rate_t *list;
    } sai_prbs_per_lane_bit_error_rate_list_t;

   ```

   Add both the above types to **sai_attribute_value_t** union.

   ```c
   ...Existing union members
      /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_PRBS_BIT_ERROR_RATE */
      sai_prbs_bit_error_rate_t      prbs_ber;

      /** @validonly meta->attrvaluetype == SAI_ATTR_VALUE_TYPE_PRBS_PER_LANE_BIT_ERROR_RATE_LIST */
      sai_prbs_per_lane_bit_error_rate_list_t prbs_ber_list;
   } sai_attribute_value_t;

   ```

5) New port attributes for PRBS Bit Error Rate (BER):

   ```c

    /**
     * @brief Per Lane PRBS Bit Error Rate (BER)
     *
     * Per lane list of PRBS Bit Error Rate (BER).
     * The values are of type sai_prbs_per_lane_bit_error_rate_list_t where the count is the number
     * of lanes in a port and the list specifies list of values of type sai_prbs_bit_error_rate_t
     * and lane id for each lane.
     * BER will be (error count/bits transmitted) = BER.mantissa * (10^-BER.exponent)
     *
     * @type sai_prbs_per_lane_bit_error_rate_list_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PRBS_PER_LANE_BER_LIST,
   ```

## 4.0 API Example

Let us take an example port **prbs_port_oid** with 8 lanes.

### Get PRBS per lane lock status, lock loss status, rx status, rx state and bit error rate

```c
sai_attr_list[attr_count].id = SAI_PORT_ATTR_PRBS_PER_LANE_LOCK_STATUS_LIST;
sai_attr_list[attr_count].value.portlanelatchstatuslist.count = 8;
sai_attr_list[attr_count].value.portlanelatchstatuslist.list = malloc(sizeof(sai_port_lane_latch_status_t)*sai_attr_list[attr_count++].value.portlanelatchstatuslist.count);

sai_attr_list[attr_count].id = SAI_PORT_ATTR_PRBS_PER_LANE_RX_STATUS_LIST;
sai_attr_list[attr_count].value.prbs_rx_status_list.count = 8;
sai_attr_list[attr_count].value.prbs_rx_status_list.list = malloc(sizeof(sai_prbs_per_lane_rx_status_t)*sai_attr_list[attr_count++].value.prbs_rx_status_list.count);

sai_attr_list[attr_count].id = SAI_PORT_ATTR_PRBS_PER_LANE_RX_STATE_LIST;
sai_attr_list[attr_count].value.prbs_rx_state_list.count = 8;
sai_attr_list[attr_count].value.prbs_rx_state_list.list = malloc(sizeof(sai_prbs_per_lane_rx_state_t)*sai_attr_list[attr_count++].value.prbs_rx_state_list.count);

sai_attr_list[attr_count].id = SAI_PORT_ATTR_PRBS_PER_LANE_BER_LIST;
sai_attr_list[attr_count].value.prbs_ber_list.count = 8;
sai_attr_list[attr_count].value.prbs_ber_list.list = malloc(sizeof(sai_prbs_per_lane_bit_error_rate_t)*sai_attr_list[attr_count++].value.prbs_ber_list.count);

sai_get_port_attribute_fn(
   prbs_port_oid,
   attr_count,
   sai_attr_list);

```

### Get PRBS per lane error count stat

```c
counter_id_list[num_counters++].id = SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_0;
counter_id_list[num_counters++].id = SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_1;
counter_id_list[num_counters++].id = SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_2;
counter_id_list[num_counters++].id = SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_3;
counter_id_list[num_counters++].id = SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_4;
counter_id_list[num_counters++].id = SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_5;
counter_id_list[num_counters++].id = SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_6;
counter_id_list[num_counters++].id = SAI_PORT_STAT_PRBS_ERROR_COUNT_LANE_7;

sai_get_port_stats_fn(
   prbs_port_oid,
   num_counters,
   counter_id_list,
   &counters);
```

