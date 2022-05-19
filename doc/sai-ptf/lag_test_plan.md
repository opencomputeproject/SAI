# SAI Lag Test plan
- [SAI Lag Test plan](#sai-lag-test-plan)
  - [Overriew](#overriew)
  - [Scope](#scope)
  - [Lag SAI APIs](#lag-sai-apis)
    - [Create and Remove](#create-and-remove)
  - [Funcationalities](#funcationalities)
    - [Loadbalanceing](#loadbalanceing)
## Overriew
The purpose of this test plan is to test VLAN function from SAI.
## Scope
The test will include two parts
1. Lag SAI APIs
2. Lag functionalities

## Lag SAI APIs
### Create and Remove
Sample APIs
Create member
```Python
sai_thrift_create_lag_member(
            self.client, lag_id=lag3, port_id=self.port24)
```
Remove member
```Python
sai_thrift_remove_lag_member(self.client, lag3_member24)
```
Counters
```Python
    counter_results["SAI_PORT_STAT_IF_IN_DISCARDS"],
    counter_results["SAI_PORT_STAT_IF_IN_DISCARDS"],
    counter_results["SAI_PORT_STAT_IF_IN_UCAST_PKTS"],
    counter_results["SAI_PORT_STAT_IF_OUT_UCAST_PKTS"]))
```
Ingress/Egreee disable
```python
    status = sai_thrift_set_lag_member_attribute(
        self.client,
        self.lag1_member4,
        ingress_disable=True,
        egress_disable=True)

```
lag port list
```Python
sai_thrift_get_lag_attribute(
                self.client, self.lag1, port_list=portlist)
```

## Funcationalities
### Loadbalanceing
For loadbalance, expecting the ports in a lag should received the packet equaly.
Even after remove and disable the port in a lag.
Sample APIS
Disbale
```Python
sai_thrift_set_lag_member_attribute(
                self.client, self.lag1_member4, egress_disable=True)
```
Remove
```Python
sai_thrift_remove_lag_member(self.client,  self.lag1_member6)
```