# Switch Abstraction Interface Change Proposal for Packet Trimming

Title       | Packet Trimming
------------|----------------
Authors     | Nvidia
Status      | In review
Type        | Standards track
Created     | 8/28/2024
SAI-Version | 1.14
----------

## Overview
When the lossy queue exceeds a buffer threshold, it drops packets without any notification to the destination host.

When a packet is lost, it can be recovered through fast retransmission (e.g., Go-Back-N in RoCE) or by using timeouts. Retransmission triggered by timeouts typically incurs significant latency. Packet trimming aims to facilitate rapid packet loss notification and, consequently, eliminate slow timeout-based retransmissions.

To help the host recover data more quickly and accurately, we introduce a packet trimming feature, that upon a failed packet admission to a shared buffer,
will trim a packet to a configured size, and try sending it on a different queue to deliver a packet drop notification to an end host.

```
                                                                                                                                        
                                                                                       ┌───────────────┐                                
                                                                                       │               │                                
                                                                                       │Trimmed packet │                                
                                                                                       │               │                                
                                                                                       └───────────────┘                                
                                                                                                                                        
                                                                                                    ┌─┬─┬─┬─┬────────┐                  
                                                                                                    │ │ │ │ │        │                  
                                                                                                    │ │ │ │ │        │                  
                                                                                   ┌────────────────► │ │ │ │        │                  
                                                                                   │                │ │ │ │ │        │    Queue         
                                                                                   │                │ │ │ │ │        │                  
                                                                                   │                │ │ │ │ │        │                  
                                                                                   │                └─┴─┴─┴─┴────────┘                  
     ┌──────────────┐                                                              │                                                    
     │              │  ┌──────────────────────────────────────────────────────┐    │                ┌─┬─┬─┬─┬─┬─┬─┬─┬┐                  
     │              │  │                                                      │    │                │ │ │ │ │ │ │ │ ││                  
     │              │  │                                                      │    │     \   /      │ │ │ │ │ │ │ │ ││                  
     │              │  │                                                      │    │      \ /       │ │ │ │ │ │ │ │ ││                  
     │    Packet    │  │           Pipeilne                                   ┼────┼───────\────────► │ │ │ │ │ │ │ ││     Queue        
     │              │  │                                                      │           / \       │ │ │ │ │ │ │ │ ││                  
     │              │  │                                                      │          /   \      │ │ │ │ │ │ │ │ ││                  
     │              │  └──────────────────────────────────────────────────────┘                     └─┴─┴─┴─┴─┴─┴─┴─┴┘                  
     │              │                                                                                                                   
     │              │                                                                                                                   
     │              │                                                                                                                   
     └──────────────┘                                                                                                                   
```

This feature assumes that forwarding tables are configured properly, and the original packet would be delivered to the destination successfully if not for the congestion.

## Spec
There is a tradeoff between trying to configure a higher threshold in a queue buffer profile and trimming the packet.

If the user chooses to configure higher thresholds for queues, the probability of a drop on a particular queue is lower only if other ports are less congested at the moment.

However, if all the ports are equally utilized, it makes sense to create a different buffer profile for these queues, with a stricter threshold to have more fairness in shared buffer.

A static trimming threshold may not be effective with shared buffer switches, where the buffer resources allocated to a queue or port can vary over time. Therefore, we propose adding a new attribute to a buffer profile to allow configuring packet trimming on such stricter profiles:
```
/**
 * @brief Enum defining queue actions in case the packet fails to pass the admission control.
 */
typedef enum _sai_buffer_profile_packet_admission_fail_action_t
{
    /**
     * @brief Drop the packet.
     *
     * Default action. Packet has nowhere to go
     * and will be dropped.
     */
    SAI_BUFFER_PROFILE_PACKET_ADMISSION_FAIL_ACTION_DROP = 0x00000000,

    /**
     * @brief Trim the packet.
     *
     * Try sending a shortened packet over a different
     * queue. SAI_QUEUE_STAT_DROPPED_PACKETS as well as SAI_QUEUE_STAT_DROPPED_BYTES
     * will count the original discarded frames even if they will be trimmed afterwards.
     */
    SAI_BUFFER_PROFILE_PACKET_ADMISSION_FAIL_ACTION_DROP_AND_TRIM = 0x00000001,
} sai_buffer_profile_packet_admission_fail_action_t;

    /**
     * @brief Buffer profile discard action
     *
     * Action to be taken upon packet discard due to
     * buffer profile configuration. Applicable only
     * when attached to a queue.
     *
     * @type sai_buffer_profile_packet_admission_fail_action_t
     * @flags CREATE_AND_SET
     * @default SAI_BUFFER_PROFILE_PACKET_ADMISSION_FAIL_ACTION_DROP
     */
    SAI_BUFFER_PROFILE_ATTR_PACKET_ADMISSION_FAIL_ACTION,
```

Trimming engine attributes are configured globally.
```
   /**
     * @brief Packet trimming size
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 128
     */
    SAI_SWITCH_ATTR_PACKET_TRIMMING_SIZE,

    /**
     * @brief New packet trimming DSCP value
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_PACKET_TRIMMING_DSCP_VALUE,

    /**
     * @brief New packet trimming queue index
     *
     * @type sai_uint8_t
     * @flags CREATE_AND_SET
     * @default 0
     */
    SAI_SWITCH_ATTR_PACKET_TRIMMING_QUEUE_INDEX,
```

If more granularity is needed (e.g. trim a specific protocol, or packets within protocol), ACL action is added to disable trimming even if the packet is eligible due to a queue with a buffer profile attached that has trimming enabled.
```
   /**
     * @brief Disable packet trimming for a given match condition.
     *
     * This rule takes effect only when packet trimming is configured on a TC to which a packet belongs.
     *
     * @type sai_acl_action_data_t bool
     * @flags CREATE_AND_SET
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_ACTION_DISABLE_TRIMMING = SAI_ACL_ENTRY_ATTR_ACTION_START + 0x39,
```
