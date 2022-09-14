# Deploy SAI PTF Test Topology With SONiC-MGMT

*In this article, you will get to know how to use the sonic-mgmt docker to set up the topology for sai testing.*

> **Those commands need to be run within a sonic-mgmt docker, or you need to run them within a similar environment.**
*This section of the document described how to build a sonic-mgmt docker*

https://github.com/Azure/sonic-mgmt/blob/master/docs/testbed/README.testbed.VsSetup.md#setup-sonic-mgmt-docker
1. install the sonic image in the DUT(device under test)
for example
```
SONiC Software Version: SONiC.20201231.08
```
2. remove the topology for the current testbed
```
./testbed-cli.sh remove-topo vms12-t0-s6000-1 password.txt
```
For understanding the topology concept, please refer to the doc
[Topologies](https://github.com/Azure/sonic-mgmt/blob/master/docs/testbed/README.testbed.Topology.md)

For how to find the topology info and the related device please refer to the doc
[Example of Testbed Configuration](https://github.com/Azure/sonic-mgmt/blob/master/docs/testbed/README.testbed.Example.Config.md)

For the example to set up the testbed with the related command please refer to 
- Contains how to build the related PTF, sonic-mgmt docker
[Testbed Setup](https://github.com/Azure/sonic-mgmt/blob/master/docs/testbed/README.testbed.Setup.md)
- More concentrate on a virtual environment with KVM and Docker
[KVM Testbed Setup](https://github.com/Azure/sonic-mgmt/blob/master/docs/testbed/README.testbed.VsSetup.md)
3. Change the topology to 'PTF' by modifying the testbed.yml
In order to get the default configuration without any other noise in the testing, like some interface configured as down, and vlan might be set up, we need to use a non-tology for testing. 
For example, we want to use the config `vms-sn2700-t1-lag`, then we need to change it
```git
 - conf-name: vms-sn2700-t1
   group-name: vms1-1
-  topo: t1
+  topo: ptf32
   ptf_image_name: docker-ptf-sai-mlnx
-  ptf: ptf-unknown
+  ptf: ptf-docker-name
   ptf_ip: 10.255.0.178/24
   ptf_ipv6:
   server: server_1
```
> **for the topo, if it ends with 64, then the topo should be ptf64, please change it according to the actual device port.**

4. deploy the new topology
```
./testbed-cli.sh -t testbed.yaml add-topo vms-sn2700-t1 password.txt
```
> **You can change the testbed filename from the testbed.yaml if needed, and the current config name is  vms-sn2700-t1**
5. push the minigraph to dut
```
./testbed-cli.sh -t testbed.yaml deploy-mg vms-sn2700-t1 str password.txt
```