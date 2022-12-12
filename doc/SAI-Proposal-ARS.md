#  Adaptive Routing and Switching
-------------------------------------------------------------------------------
 Title       | SAI support for Adaptive Routing and Switching
-------------|-----------------------------------------------------------------
 Authors     | Jai Kumar, Broadcom Inc.
 Status      | In review
 Type        | Standards track
 Created     | 07/08/2022: Initial Draft
 SAI-Version |
-------------------------------------------------------------------------------

## 1.0  Introduction

With the advent of RoCE, ethernet based networks are extensively used in AI/ML clusters. AI/ML workloads present a very different challenge where the load increases very rapidly. Traditional methods of managing the traffic across the network for best network utilization are static in nature and do not account for the variability and sensitivity of AI/ML traffic patterns.

> https://www.youtube.com/watch?v=miv5PExXTmc
>
>Session presented in OCP 2022, notes that AI/ML infrastructure with expensive accelerators deployed spend up to 57% idle time "waiting for the network" for the Grand Teton AI/ML servers.

ARS attempts to address some of the shortcomings in the existing network deployments.

## 2.0 Existing Forwarding Decision Model
Today the routing protocol or SDN controller decides the reachability of a given destination and finds all the possible paths. These paths may be equal cost or unequal cost. Decision to choose one of the paths out of all available paths is done in the switch pipeline based on a computed hash. This path selection is static in nature based on the packet fields. Hash based selection doesn't take into account the dynamic state of the local or end to end path.

There are control plane protocols for traffic engineering a path but involves a control plane decision that is very slow to react to changing traffic patterns in the network.

Some of the observed shortcomings are
- Load balancing requires different sizes of flows to be evenly distributed across aggregate member links.
- Size and duration of flows is not considered when performing aggregate member assignment.
- Once a member link is assigned there is no consideration of degradation of the link because of overloading or queue congestion.
- Polarization: where traffic takes a particular path even if the path is congested.
- Local Link Quality: There is no concept of link quality in path selection.
- End to End Path Quality: There is no concept of path quality between two endpoints.
- Link failure: where control plane rebalances the available paths but results in blackholing of traffic while rebalance is going on.

These shortcomings result in blackholing, slow convergence, dropped traffic and/or under utilization of the network thereby resulting in under utilization of expensive accelerators.

## 3.0 Adaptive Routing and Switching

ARS attempts to address these shortcomings by
1. Accounting for local and remote link quality for path selection
2. Use quality metrics to account for a changing traffic pattern in the network
3. Dynamic path assignment and reassignment without any interaction with the control plane
4. Minimize traffic drop when link flaps by HW based link down detection and path reassignment

### 3.1 Terms and Acronyms
| Acronym   | Term      |
|:----------|:----------|
| Micro Flow    | A single traffic flow typically identified by 5 tuple    |
| Macro Flow    | collection of micro flows that hash to same value   |
| Idle Time | Time when there are no packets observed within a macro flow |
| Flowlet    | contagious subsequence of packets within a macro flow separated by an idle time interval    |
| Macro Flow Table    | HW table to store per macro flow state    |
| EWMA | Exponential weighted moving average |
| ARS    | Adaptive routing and switching: Path selection is influenced by quality metrics    |
| NHG | Next Hop Group |

### 3.2 Key Idea behind ARS
1. ARS is flowlet based

      - *A flow is viewed as a sequence of flowlets separated by idle time*

2. Path selection is based on local visibility into link quality

      - *Two metrics are considered. Historical load:Bytes sent over an interval of time and future load: Bytes queued for transmission*

3. Path reassignment takes flow (in)activity into account

      - *Best path is selected when the flow starts. Path is locked while flow is active.*
      - *Flow becomes eligible for path reassignment after the idle time.*

      - *Idle time should be configured typically as 1/2 of RTT*

4. Algorithm for load computation

      - *EWMA is used for computing the historical and future load*

5. Quantization

      - *Computed value need to be quantized into buckets or bands for easier intepretation*

6. Non minimal cost path selection

      - *ARS can be used to partition a NHG into a primary and alternate set of paths. Alternate paths are typically non minimal cost or side links based on the topology. The alternate paths are chosen when primary paths are relatively worse.*

7. LAG

      - *Besides being able to choose best quality path from the NHG, packet should also be able to select best link available amongst LAG memembers*

8. Link Failure or Admin Down

      - *Besides being able to choose best quality path from the NHG, packet should also be able to select best link available amongst LAG memembers*

> *Additional algorithms and quality metrics can be added laters*

### 3.3 ARS path selections modes

1. Flow-let based

      - 	*best quality path till the flow-let hits an idle time*

      - *random path with out considering path quality*

2. Per packet

      - *best quality path assignment for each packet*

      - *random path assignment for each packet*

3. Fixed: fixed path assignment

      - *mostly used for debugging*

## 4.0 ARS Packet Flow

![Adaptive Routing and Switching](../figures/ARS-Packet-Flow.png "Figure 1: ARS Packet Flow")

__Figure 1: ARS Packet Flow__

## 5.0 ARS SAI Components

ARS processing can be broken down into following components.

1. Global Configuration
2. Sampling rate (Global)
3. Quality metric generation
4. Algorithm decision: EWMA
5. Algorithm quality parameters: past and future load, link state
6. Quantization: Compaction of quality measured data
7. Quality bands: Mapping of quantized quality to quality bands
8. Participating links
9. Scaling factor for normalization of different port speeds
10. Weight of past and future load in quality computation

### 5.1 New Objects
Following two new SAI objects are created.
ARS profile object is a global object with a switch binding.

ARS object is a per nexthop group object with a NHG binding.

#### 5.1.1 ARS Profile Object
ARS profile object is created to capture the global configuration and hardware related read only scale attributes. ARS profile object has a switch object level binding.

ARS profile object attribute in switch object is used for capability query to find out if a given platform supports ARS or not.

#### 5.1.2 ARS Object

ARS object defines a behavior of path selection for a given packet. There can be multiple NHG objects pointing to the same ARS object or multiple ARS objects can be created to capture differeing traffic patterns in the network.

## 6.0 Sample Workflow

This section talks about enabling ARS and controlling its behavior for different deployment considerations.

### 6.1 Create ARS Profile Object

ARS profile objects specify the Algorithm to be used for load computation, weight for the load computation metrics, quantization parameters and sampling interval for the load computation.

```
/*
 * Create an ARS profile object:
 * -----------------------------
 */
sai_attr_list[0].id = SAI_ARS_PROFILE_ATTR_ALGO;
sai_attr_list[0].value.s32 = SAI_ARS_QUALITY_MAP_ALGO_EWMA;

sai_attr_list[1].id = SAI_ARS_PROFILE_ATTR_SAMPLING_INTERVAL;
sai_attr_list[1].value.u16 = 16;

sai_attr_list[2].id = SAI_ARS_PROFILE_ATTR_ARS_RANDOM_SEED;
sai_attr_list[2].value.u32 = 0x12345678;

sai_attr_list[3].id = SAI_ARS_PROFILE_ATTR_PORT_LOAD_PAST;
sai_attr_list[3].value.booldata = true;

sai_attr_list[4].id = SAI_ARS_PROFILE_ATTR_PORT_LOAD_PAST_WEIGHT;
sai_attr_list[4].value.u8 = 16;

sai_attr_list[5].id = SAI_ARS_PROFILE_ATTR_PORT_LOAD_FUTURE;
sai_attr_list[5].value.booldata = true;

sai_attr_list[6].id = SAI_ARS_PROFILE_ATTR_PORT_LOAD_FUTURE_WEIGHT;
sai_attr_list[6].value.u8 = 16;

sai_attr_list[7].id = SAI_ARS_PROFILE_ATTR_PORT_LOAD_CURRENT;
sai_attr_list[7].value.booldata = false;

sai_attr_list[8].id = SAI_ARS_PROFILE_ATTR_PORT_LOAD_EXPONENT;
sai_attr_list[8].value.u8 = 2;

sai_attr_list[9].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_0_MIN_THRESHOLD;
sai_attr_list[9].value.u32 = 10;

sai_attr_list[10].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_0_MAX_THRESHOLD;
sai_attr_list[10].value.u32 = 100;

sai_attr_list[11].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_1_MIN_THRESHOLD;
sai_attr_list[11].value.u32 = 100;

sai_attr_list[12].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_1_MAX_THRESHOLD;
sai_attr_list[12].value.u32 = 200;

sai_attr_list[13].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_2_MIN_THRESHOLD;
sai_attr_list[13].value.u32 = 200;

sai_attr_list[14].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_2_MAX_THRESHOLD;
sai_attr_list[14].value.u32 = 300;

sai_attr_list[15].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_3_MIN_THRESHOLD;
sai_attr_list[15].value.u32 = 300;

sai_attr_list[16].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_3_MAX_THRESHOLD;
sai_attr_list[16].value.u32 = 400;

sai_attr_list[17].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_4_MIN_THRESHOLD;
sai_attr_list[17].value.u32 = 400;

sai_attr_list[18].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_4_MAX_THRESHOLD;
sai_attr_list[18].value.u32 = 500;

sai_attr_list[19].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_5_MIN_THRESHOLD;
sai_attr_list[19].value.u32 = 500;

sai_attr_list[20].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_5_MAX_THRESHOLD;
sai_attr_list[20].value.u32 = 600;

sai_attr_list[21].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_6_MIN_THRESHOLD;
sai_attr_list[21].value.u32 = 600;

sai_attr_list[22].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_6_MAX_THRESHOLD;
sai_attr_list[22].value.u32 = 700;

sai_attr_list[23].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_7_MIN_THRESHOLD;
sai_attr_list[23].value.u32 = 700;

sai_attr_list[24].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_7_MAX_THRESHOLD;
sai_attr_list[24].value.u32 = 800;

attr_count = 25;
sai_create_ars_profile_fn(
	&sai_ars_profile_id,
	switch_id,
	attr_count,
	sai_attr_list);
```

### 6.2 Bind ARS Profile Object to Switch Object
Bind the ars profile object created in the previous step to the switch. There can be only a single ARS profile specified for a switch.

```
sai_attr.id = SAI_SWITCH_ATTR_ARS_PROFILE;
sai_attr.value.oid = sai_ars_profile_id;

sai_set_switch_attribute_fn(
	switch_id,
	&sai_attr);

```

### 6.3 Enable ARS on Participating Ports

ARS may be enabled on all the ports on the switch or only a few of the ports like uplink ports connecting to the spine layer. Port can be configured for different parameters for load computation.

Also a NHG may consist of ports of different speeds so there is a need to normalize the port speeds for uniform computation across ports. Following configuration is done for enabling ARS, ars load parameters and port normalization.

Port normalization is a simple scaling factor with a base of 10G.

| Port Speed  | Scale Value  |
|:----------|:----------|
| 10G    | 1    |
| 25G    | 2.5    |
| 40G    | 4    |
| 50G    | 5    |
| 100G    | 10    |
| 200G    | 20    |
| 400G    | 40    |

```
sai_attr.id = SAI_PORT_ATTR_ARS_ENABLE;
sai_attr.value.booldata = true;
sai_set_port_attribute_fn(
	port_id,
	&sai_attr);

sai_attr.id = SAI_PORT_ATTR_ARS_PORT_LOAD_SCALING_FACTOR;
sai_attr.value.u32 = 10;
sai_set_port_attribute_fn(
	port_id,
	&sai_attr);

sai_attr.id = SAI_PORT_ATTR_ARS_PORT_LOAD_PAST_ENABLE;
sai_attr.value.booldata = true;
sai_set_port_attribute_fn(
	port_id,
	&sai_attr);

sai_attr.id = SAI_PORT_ATTR_ARS_PORT_LOAD_FUTURE_ENABLE;
sai_attr.value.booldata = true;
sai_set_port_attribute_fn(
	port_id,
	&sai_attr);

```

### 6.4 Create ARS Object
Once the ARS profile is created and ports are configured, the next step is to configure NHG for adaptive routing. This is done by creating an ARS object and bind it to an NHG object.

There can be one ARS object bound to multiple NHG or there can be different ARS objects for different NHG. Only constraint is that one NHG can not point to multiple ARS objects i.e. NHG binding to ARS objects is not a list.

#### 6.4.1 ARS Mode

Each arriving packet is identified as part of the macro flow. If the macro flow itself is new, the best quality link is assigned from the available member link pool.

Path assignment can be done at a flowlet or per packet boundary based on the configured mode. Fixed assignment is done mostly for debugging purposes.

```
/**
 * @brief Adaptive routing and switching path (re)assignment mode
 */
typedef enum _sai_ars_mode_t
{
    /** Per flow-let quality based path (re)assignment */
    SAI_ARS_MODE_FLOWLET_QUALITY,

    /** Per flow-let random path (re)assignment */
    SAI_ARS_MODE_FLOWLET_RANDOM,

    /** Per packet quality based path (re)assignment */
    SAI_ARS_MODE_PER_PACKET_QUALITY,

    /** Per packet random path (re)assignment */
    SAI_ARS_MODE_PER_PACKET_RANDOM,

    /** Fixed path assignment */
    SAI_ARS_MODE_FIXED,

} sai_ars_mode_t;

/*
 * Create an ARS profile object:
 * Set the ARS mode
 * -----------------------------
 */
sai_attr_list[0].id = SAI_ARS_ATTR_MODE;
sai_attr_list[0].value.s32 = SAI_ARS_MODE_FLOWLET_QUALITY;
```

#### 6.4.2 Idle time

Idle time defines a time spacing between the packets in a macro flow. Sequence of packets in a macro flow separated by idle time is a flowlet.

Each flowlet is eligible for a new link assignment. Idle time can also be viewed as the window where link reassignment can be done safely to avoid packet reordering. Though packet reorder may still occur as packets will take a different path to the destination after the reassignment.

> (Current time - Observation Time) > Idle Time

```
/*
 * Set the Idle Time for flowlet
 * -----------------------------
 */
sai_attr_list[1].id = SAI_ARS_ATTR_IDLE_TIME;
sai_attr_list[1].value.u32 =  256;
```
#### 6.4.3 Max Flows per NHG
This attribute specifies the maximum number of flows that can be learned in the ARS enabled NHG.

```
/*
 * Set the Idle Time for flowlet
 * -----------------------------
 */
sai_attr_list[2].id = SAI_ARS_ATTR_MAX_FLOWS;
sai_attr_list[2].value.u32 = 512;
```

#### 6.4.4 Enable ARS Monitoring

ARS monitoring is an important feature to analyze the performance of ARS enabled domains.

Monitoring can be enabled on a per NHG basis using the SAI_ARS_ATTR_MON_ENABLE attribute. When monitoring is enabled mirror copy can be generated using the sampler attached to the ARS object. Mirror copy is generated whenever path reassignment happens. Contents of mirror copy are chip vendor specific and are not captured in the specification.

```
/*
 * Enable ARS moinitoring using a sampler
 * --------------------------------------
 */
sai_attr_list[3].id = SAI_ARS_ATTR_MON_ENABLE;
sai_attr_list[3].value.booldata = true;

sai_attr_list[4].id = SAI_ARS_ATTR_SAMPLEPACKET_ENABLE;
sai_attr_list[4].value.oid = sai_sampler_id;

sai_attr_list[5].id = SAI_ARS_ATTR_MAX_PRIMARY_MEMEBERS_PER_GROUP;
sai_attr_list[5].value.u32 = 64;

sai_attr_list[6].id = SAI_ARS_ATTR_MAX_ALT_MEMEBERS_PER_GROUP;
sai_attr_list[6].value.u32 = 64;

attr_count = 7;
sai_create_ars_fn(
	&sai_ars_id,
	switch_id,
	attr_count,
	sai_attr_list);
```

### 7.0 Path Selection Across Primary and Alternamte Sets from NHG Member

ARS enabled NHG can be partitioned into a primary and alternate set of paths. Such partition is needed when there are east west links or there are non minimal cost paths that are available but not used. These paths can be captured as a set by the control plane in the ARS enabled NHG object. Idea is to be able to use this alternate set either as a non minimal cost path or for some high priority flows depending on how a given deployment wants to use these links.

When the primary choice falls below some configured threshold value quality,  an alternate path is chosen with an associated cost. Bias is introduced for probability of choosing the path.

For example:
- Best quality path in primary set is P1
- Best quality path in alternate set is P4
- if (qualityBand(P1) < SAI_ARS_ATTR_PRIMARY_PATH_QUALITY_THRESHOLD) AND (qualityBand(P1) + SAI_NEXT_HOP_GROUP_MEMBER_ATTR_ARS_ALTERNATE_PATH_COST) > qualityBand(P4) + SAI_NEXT_HOP_GROUP_MEMBER_ATTR_ARS_ALTERNATE_PATH_BIAS) then use P1 else P4.

Note that HW will compute load metrics for both primary and alternate paths. All other characteristics of path assignment remain the same as per ARS profile.

```
/*
 * Configure primary and alternate path selection criteria
 * --------------------------------------------------------
 */

sai_attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_ARS_PRIMARY_PATH_QUALITY_THRESHOLD;
sai_attr.value.u16 = 100;
sai_set_next_hop_group_member_attribute_fn(
	next_hop_group_member_id,
	&sai_attr);

sai_attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_ARS_ALTERNATE_PATH_COST;
sai_attr.value.u16 = 100;
sai_set_next_hop_group_member_attribute_fn(
	next_hop_group_member_id,
	&sai_attr);

sai_attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_ARS_ALTERNATE_PATH_BIAS;
sai_attr.value.u16 = 100;
sai_set_next_hop_group_member_attribute_fn(
	next_hop_group_member_id,
	&sai_attr);

```

### 8.0 Bind ARS Object to NHG Object

```
sai_attr.id = SAI_NEXT_HOP_GROUP_ATTR_ARS_OBJECT_ID;
sai_attr.value.oid = sai_ars_id;

sai_set_next_hop_group_attribute_fn(
	sai_nhg_id,
	&sai_attr);

```

### 9.0 Stats Support

Following per NHG read only attributes are provided for reading ARS related stats.

> SAI_NEXT_HOP_GROUP_ATTR_ARS_PACKET_DROPS: Drops because path reassignment due to link flap
>
> SAI_NEXT_HOP_GROUP_ATTR_ARS_NEXT_HOP_REASSIGNMENTS: Nexthop reassignment when the path quality degrades
>
> SAI_NEXT_HOP_GROUP_ATTR_ARS_PORT_REASSIGNMENTS: Link reassignment when link quality degrades

```
    /**
     * @brief Number of packets dropped by ARS mechanism
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_NEXT_HOP_GROUP_ATTR_ARS_PACKET_DROPS,

    /**
     * @brief Number of ARS next hop reassignments done
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_NEXT_HOP_GROUP_ATTR_ARS_NEXT_HOP_REASSIGNMENTS,

    /**
     * @brief Number of ARS port reassignments done
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_NEXT_HOP_GROUP_ATTR_ARS_PORT_REASSIGNMENTS,
```

### 10.0 How to enable ARS for different traffic profiles
Following attributes are added to the ACL table. These attributes provide fine grain control to enable/disable ARS processing for a specific traffic profile. ACL action of SAI_ACL_ENTRY_ATTR_ACTION_SET_ARS_MONITORING can also be used to provide fine control of monitoring of ARS path reassignments.

```
    /**
     * @brief Enable ARS monitoring for a destination that can be a LAG or nexthopgroup
     *
     * @type sai_acl_action_data_t sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_NEXT_HOP_GROUP
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_ARS_MONITORING = SAI_ACL_ENTRY_ATTR_ACTION_START + 0x35,

    /**
     * @brief Enable ARS object for a destination that can be a LAG or nexthopgroup
     *
     * @type sai_acl_action_data_t sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ARS
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_ARS_OBJECT = SAI_ACL_ENTRY_ATTR_ACTION_START + 0x36,

    /**
     * @brief Disable ARS forwarding for a destination that can be a LAG or nexthopgroup
     *
     * @type sai_acl_action_data_t sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_NEXT_HOP_GROUP
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_ACTION_DISABLE_ARS_FORWARDING = SAI_ACL_ENTRY_ATTR_ACTION_START + 0x37,
```

#### 10.1 Disable TCP
When a flow resolves via NHG with an ARS object.
e.g. match action

# Adaptive Routing and Switching

***

| Title | SAI support for Adaptive Routing and Switching |
| --- | --- |
| Authors | Jai Kumar, Broadcom Inc. |
| Status | In review |
| Type | Standards track |
| Created | 07/08/2022: Initial Draft |
| SAI-Version |  |

***

## 1.0 Introduction

With the advent of RoCE, ethernet based networks are extensively used in AI/ML clusters. AI/ML workloads present a very different challenge where the load increases very rapidly. Traditional methods of managing the traffic across the network for best network utilization are static in nature and do not account for the variability and sensitivity of AI/ML traffic patterns.

> https://www.youtube.com/watch?v=miv5PExXTmc
Session presented in OCP 2022, notes that AI/ML infrastructure with expensive accelerators deployed spend up to 57% idle time "waiting for the network" for the Grand Teton AI/ML servers.

ARS attempts to address some of the shortcomings in the existing network deployments.

## 2.0 Existing Forwarding Decision Model

Today the routing protocol or SDN controller decides the reachability of a given destination and finds all the possible paths. These paths may be equal cost or unequal cost. Decision to choose one of the paths out of all available paths is done in the switch pipeline based on a computed hash. This path selection is static in nature based on the packet fields. Hash based selection doesn't take into account the dynamic state of the local or end to end path.

There are control plane protocols for traffic engineering a path but involves a control plane decision that is very slow to react to changing traffic patterns in the network.

Some of the observed shortcomings are

* Load balancing requires different sizes of flows to be evenly distributed across aggregate member links.
* Size and duration of flows is not considered when performing aggregate member assignment.
* Once a member link is assigned there is no consideration of degradation of the link because of overloading or queue congestion.
* Polarization: where traffic takes a particular path even if the path is congested.
* Local Link Quality: There is no concept of link quality in path selection.
* End to End Path Quality: There is no concept of path quality between two endpoints.
* Link failure: where control plane rebalances the available paths but results in blackholing of traffic while rebalance is going on.

These shortcomings result in blackholing, slow convergence, dropped traffic and/or under utilization of the network thereby resulting in under utilization of expensive accelerators.

## 3.0 Adaptive Routing and Switching

ARS attempts to address these shortcomings by

1. Accounting for local and remote link quality for path selection
2. Use quality metrics to account for a changing traffic pattern in the network
3. Dynamic path assignment and reassignment without any interaction with the control plane
4. Minimize traffic drop when link flaps by HW based link down detection and path reassignment

### 3.1 Terms and Acronyms

| Acronym | Term |
| :--- | :--- |
| Micro Flow | A single traffic flow typically identified by 5 tuple |
| Macro Flow | collection of micro flows that hash to same value |
| Idle Time | Time when there are no packets observed within a macro flow |
| Flowlet | contagious subsequence of packets within a macro flow separated by an idle time interval |
| Macro Flow Table | HW table to store per macro flow state |
| EWMA | Exponential weighted moving average |
| ARS | Adaptive routing and switching: Path selection is influenced by quality metrics |
| NHG | Next Hop Group |

### 3.2 Key Idea behind ARS

1. ARS is flowlet based

      - _A flow is viewed as a sequence of flowlets separated by idle time_

1. Path selection is based on local visibility into link quality

      - _Two metrics are considered. Historical loadBytes sent over an interval of time and future load Bytes queued for transmission_

1. Path reassignment takes flow (in)activity into account

      - _Best path is selected when the flow starts. Path is locked while flow is active._
      - _Flow becomes eligible for path reassignment after the idle time._

      - _Idle time should be configured typically as 1/2 of RTT_

1. Algorithm for load computation

      - _EWMA is used for computing the historical and future load_

1. Quantization

      - _Computed value need to be quantized into buckets or bands for easier intepretation_

1. Non minimal cost path selection

      - _ARS can be used to partition a NHG into a primary and alternate set of paths. Alternate paths are typically non minimal cost or side links based on the topology. The alternate paths are chosen when primary paths are relatively worse._

1. LAG

      - _Besides being able to choose best quality path from the NHG, packet should also be able to select best link available amongst LAG memembers_

1. Link Failure or Admin Down

      - _Besides being able to choose best quality path from the NHG, packet should also be able to select best link available amongst LAG memembers_

> _Additional algorithms and quality metrics can be added laters_

### 3.3 ARS path selections modes

1. Flow-let based

      - 	_best quality path till the flow-let hits an idle time_

      - _random path with out considering path quality_

1. Per packet

      - _best quality path assignment for each packet_

      - _random path assignment for each packet_

1. Fixed: fixed path assignment

      - _mostly used for debugging_

## 4.0 ARS Packet Flow

![Adaptive Routing and Switching](../figures/ARS-Packet-Flow.png "Figure 1: ARS Packet Flow")

**Figure 1: ARS Packet Flow**

## 5.0 ARS SAI Components

ARS processing can be broken down into following components.

1. Global Configuration
2. Sampling rate (Global)
3. Quality metric generation
4. Algorithm decision: EWMA
5. Algorithm quality parameters: past and future load, link state
6. Quantization: Compaction of quality measured data
7. Quality bands: Mapping of quantized quality to quality bands
8. Participating links
9. Scaling factor for normalization of different port speeds
10. Weight of past and future load in quality computation

### 5.1 New Objects

Following two new SAI objects are created.
ARS profile object is a global object with a switch binding.

ARS object is a per nexthop group object with a NHG binding.

#### 5.1.1 ARS Profile Object

ARS profile object is created to capture the global configuration and hardware related read only scale attributes. ARS profile object has a switch object level binding.

ARS profile object attribute in switch object is used for capability query to find out if a given platform supports ARS or not.

#### 5.1.2 ARS Object

ARS object defines a behavior of path selection for a given packet. There can be multiple NHG objects pointing to the same ARS object or multiple ARS objects can be created to capture differeing traffic patterns in the network.

## 6.0 Sample Workflow

This section talks about enabling ARS and controlling its behavior for different deployment considerations.

### 6.1 Create ARS Profile Object

ARS profile objects specify the Algorithm to be used for load computation, weight for the load computation metrics, quantization parameters and sampling interval for the load computation.

```
/*
 * Create an ARS profile object:
 * -----------------------------
 */
sai_attr_list[0].id = SAI_ARS_PROFILE_ATTR_ALGO;
sai_attr_list[0].value.s32 = SAI_ARS_QUALITY_MAP_ALGO_EWMA;

sai_attr_list[1].id = SAI_ARS_PROFILE_ATTR_SAMPLING_INTERVAL;
sai_attr_list[1].value.u16 = 16;

sai_attr_list[2].id = SAI_ARS_PROFILE_ATTR_ARS_RANDOM_SEED;
sai_attr_list[2].value.u32 = 0x12345678;

sai_attr_list[3].id = SAI_ARS_PROFILE_ATTR_PORT_LOAD_PAST;
sai_attr_list[3].value.booldata = true;

sai_attr_list[4].id = SAI_ARS_PROFILE_ATTR_PORT_LOAD_PAST_WEIGHT;
sai_attr_list[4].value.u8 = 16;

sai_attr_list[5].id = SAI_ARS_PROFILE_ATTR_PORT_LOAD_FUTURE;
sai_attr_list[5].value.booldata = true;

sai_attr_list[6].id = SAI_ARS_PROFILE_ATTR_PORT_LOAD_FUTURE_WEIGHT;
sai_attr_list[6].value.u8 = 16;

sai_attr_list[7].id = SAI_ARS_PROFILE_ATTR_PORT_LOAD_CURRENT;
sai_attr_list[7].value.booldata = false;

sai_attr_list[8].id = SAI_ARS_PROFILE_ATTR_PORT_LOAD_EXPONENT;
sai_attr_list[8].value.u8 = 2;

sai_attr_list[9].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_0_MIN_THRESHOLD;
sai_attr_list[9].value.u32 = 10;

sai_attr_list[10].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_0_MAX_THRESHOLD;
sai_attr_list[10].value.u32 = 100;

sai_attr_list[11].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_1_MIN_THRESHOLD;
sai_attr_list[11].value.u32 = 100;

sai_attr_list[12].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_1_MAX_THRESHOLD;
sai_attr_list[12].value.u32 = 200;

sai_attr_list[13].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_2_MIN_THRESHOLD;
sai_attr_list[13].value.u32 = 200;

sai_attr_list[14].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_2_MAX_THRESHOLD;
sai_attr_list[14].value.u32 = 300;

sai_attr_list[15].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_3_MIN_THRESHOLD;
sai_attr_list[15].value.u32 = 300;

sai_attr_list[16].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_3_MAX_THRESHOLD;
sai_attr_list[16].value.u32 = 400;

sai_attr_list[17].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_4_MIN_THRESHOLD;
sai_attr_list[17].value.u32 = 400;

sai_attr_list[18].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_4_MAX_THRESHOLD;
sai_attr_list[18].value.u32 = 500;

sai_attr_list[19].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_5_MIN_THRESHOLD;
sai_attr_list[19].value.u32 = 500;

sai_attr_list[20].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_5_MAX_THRESHOLD;
sai_attr_list[20].value.u32 = 600;

sai_attr_list[21].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_6_MIN_THRESHOLD;
sai_attr_list[21].value.u32 = 600;

sai_attr_list[22].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_6_MAX_THRESHOLD;
sai_attr_list[22].value.u32 = 700;

sai_attr_list[23].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_7_MIN_THRESHOLD;
sai_attr_list[23].value.u32 = 700;

sai_attr_list[24].id = SAI_ARS_PROFILE_ATTR_QUANT_BAND_7_MAX_THRESHOLD;
sai_attr_list[24].value.u32 = 800;

attr_count = 25;
sai_create_ars_profile_fn(
	&sai_ars_profile_id,
	switch_id,
	attr_count,
	sai_attr_list);
```

### 6.2 Bind ARS Profile Object to Switch Object

Bind the ars profile object created in the previous step to the switch. There can be only a single ARS profile specified for a switch.

```
sai_attr.id = SAI_SWITCH_ATTR_ARS_PROFILE;
sai_attr.value.oid = sai_ars_profile_id;

sai_set_switch_attribute_fn(
	switch_id,
	&sai_attr);

```

### 6.3 Enable ARS on Participating Ports

ARS may be enabled on all the ports on the switch or only a few of the ports like uplink ports connecting to the spine layer. Port can be configured for different parameters for load computation.

Also a NHG may consist of ports of different speeds so there is a need to normalize the port speeds for uniform computation across ports. Following configuration is done for enabling ARS, ars load parameters and port normalization.

Port normalization is a simple scaling factor with a base of 10G.

| Port Speed | Scale Value |
| :--- | :--- |
| 10G | 1 |
| 25G | 2.5 |
| 40G | 4 |
| 50G | 5 |
| 100G | 10 |
| 200G | 20 |
| 400G | 40 |

```
sai_attr.id = SAI_PORT_ATTR_ARS_ENABLE;
sai_attr.value.booldata = true;
sai_set_port_attribute_fn(
	port_id,
	&sai_attr);

sai_attr.id = SAI_PORT_ATTR_ARS_PORT_LOAD_SCALING_FACTOR;
sai_attr.value.u32 = 10;
sai_set_port_attribute_fn(
	port_id,
	&sai_attr);

sai_attr.id = SAI_PORT_ATTR_ARS_PORT_LOAD_PAST_ENABLE;
sai_attr.value.booldata = true;
sai_set_port_attribute_fn(
	port_id,
	&sai_attr);

sai_attr.id = SAI_PORT_ATTR_ARS_PORT_LOAD_FUTURE_ENABLE;
sai_attr.value.booldata = true;
sai_set_port_attribute_fn(
	port_id,
	&sai_attr);

```

### 6.4 Create ARS Object

Once the ARS profile is created and ports are configured, the next step is to configure NHG for adaptive routing. This is done by creating an ARS object and bind it to an NHG object.

There can be one ARS object bound to multiple NHG or there can be different ARS objects for different NHG. Only constraint is that one NHG can not point to multiple ARS objects i.e. NHG binding to ARS objects is not a list.

#### 6.4.1 ARS Mode

Each arriving packet is identified as part of the macro flow. If the macro flow itself is new, the best quality link is assigned from the available member link pool.

Path assignment can be done at a flowlet or per packet boundary based on the configured mode. Fixed assignment is done mostly for debugging purposes.

```
/**
 * @brief Adaptive routing and switching path (re)assignment mode
 */
typedef enum _sai_ars_mode_t
{
    /** Per flow-let quality based path (re)assignment */
    SAI_ARS_MODE_FLOWLET_QUALITY,

    /** Per flow-let random path (re)assignment */
    SAI_ARS_MODE_FLOWLET_RANDOM,

    /** Per packet quality based path (re)assignment */
    SAI_ARS_MODE_PER_PACKET_QUALITY,

    /** Per packet random path (re)assignment */
    SAI_ARS_MODE_PER_PACKET_RANDOM,

    /** Fixed path assignment */
    SAI_ARS_MODE_FIXED,

} sai_ars_mode_t;

/*
 * Create an ARS profile object:
 * Set the ARS mode
 * -----------------------------
 */
sai_attr_list[0].id = SAI_ARS_ATTR_MODE;
sai_attr_list[0].value.s32 = SAI_ARS_MODE_FLOWLET_QUALITY;
```

#### 6.4.2 Idle time

Idle time defines a time spacing between the packets in a macro flow. Sequence of packets in a macro flow separated by idle time is a flowlet.

Each flowlet is eligible for a new link assignment. Idle time can also be viewed as the window where link reassignment can be done safely to avoid packet reordering. Though packet reorder may still occur as packets will take a different path to the destination after the reassignment.

> (Current time - Observation Time) > Idle Time

```
/*
 * Set the Idle Time for flowlet
 * -----------------------------
 */
sai_attr_list[1].id = SAI_ARS_ATTR_IDLE_TIME;
sai_attr_list[1].value.u32 =  256;
```

#### 6.4.3 Max Flows per NHG

This attribute specifies the maximum number of flows that can be learned in the ARS enabled NHG.

```
/*
 * Set the Idle Time for flowlet
 * -----------------------------
 */
sai_attr_list[2].id = SAI_ARS_ATTR_MAX_FLOWS;
sai_attr_list[2].value.u32 = 512;
```

#### 6.4.4 Enable ARS Monitoring

ARS monitoring is an important feature to analyze the performance of ARS enabled domains.

Monitoring can be enabled on a per NHG basis using the SAI_ARS_ATTR_MON_ENABLE attribute. When monitoring is enabled mirror copy can be generated using the sampler attached to the ARS object. Mirror copy is generated whenever path reassignment happens. Contents of mirror copy are chip vendor specific and are not captured in the specification.

```
/*
 * Enable ARS moinitoring using a sampler
 * --------------------------------------
 */
sai_attr_list[3].id = SAI_ARS_ATTR_MON_ENABLE;
sai_attr_list[3].value.booldata = true;

sai_attr_list[4].id = SAI_ARS_ATTR_SAMPLEPACKET_ENABLE;
sai_attr_list[4].value.oid = sai_sampler_id;

sai_attr_list[5].id = SAI_ARS_ATTR_MAX_PRIMARY_MEMEBERS_PER_GROUP;
sai_attr_list[5].value.u32 = 64;

sai_attr_list[6].id = SAI_ARS_ATTR_MAX_ALT_MEMEBERS_PER_GROUP;
sai_attr_list[6].value.u32 = 64;

attr_count = 7;
sai_create_ars_fn(
	&sai_ars_id,
	switch_id,
	attr_count,
	sai_attr_list);
```

### 7.0 Path Selection Across Primary and Alternamte Sets from NHG Member

ARS enabled NHG can be partitioned into a primary and alternate set of paths. Such partition is needed when there are east west links or there are non minimal cost paths that are available but not used. These paths can be captured as a set by the control plane in the ARS enabled NHG object. Idea is to be able to use this alternate set either as a non minimal cost path or for some high priority flows depending on how a given deployment wants to use these links.

When the primary choice falls below some configured threshold value quality,  an alternate path is chosen with an associated cost. Bias is introduced for probability of choosing the path.

For example:

* Best quality path in primary set is P1
* Best quality path in alternate set is P4
* if (qualityBand(P1) < SAI_ARS_ATTR_PRIMARY_PATH_QUALITY_THRESHOLD) AND (qualityBand(P1) + SAI_NEXT_HOP_GROUP_MEMBER_ATTR_ARS_ALTERNATE_PATH_COST) > qualityBand(P4) + SAI_NEXT_HOP_GROUP_MEMBER_ATTR_ARS_ALTERNATE_PATH_BIAS) then use P1 else P4.

Note that HW will compute load metrics for both primary and alternate paths. All other characteristics of path assignment remain the same as per ARS profile.

```
/*
 * Configure primary and alternate path selection criteria
 * --------------------------------------------------------
 */

sai_attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_ARS_PRIMARY_PATH_QUALITY_THRESHOLD;
sai_attr.value.u16 = 100;
sai_set_next_hop_group_member_attribute_fn(
	next_hop_group_member_id,
	&sai_attr);

sai_attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_ARS_ALTERNATE_PATH_COST;
sai_attr.value.u16 = 100;
sai_set_next_hop_group_member_attribute_fn(
	next_hop_group_member_id,
	&sai_attr);

sai_attr.id = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_ARS_ALTERNATE_PATH_BIAS;
sai_attr.value.u16 = 100;
sai_set_next_hop_group_member_attribute_fn(
	next_hop_group_member_id,
	&sai_attr);

```

### 8.0 Bind ARS Object to NHG Object

```
sai_attr.id = SAI_NEXT_HOP_GROUP_ATTR_ARS_OBJECT_ID;
sai_attr.value.oid = sai_ars_id;

sai_set_next_hop_group_attribute_fn(
	sai_nhg_id,
	&sai_attr);

```

### 9.0 Stats Support

Following per NHG read only attributes are provided for reading ARS related stats.

> SAI_NEXT_HOP_GROUP_ATTR_ARS_PACKET_DROPS: Drops because path reassignment due to link flap
SAI_NEXT_HOP_GROUP_ATTR_ARS_NEXT_HOP_REASSIGNMENTS: Nexthop reassignment when the path quality degrades
SAI_NEXT_HOP_GROUP_ATTR_ARS_PORT_REASSIGNMENTS: Link reassignment when link quality degrades

```
    /**
     * @brief Number of packets dropped by ARS mechanism
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_NEXT_HOP_GROUP_ATTR_ARS_PACKET_DROPS,

    /**
     * @brief Number of ARS next hop reassignments done
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_NEXT_HOP_GROUP_ATTR_ARS_NEXT_HOP_REASSIGNMENTS,

    /**
     * @brief Number of ARS port reassignments done
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_NEXT_HOP_GROUP_ATTR_ARS_PORT_REASSIGNMENTS,
```

### 10.0 How to enable ARS for different traffic profiles

Following attributes are added to the ACL table. These attributes provide fine grain control to enable/disable ARS processing for a specific traffic profile. ACL action of SAI_ACL_ENTRY_ATTR_ACTION_SET_ARS_MONITORING can also be used to provide fine control of monitoring of ARS path reassignments.

```
    /**
     * @brief Enable ARS monitoring for a destination that can be a LAG or nexthopgroup
     *
     * @type sai_acl_action_data_t sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_NEXT_HOP_GROUP
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_ARS_MONITORING = SAI_ACL_ENTRY_ATTR_ACTION_START + 0x35,

    /**
     * @brief Enable ARS object for a destination that can be a LAG or nexthopgroup
     *
     * @type sai_acl_action_data_t sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ARS
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_ACTION_SET_ARS_OBJECT = SAI_ACL_ENTRY_ATTR_ACTION_START + 0x36,

    /**
     * @brief Disable ARS forwarding for a destination that can be a LAG or nexthopgroup
     *
     * @type sai_acl_action_data_t sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_LAG, SAI_OBJECT_TYPE_NEXT_HOP_GROUP
     * @default disabled
     */
    SAI_ACL_ENTRY_ATTR_ACTION_DISABLE_ARS_FORWARDING = SAI_ACL_ENTRY_ATTR_ACTION_START + 0x37,
```

#### 10.1 Disable TCP

When a flow resolves via NHG with an ARS object.
e.g. match action <SAI_ACL_ACTION_TYPE_DISABLE_ARS_FORWARDING>
In this case TCP flows resolving via NHG with ARS object will go through normal NHG object processing. This rule can be used as a catch all rule for all TCP traffic types, even if NHG does not have any ARS object present and will be noop in this case.

#### 10.2 Enable UDP Only

When a flow resolves via NHG without ARS object
e.g. match action < SAI_ACL_ACTION_TYPE_SET_ARS_OBJECT>
In this case UDP flows will undergo ARS object processing.

#### 10.3 Override ARS enabled NHG based on ACL policy

When a flow resolves via NHG with ARS object.
e.g. match action < SAI_ACL_ACTION_TYPE_SET_ARS_OBJECT>
In this case ARS object OID as provided by the ACL rule will be used instead of NHG provided ARS OID.

### 11.0 Enabling ARS on LAG

ARS objects can also be binded to LAG objects. Following new attributes are added to the LAG object.
Packet drops and port reassignments are tracked on a per LAG object basis.

```
/**
     * @brief Adaptive routing and switching object for LAG.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ARS
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
    SAI_LAG_ATTR_ARS_OBJECT_ID,

    /**
     * @brief Number of packets dropped by ARS mechanism
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_LAG_ATTR_ARS_PACKET_DROPS,

    /**
     * @brief Number of ARS port reassignments done
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_LAG_ATTR_ARS_PORT_REASSIGNMENTS,
```

### 11.0 Capability Query

This section describes the workflow for querying the device for ARS capability and if the device is ARS capable then ARS resources.

#### 11.1 ARS Capability

Device can be queried using the capability query for ARS profile object. Success will return the sai_attr_capability_t structure with create/get/set capabilities.

```
sai_query_attribute_capability(
        switch_id,
        SAI_OBJECT_TYPE_ARS_PROFILE,
        SAI_SWITCH_ATTR_ARS_PROFILE,
        attr_cap);
```

#### 11.2 ARS Global Scale Information

Following attributes can be read by the get_ars_profile_attribute() API.

```
    /**
     * @brief Maximum number of ECMP ARS groups
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_ECMP_ARS_MAX_GROUPS,

    /**
     * @brief Maximum number of members per ECMP ARS group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_ECMP_ARS_MAX_MEMBERS_PER_GROUP,

    /**
     * @brief Maximum number of LAG ARS groups
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_LAG_ARS_MAX_GROUPS,

    /**
     * @brief Maximum number of members per LAG ARS group
     *
     * @type sai_uint32_t
     * @flags READ_ONLY
     */
    SAI_ARS_PROFILE_ATTR_LAG_ARS_MAX_MEMBERS_PER_GROUP,
```

#### 11.3 Nexthop Group Specific Scale

NHG specific scale is captured as a configuration attribute in the ARS object. Each ARS object can be configured to support

* Maximum number of flow state that can be maintained
* Maximum number of primary paths
* Maximum number of alternate paths

sai_set_ars_attribute_fn() API can be used to set the scale per ARS object.

```
    /**
     * @brief Maximum number of flow states that can be maintained per this ARS object
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 512
     */
    SAI_ARS_ATTR_MAX_FLOWS,

    /**
     * @brief Maximum number of primary members per adaptive routing group
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 16
     */
    SAI_ARS_ATTR_MAX_PRIMARY_MEMEBERS_PER_GROUP,

    /**
     * @brief Maximum number of alternate members per adaptive routing group
     *
     * @type sai_uint32_t
     * @flags CREATE_AND_SET
     * @default 16
     */
    SAI_ARS_ATTR_MAX_ALT_MEMEBERS_PER_GROUP,
```

### 12.0 Community meeting Q&A

#### 12.1 Clarify how setting the ARS Oid in ACL action relates to a ARS Object

@eddyk-nvidia eddyk-nvidia 9 days ago
Please clarify how setting the ARS Oid in ACL action relates to a ARS Object (single) bound to NHG object. As we discussed, multiple ARS objects (profiles) can be defined and used e.g. for TCP and non-TCP traffic. Still a single ARS object is on NHG object

Contributor
Author
@JaiOCP JaiOCP 9 days ago
Route lookup will result in a NHG with ARS object. This is correct. There is 1:1 relationship between NHG and ARS object.

Following two ways can be used to assign an ARS object for a specific flow type.

When a flow resolves via NHG with ARS object.
e.g. match action <SAI_ACL_ACTION_TYPE_DISABLE_ARS_FORWARDING>
In this case TCP flows resolving via NHG with ARS object will go through normal NHG object processing. This rule can be used as a catch all rule for all TCP traffic type, even if NHG do not have any ARS object present and will be noop in this case.

When a flow resolves via NHG without ARS object
e.g. match action < SAI_ACL_ACTION_TYPE_SET_ARS_OBJECT>
In this case UDP flows will undergo ARS object processing.
Note: (2) can be used to override ARS object as present in NHG as well. In this case ARS object OID as provided by the ACL rule will be used instead of NHG provided ARS OID.

#### 12.2 How to setup NHG for ARS

eddyk-nvidia 9 days ago
Probably the question for SONIC folks. What is the trigger for binding the ARS object to NHG ? How is the ARS object chosen (multiple objects/profiles) can be created

#### 12.3 Port not enabled for ARS ins parth ARS aenabled NHG

marian-pritsak 9 days ago
Can a port RIF with disabled ARS be part of the next hop group with ARS enabled? Or is this attribute only related to LAG?

Contributor
Author
@JaiOCP JaiOCP yesterday
Good practice is to enable all the ports for ARS participation and not worry about NHG resolution for a port that is not enabled for ARS.

Having said that if there is a an advantage to disable ARS for certain ports or port groups (like for power etc), and if NHG resolves via port not enabled for ARS, it will effectively not participate in the ARS group and will not get any traffic scheduled. This is easy to observe in a deployment as no traffic will be forwarded to the next hop pointing to this link.

#### 12.4 Q&A Capture

Adaptive Routing and Switching only was discussed:

5 -tuple -> micro flow
If the micro flows hashed to same bucket -> macro flow

Reason of normalizations - e.g. when there are ports of different speed, make it to a normalized value

Matrix computed based on work load, based on past or future load (amount of queue backup happening)

Utilization x queue build up -> each can have a weight

E.g. two links:

50% load, 50% queue build up

20% load, less than 50% queue build up

If load has higher weight, then link 2 is better, but if queue buildup has higher weight, then link 1 is better

Even though load is low, queue build up can still happen if traffic is bursty

Sampling rate can be configured to compute quality of link

Q: if port is already close to 100%, then should not look at queue buildup only
A: Yes, both should be looked at

Static configuration at link level - each link can have different weight

Can it be dynamic say, to change when port is higher load?
A: yes

Current SAI PR, it's global for all links
For more granularity, can be future enhancement

EWMA algorithm is supported, PR is extensible to add future algorithm

How to establish future load? Queue build up

Q: is it probability based?
A: no, absolute

BUM and Mirror Traffic are not covered

Deployment should be incremental -> acl based

Configure based on NG group, then flow level

Ethertype control , e.g. IPv4 only

Q: what about there is port-channel/LAG?
A: Covered in spec.  Two levels: L3 level (what's best path), LAG level (which port within LAG is best)

Q: how to do port load/queue build up at L3 level?
A: Defined in spec, but there's also Implementation details

Q:  if there's a two ECMP paths with two LAGs, what will make one LAG better than the other?
A: Same algorithm: port load  of all LAG ports, and queue build up of all LAG ports

Q: Does it need to flatten all ECMP LAG ports for multiple paths?
A: No, ecmp next-hop is picked first; each LAG is like a big port logically, eg. 200G for a 2x100G lag

Q: is link status checked?
A: yes, decision happens at hardware, not relying on control plane to update the next-hop; this avoids traffic blackholing; control plane no need to update a ecmp group even when one link is down

Q: what's the corresponding sai attribute for link status handling above?
A: no notification currently, but there is counter; also the port will be an ARS port ; can consider an explicit port attribute for this

Q: How about Weighted ECMP compatibility?
A: No, this feature is not compatible if ecmp path/bucket is weighted.
