# PFC Deadlock Detection/Recovery State

Title       | PFC Deadlock
------------|----------------
Authors     | Cisco
Status      | In review
Type        | Standards track
Created     | 08/5/2021
SAI-Version | 1.8

## Overview
Priority Flow Control (PFC) is used in Ethernet networks which makes the network prone to deadlocks. Detection and recovery of deadlocks can differ across switch vendors due to the ASIC implementation. SAI provides many attributes that can be queried to determine when a queue is in a deadlock state. From the Network Operating System (NOS), you need to know the ASIC vendor specific implementation to determine which attributes need to be queried to determine if the queue is deadlocked or has recovered from the deadlock. Adding a single new attribute that can be used to query for PFC deadlock/recovery will eliminate the need for a NOS to know which attributes to query.

SAI also provides for PFC deadlock detection/recovery to be implemented in the ASIC SDK where the polling for deadlock or recovery is done in the SAI or SDK layers. To allow for the PFC polling intervals to be different across ports, port level SAI attributes are provided to set the polling interval for PFC deadlock detection and recovery along with port level attributes to query the polling interface range across queues.

## Proposal

### New PFC deadlock queue attributes:
```
typedef enum _sai_queue_pfc_continuous_deadlock_state_t
{
    /**
     * @brief PFC continuous deadlock state not paused.
     *
     * H/w queue PFC state is not paused.
     * Queue can forward packets.
     */
    SAI_QUEUE_PFC_CONTINUOUS_DEADLOCK_STATE_NOT_PAUSED = 0x00000000,

    /**
     * @brief PFC continuous deadlock state paused.
     *
     * H/w queue is paused off and has not resumed
     * forwarding packets since the last time the
     * SAI_QUEUE_ATTR_PFC_CONTINUOUS_DEADLOCK_STATE
     * attribute for this queue was polled.
     */
    SAI_QUEUE_PFC_CONTINUOUS_DEADLOCK_STATE_PAUSED = 0x00000001,

    /**
     * @brief PFC continuous deadlock state paused, but not continuously.
     *
     * H/w queue is paused off, but was not paused
     * off for the full interval that the
     * SAI_QUEUE_ATTR_PFC_CONTINUOUS_DEADLOCK_STATE
     * attribute for this queue was last polled.
     */
    SAI_QUEUE_PFC_CONTINUOUS_DEADLOCK_STATE_PAUSED_NOT_CONTINUOUS = 0x00000002,

} sai_queue_pfc_continuous_deadlock_state_t;

    /**
     * @brief Control for buffered and incoming packets on a queue undergoing PFC Deadlock Recovery.
     *
     * This control applies to all packets on the queue.
     *
     * @type sai_packet_action_t
     * @flags CREATE_AND_SET
     * @default SAI_PACKET_ACTION_DROP
     */
    SAI_QUEUE_ATTR_PFC_DLR_PACKET_ACTION,

    /**
     * @brief Queue PFC continuous deadlock state
     *
     * This attribute represents the queue's internal hardware PFC
     * continuous deadlock state. It is an aggregation of all HW state used
     * to determine if a queue is in PFC deadlock based on state
     * cached/maintained by the SDK. Consecutive queries of this
     * attribute provide the PFC state for the queue for the interval
     * period between the queries.
     *
     * This attribute should only be queried as part of the PFC deadlock
     * and recovery detection processing.
     *
     * @type sai_queue_pfc_continuous_deadlock_state_t
     * @flags READ_ONLY
     */
    SAI_QUEUE_ATTR_PFC_CONTINUOUS_DEADLOCK_STATE,
```
### New PFC deadlock port attributes:
```
    /**
     * @brief  PFC Deadlock Detection timer interval range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PFC_TC_DLD_INTERVAL_RANGE,

    /**
     * @brief PFC Deadlock Detection timer interval in milliseconds.
     *
     * If the monitored queue is in XOFF state for more than this duration then
     * its considered to be in a PFC deadlock state and recovery process is kicked off.
     * Note: Use TC (Traffic Class) value as key and timer interval as value.
     *
     * @type sai_map_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_ATTR_PFC_TC_DLD_INTERVAL,

    /**
     * @brief  PFC Deadlock Recovery timer interval range
     *
     * @type sai_u32_range_t
     * @flags READ_ONLY
     */
    SAI_PORT_ATTR_PFC_TC_DLR_INTERVAL_RANGE,

    /**
     * @brief PFC Deadlock Recovery timer interval in milliseconds.
     *
     * The PFC deadlock recovery process will run for this amount of time and then normal
     * state will resume. If the system remains in a deadlock state then the detection and
     * recovery will resume again after the configured detection timer interval.
     * Note: Use TC (Traffic Class) value as key and timer interval as value.
     *
     * @type sai_map_list_t
     * @flags CREATE_AND_SET
     * @default empty
     */
    SAI_PORT_ATTR_PFC_TC_DLR_INTERVAL,
```
## Usage
###NOS polling for PFC deadlock and recovery
```
/********************************************************************
 * For each port/queue, poll SDK via SAI for PFC deadlock detection
 *********************************************************************/
for (auto port = ports.begin(); port != ports.end(); port++) {
    port_p = port->second;
    for (auto queue = port_p->queues.begin(); queue != port_p->queues.end(); queue++) {
        queue_p = queue->second;

        // Get PFC deadlock state from SDK
        sai_attribute_t attr;
        attr.id = SAI_QUEUE_ATTR_PFC_CONTINUOUS_DEADLOCK_STATE;
        status = sai_queue_api->get_queue_attribute(queue_p->oid, 1, attr);
        if (status != SAI_STATUS_SUCCESS) {
            LOG_ERROR("Failed to get PFC DEADLOCK State for queue 0x%" PRIx64 ": %d", queue_p->oid, status);
            return;
        }
        sai_queue_pfc_state_t pfc_deadlock_state = attr.value.s32;

        if (queue_p->pfc_deadlock_state == DETECTION) {
            // If queue is in the PFC deadlock DETECTION state, check if
            // deadlocked and if time interval for detection has
            // expired, instruct SDK to initiate recovery.
            if (pfc_deadlock_state == SAI_QUEUE_PFC_CONTINUOUS_DEADLOCK_STATE_PAUSED) {
                if (queue_p->pfc_deadlock_time_left <= poll_period) {
                    queue_p->pfc_storm_detected();
                    queue_p->pfc_deadlock_time_left = queue_p->pfc_deadlock_recovery_intv_time;
                    queue_p->pfc_deadlock_state = RECOVERY;
                } else {
                    queue_p->pfc_deadlock_time_left -= poll_period;
                }
            } else {
                // Queue is in PFC deadlock DETECTION state, but queue is not stuck
                // or was not stuck the the whole time during the polling interval.
                // Reset detection interval time.
                queue_p->pfc_deadlock_time_left = queue_p->pfc_deadlock_detect_intv_time;
            }
        } else if (queue_p->pfc_deadlock_state == RECOVERY) {
            // If queue is in the PFC deadlock RECOVERY state, check if
            // not deadlocked and if time interval for recovery has
            // expired, instruct SDK to restore queue to forwarding again.
            if (pfc_deadlocked == SAI_QUEUE_PFC_CONTINUOUS_DEADLOCK_STATE_NOT_PAUSED) {
                if (queue_p->pfc_deadlock_time_left <= poll_period) {
                    queue_p->pfc_storm_recovered();
                    queue_p->pfc_deadlock_time_left = queue_p->pfc_deadlock_detect_intv_time;
                    queue_p->pfc_deadlock_state = DETECTION;
                } else {
                    queue_p=>pfc_deadlock_time_left -= poll_period;
                }
            } else {
                // Queue is in PFC deadlock RECOVERY state, but queue is still stuck.
                // Reset recovery interval time.
                queue_p->pfc_deadlock_time_left = queue_p->pfc_deadlock_recovery_intv_time;
            }
        }
    }
}

void
QueueType::pfc_storm_detected()
{
    // Instruct SDK to initiate PFC deadlock recovery for queue.
    sai_attribute_t attr;
    attr.id = SAI_QUEUE_ATTR_PFC_DLR_INIT;
    attr.value.bool = true;
    status = sai_queue_api->set_queue_attribute(oid, 1, attr);
    if (status != SAI_STATUS_SUCCESS) {
        LOG_ERROR("Failed to set PFC DLR INIT for queue 0x%" PRIx64 ": %d", oid, status);
        return;
    }
}

void
QueueType::pfc_storm_recovered()
{
    // Instruct SDK to restore queue from PFC deadlock state
    sai_attribute_t attr;
    attr.id = SAI_QUEUE_ATTR_PFC_DLR_INIT;
    attr.value.bool = false;
    status = sai_queue_api->set_queue_attribute(oid, 1, attr);
    if (status != SAI_STATUS_SUCCESS) {
        LOG_ERROR("Failed to set PFC DLR INIT for queue 0x%" PRIx64 ": %d", oid, status);
        return;
    }
}
```
