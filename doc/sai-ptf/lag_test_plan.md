# SAI Lag Test plan
- [SAI Lag Test plan](#sai-lag-test-plan)
- [Overriew](#overriew)
- [Test Environment](#test-environment)
  - [Testbed](#testbed)
  - [Test Configuration](#test-configuration)
  - [Variations](#variations)
- [Test Execution](#test-execution)
  - [Basic function test](#basic-function-test)
    - [PortChannel Loadbalanceing](#portchannel-loadbalanceing)
      - [Testing Objective](#testing-objective)
      - [Test Data/Packet](#test-datapacket)
      - [Test Cases](#test-cases)
  - [SAI API test](#sai-api-test)
    - [Renove lag member](#remove-lag-member)
      - [Testing Objective](#testing-objective)
      - [Test Cases](#test-cases)
    - [Disable Lag memeber](#disable-lag-memeber.)
      - [Testing Objective](#testing-objective)
      - [Test Data/Packet](#test-datapacket)
      - [Test Cases](#test-case)
      
## Overriew
The purpose of this test plan is to test the LAG/PortChannel function from SAI.

# Test Environment
## Testbed
Those tests will be run on the testbed structure, the components are:
* PTF - running in a server that can connect to the target DUT
* SAI server - running on a dut
## Test Configuration

For the test configuration, please refer to the file 
  - [VLAN_config](./config_data/vlan_config_t0.md)
  - [FDB_config](./config_data/fdb_config_t0.md)
  - [Route_config](./config_data/route_config_t0.md)
  - [LAG_config](./config_data/LAG_config_t0.md)

  
**Note. All the tests will be based on the configuration above, if any additional configuration is required, it will be specified in the Test case.**

## Variations
Cause the testbed might also encounter some issues like the host interface being down. 
Before running the actual test there will need some sanity test to check the DUT status and select the active ports for testing.

**All the ports in this test plan just to illustrate the test purpose, they are not exactly the same for the actual environment.**

# Test Execution
## Basic function test

### PortChannel Loadbalanceing
#### Testing Objective
For load balancing, expecting the ports in a lag should receive the packet equally. Traffic direction: from server side to T1 side
Even after removing and disabling the port in a lag.

#### Test Data/Packet

[Sample_Packet](./config_data/LAG_config_t0.md#sample-datapacket)
- Input Packet
- Output Packet

#### Test Cases
|  Goal| Steps/Cases  | Expect  |
|-|-|-|
| Prepare to send from port0 to Lag1.| Send packet with.| Lag1 and members have been created.|
| Packet forwards on port equally.| Send packet on port0 to the lag1  100 times .| Loadbalance on lag members.|
| Packet forwards on available ports equally.| Every time, disable egress/ingress on one lag member, then send packet | Loadbalance on lag members.|
| Packet forwards on available ports equally.| Every time, enable egress/ingress on one lag member, then send packet | Loadbalance on lag members.|
| Packet forwards on available ports equally.| Every time, remove one lag member, then send packet | Loadbalance on lag members.|



## SAI API test
### Remove Lag member 
#### Testing Objective
Test verifies the LAG load balancing for scenario when LAG members are removed.


#### Test Data/Packet

[Sample_Packet](./config_data/LAG_config_t0.md#sample-datapacket)
- Input Packet
- Output Packet

#### Test Cases
| Goal | Steps/Cases | Expect  |
|-|-|-|
|Remove lag2_member2 and forwarding packet from port0 to lag2|Remove lag2_member2 form Lag2 and Send packet on port0 to lag2 100 times| Port0 will receive an equal number of packets.|


### Disable Lag memeber
For lag, we can disable it from ingress or egress direction, after we disable the member of a lag, we expect traffic can be loadbalanced to other lag members.

#### Test Data/Packet

[Sample_Packet](./config_data/LAG_config_t0.md#sample-datapacket)
- Input Packet
- Output Packet

#### Test Cases
| Goal | Steps/Cases | Expect  |
|-|-|-|
|Packet dropped on port22| Disable egress and ingress on lag3 member2. send packet | Packet drop.|
