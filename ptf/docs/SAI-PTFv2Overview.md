# SAI-PTFv2 Overview
*This article will introduce SAI-PTFv2 and the detailed process of the entire SAI-PTFv2 testing.*

- [SAI-PTFv2 Overview](#sai-ptfv2-overview)
  - [SAI PTF v2 introduction](#sai-ptf-v2-introduction)
    - [Logical Topology](#logical-topology)
    - [Physical Connection](#physical-connection)
      - [Key components/devices in the physical connection:](#key-componentsdevices-in-the-physical-connection)
  - [Setup Testbed](#setup-testbed)
    - [Download SAI-PTFv2 components](#download-sai-ptfv2-components)
    - [Build SAI-PTFv2 components](#build-sai-ptfv2-components)
      - [Code for Build SAI-PTFv2 components](#code-for-build-sai-ptfv2-components)
    - [Setup the testbed by sonic-mgmt](#setup-the-testbed-by-sonic-mgmt)
    - [Setup DUT (Device under testing)](#setup-dut-device-under-testing)
    - [Setup ptf-sai docker](#setup-ptf-sai-docker)
    - [Run test](#run-test)
      - [cases in: test/sai\_test vs ptf](#cases-in-testsai_test-vs-ptf)
      - [Parameters](#parameters)
  - [Reference](#reference)
    - [SAI PTF introduction and manually setup Testbed](#sai-ptf-introduction-and-manually-setup-testbed)
    - [sonic-buildimage user guide](#sonic-buildimage-user-guide)
    - [Deploy SAI Test Topology With SONiC-MGMT](#deploy-sai-test-topology-with-sonic-mgmt)
    - [Setup Docker builder for debugging](#setup-docker-builder-for-debugging)
    - [SAI-PTF\_resources\_download](#sai-ptf_resources_download)



## SAI PTF v2 introduction
*In this part, we will get to know what's the SAI-PTF v2 framework.*

SAI-PTF v2 is upgraded from the previous [SAI-PTF fremework](../../test/saithrift/README.md).
SAI PTFv2 has two parts, [PTF (Packet Test Framework)](https://github.com/p4lang/ptf) and [SAI RPC framework](../../meta/rpc/README.md).

### Logical Topology
![Logical topology](./img/logic_connection.png#=120x120)

In the chart above the components are:
- PTF container - run test cases, and use an RPC client to invoke the SAI interfaces on DUT
- SAI Server container - run inside DUT/switch, which exposes the SAI SDK APIs from the libsai


### Physical Connection
Below is the [Physical connection of testbed](./ExamplePhysicalConnection.md):
![Physical connection](./img/physical_connection.png)

#### Key components/devices in the physical connection:

* Test servers
* Fanout switches
  * Root fanout switch (optional)
  * Leaf fanout switch
* SONiC DUT

Key aspects of the physical connection:

1. Every DUT port is connected to one of the leaf fanout switches.
2. Every leaf fanout switch has a unique VLAN tag for every DUT port.
3. Root fanout switch connects leaf fanout switches and test servers using 802.1Q trunks. *The root fanout switch is not mandatory if there is only one testbed or a test server is only used by one testbed. In this case, the leaf fanout switch can be directly connected with NIC of the test server by 802.1Q trunk.*
4. Any test server can access any DUT port by sending a packet with the port VLAN tag (The root fanout switch should have this VLAN number enabled on the server trunk)

Device and equipment needed in this doc
> Note: The device information below is just an example. They are not mandatory. Please make adjustments according to your actual environment.
- Test Servers: `dev-acs-serv-01`
- Fanout Switches
  - Root Fanout Switch (optional): `dev-7260-11` HwSku:`Arista-7260QX-64`
  - Leaf Fanout Switch: `dev-7260-16` HwSku: `Arista-7260QX-64`
- SONiC DUT: `str-s6000-acs-10` HwSku: `Force10-S6000`


## Setup Testbed
*In this part, we will build SAI-PTFv2 infras using sonic-buildimage.*

 > Note: We use sonic-buildimage and sonic-mgmt to automatically set up the testbed by some script. Please refer to [SAI PTF introduction and manually setup Testbed](ManuallySetupTestbedGuide.md) for setup if manually.

 > For more introduction about how to use the sonic-buildimage, please refer to [sonic-buildimage user guide](https://github.com/Azure/sonic-buildimage)

In the sections below, we will see how to get the sai-related resources.

### Download SAI-PTFv2 components
For test-related resources, we already enable pipeline build for some ASICs and on some versions. 

Please refer to this page for the links on support ASICs and versions. [SAI-PTF_resources_download](SAI-PTF_resources_download.md)

### Build SAI-PTFv2 components
*In this section, we will get the test-related resources.*

If you need those test-related resources on some other ASICs or Versions that have not been built by pipeline, you can also build them manually.

In the following, we use other SONiC scripts to help setup the [SAI PTF topology](https://github.com/sonic-net/sonic-mgmt/blob/master/docs/testbed/README.testbed.Overview.md#ptf-type-topology) environment with all the testing components mentioned in [SAI PTF introduction and manually setup Testbed](ManuallySetupTestbedGuide.md).

For SAI-PTFv2 test, we can build the components:
1. Docker PTF, which contains all the runtime dependencies
2. Docker SAIServerv2, which contains the RPC Server, SAI SDK, and all the running dependences
3. python-saithrift, the RPC client, this binary will be generated when building the docker saiserverv2

> Note: SAIServer (RPC Server) and python-saithrift (RPC client) must be built from the same SAI version, when deploying your environment, please make sure they are from the same version.

Precondition:

Before starting the build process, please make sure you get the right sonic build image branch.
For how to check the sai header version and sonic branch from a certain sonic image please refer to
[Check SAI Header Version And SONiC Branch](CheckVersion.md)

> Note: the example below is based on the sonic 202205 branch

1. Checkout code in sonic-buildimage repo
    > please check the branch and commit id previously checked   

    ```
    git clone https://github.com/Azure/sonic-buildimage.git
    cd sonic-buildimage

    git checkout <specific branch>
    ```
    Here we use the 202205 branch for example:
    ```
    git clone https://github.com/Azure/sonic-buildimage.git
    cd sonic-buildimage

    git checkout 202205    
    ```

2. Build SAI-PTFv2 infras 

    - build saiserverv2
        ```
        # Init env
        make init
        # BLDENV=bullseye: Current image is the bullseye
        # PLATFORM=<vendor name> Setup platform environment e.g. Broadcom
        NOSTRETCH=y NOJESSIE=y NOBUSTER=y make configure PLATFORM=broadcom

        # SAITHRIFT_V2=y: build the saiserver version 2nd
        # build brcm saiserverv2 docker 
        NOSTRETCH=y NOJESSIE=y NOBUSTER=y SAITHRIFT_V2=y make target/docker-saiserverv2-brcm.gz
        ```

    - Download Prepared docker ptf-sai from [here](https://sonicstorage.blob.core.windows.net/public/sai/ptf-sai/master/20220919/docker-ptf-sai.gz?sv=2020-08-04&st=2022-09-19T10%3A42%3A43Z&se=2037-09-20T10%3A42%3A00Z&sr=b&sp=r&sig=4bJVXu4LsFMV2whX3VUxz0TBwnWgMKgNzOQMQSIdZJg%3D)



3. Locate the generated binary and dockers

    - docker saiserverv2 at `<local_folder>/sonic-buildimage/target/docker-saiserverv2-brcm.gz`
    - docker ptf-sai from [Download](https://sonicstorage.blob.core.windows.net/public/sai/ptf-sai/master/20220919/docker-ptf-sai.gz?sv=2020-08-04&st=2022-09-20T11%3A36%3A23Z&se=2037-09-21T11%3A36%3A00Z&sr=b&sp=r&sig=e2PjlExxekVY%2FniU3g3ED7lE2BfWkz9b3tudf5b22jE%3D), please move to `<local_folder>/sonic-buildimage/target/docker-ptf-sai.gz`
    - python_saithrift at `<local_folder>/target/debs/bullseye/python-saithriftv2_0.9.4_amd64.deb`

        > Note: for different platform (BLDENV=buster), the output folder might be different, i.e. BLDENV=bullseye, it will be <local_folder>/target/debs/bullseye

Now, you have built all PFT-SAIv2 components: docker saiserverv2, docker ptf-sai, and python_saithrift. The next step is to upload those dockers, i.e. tag and push them to your local available docker registry. After this, you can pull those dockers in the DUT with our script. 

#### Code for Build SAI-PTFv2 components
In cases, you want to enable (Build SAI-PTFv2 components)[#code-for-build-sai-ptfv2-components] on a platform or versions from scratch, where doesn't have docker-saiserver at all.

Please refer to this PR ([SAI-PTF][BFN]Enable saiserver test container on bfn platform)[https://github.com/sonic-net/sonic-buildimage/pull/13166] and make the similar and necessary changes to add the build script and pipeline build.

### Setup the testbed by sonic-mgmt

*In this section, we will set up the physical switch testbed by sonic-mgmt.*

- Precondition:

    Install the sonic image in the DUT, as for how to install a sonic image on the supported switch, please refer to this doc [Install sonic eos image](https://github.com/Azure/SONiC/wiki/Quick-Start#install-sonic-eos-image).
You have a local docker registry that can be used to push and pull dockers.

1. Check the SONiC OS version
   
   In order to use the script to prepare the environment, we need to get the right SONiC OS version for pushing the saiserver docker.
    ```
    # In a SONiC OS
    ~$ show version

    SONiC Software Version: SONiC.20201231.76
    ......

    Docker images:
    REPOSITORY                                                 TAG                 IMAGE ID            SIZE
    acs-repo.corp.microsoft.com:5001/docker-saiserverv2-brcm   20201231.76         32e9d2f6c269        751MB
    docker-saiserverv2-brcm                                    latest              32e9d2f6c269        751MB
    acs-repo.corp.microsoft.com:5001/docker-syncd-brcm-rpc     20201231.76         fc97962d4b2a        983MB
    ```
    **Here, the 20201231.76 is the OS_VERSION**
2. Upload the build-out dockers 

    We need to upload those dockers to your local available docker registry:
    - docker saiserverv2 at `<local_folder>/sonic-buildimage/target/docker-saiserverv2-brcm.gz`
    - docker ptf-sai at `<local_folder>/sonic-buildimage/target/docker-ptf-sai.gz`
    ```
    docker load -i <local_folder>/sonic-buildimage/target/docker-saiserverv2-brcm.gz
    docker load -i <local_folder>/sonic-buildimage/target/docker-ptf-sai.gz

    # tag docker
    docker tag docker-saiserverv2-brcm:latest <docker-registry-address>/docker-saiserverv2-brcm:<TAG_WITH_OS_VERSION>
    docker tag docker-ptf-sai <docker-registry-address>/docker-SAI-PTFv2

    docker push <docker-registry-address>/docker-saiserverv2-brcm:<TAG_WITH_OS_VERSION>
    docker push <docker-registry-address>/docker-SAI-PTFv2
    ```
    > For the setup of ptf-sai docker, it is similar to this section [Setup Docker Registry for docker-ptf](https://github.com/Azure/sonic-mgmt/blob/master/docs/testbed/README.testbed.Setup.md#setup-docker-registry-for-docker-ptf). But here, we need to use `docker-SAI-PTFv2`.

    > Make sure you upload the docker-saiserverv2-brcm with the right TAG_WITH_OS_VERSION, it is critical for the latter dut setup script.

    ```
    # From step 1 the docker push command could be
    docker push <docker-registry-address>/docker-saiserverv2-brcm:20201231.76
    ```

3. Add docker registry for sonic-mgmt

    sonic-mgmt will try to pull the dependent ptf docker during the deployment process, we need to use the local docker registry here:

    ```
    # Edit file <local_folder>/sonic-mgmt/ansible/vars/docker_registry.yml with your local docker reigstry
    docker_registry_host: <docker-reigstry>:<port>
    ```

4. Deploy SAI Test Topology With SONiC-MGMT
    
    For the detailed steps please refer to [Deploy SAI Test Topology With SONiC-MGMT](DeploySAITestTopologyWithSONiC-MGMT.md)

    After setup the SONiC-MGMT env, we can use the command to set up the topology.

    ```
    cd /data/<sonic-mgmt-clone>/ansible
    ./testbed-cli.sh -t testbed.yaml add-topo <config_name> ../password.txt
    ```

### Setup DUT (Device under testing)
*In this section, we will introduce how to setup the saiserverv2 docker in DUT.*

> We prepared some scripts to help setup the DUT, you can change some of the scripts as needed

1. Install the script for the setup DUT

    ```
    # clone the sonic-misc to your local environment
    git clone https://github.com/richardyu-ms/sonic-misc

    # scp the subfolder DUTScript to your dut
    cd sonic-misc/sonic-scripts
    scp -r ./DUTScript admin@<DUT_IP>:~/
    ```

> below steps are under DUT

2. Go into folder DUTScript
   
   ```
   cd DUTScript
   ```

3. Log in to the docker of your local repo and make sure it is accessible
    ```
    docker login <local_docker_reg>
    ```

4. Pull saiserverv docker 

    Change the docker registry in `<sonic-misc>/DUTScript/pull_saiserver_syncd_rpc_dockers.sh`

    ```
    #SONIC_REG=<local_docker_reg>
    ```

    Run command to pull docker based on OS version

    ```
    # pull docker from docker registry
    # for example, it will pull prepared 
    # docker pull <docker-registry-address>/docker-saiserverv2-brcm:20201231.76
    # v1 for saiserver v1, it will pull saiserver and syncd-rpc dockers
    ./pull_saiserver_syncd_rpc_dockers.sh -v v2  
    ```

    > Make sure you pushed docker correctly.

5. Prepare saiserver 
   
    ```
    sudo ./prepare_saiserver_service.sh -v v2 
    ```
    Here is the output

    ```
    make folder ......
    copy_syncd_files
    change_scripts
    comment out functions and variables
    change saiserver version to v2
    Start saiserver service
            sudo systemctl start saiserver
    Start sai server manually, and run inside saiserver container with:
            /usr/bin/start.sh
            /usr/sbin/saiserver -p /etc/sai.d/sai.profile -f /usr/share/sonic/hwsku/port_config.ini
    ```

6. Stop all listeners which are used for docker recovery
   
    ```
    sudo ./all_listener.sh -o stop
    ```

7. Stop all the other docker services

    ```
    sudo ./all_service.sh -o stop
    ```

8. Start saiserver

    ```
    sudo systemctl start saiserver
    ```

Right here saiserver should be started, you can check it by

    
    # check the saiserver process
    docker exec -it saiserver ps -a
    # output
    PID TTY          TIME CMD
        11 pts/0    00:00:01 rsyslogd
    714 pts/9    00:03:11 saiserver
    

### Setup ptf-sai docker 
*In the last section, we will set up our testing environment and run a sanity test on PTF side.*

1. Log in to the ptf-sai docker, you can find the IP address of docker which is connected to the DUT in [testbed.yaml](https://github.com/Azure/sonic-mgmt/blob/master/ansible/testbed.yaml).

    ```
    ssh root@<PTF_IP>
    ```

2. Make sure Github is accessible on ptf-sai docker and download the SAI repo which contains SAI-PTFv2 test cases

    ```
    cd <PTF_Folder>
    git clone https://github.com/opencomputeproject/SAI.git
    cd SAI
    git checkout master
    ```

3. Install the sai python header `python-saithriftv2_0.9.4_amd64.deb` into ptf-sai docker.

    ```
    # install the deb package into ptf-sai docker
    dpkg -i python-saithriftv2_0.9.4_amd64.deb          
    ```

### Run test

Start SAI-PTFv2 testing within ptf-sai docker
```shell
        
# set the platform name
export PLATFORM=<vendor name>

# run a sanitytest
ptf --test-dir ptf saisanity.L2SanityTest --interface '<Port_index@eth_name>' -t "thrift_server='<DUT ip address>'"

ptf --test-dir ptf saisanity.L2SanityTest --interface '<Port_index@eth_name>' -t "thrift_server='<DUT ip address>';port_config_ini='<port_ini path>';config_db_json='<config_db.json path>'"

# use a Broadcom switch with 32-port as an example 

export PLATFORM=brcm
export DUTIP=<DUT_IP>
ptf --test-dir /tmp/SAI/ptf saisanity.L2SanityTest --interface '0@eth0' --interface '1@eth1' --interface '2@eth2' --interface '3@eth3' --interface '4@eth4' --interface '5@eth5' --interface '6@eth6' --interface '7@eth7' --interface '8@eth8' --interface '9@eth9' --interface '10@eth10' --interface '11@eth11' --interface '12@eth12' --interface '13@eth13' --interface '14@eth14' --interface '15@eth15' --interface '16@eth16' --interface '17@eth17' --interface '18@eth18' --interface '19@eth19' --interface '20@eth20' --interface '21@eth21' --interface '22@eth22' --interface '23@eth23' --interface '24@eth24' --interface '25@eth25' --interface '26@eth26' --interface '27@eth27' --interface '28@eth28' --interface '29@eth29' --interface '30@eth30' --interface '31@eth31' "--test-params=thrift_server=$DUTIP"

# or

# test/sai_test test set
ptf --test-dir test/sai_test sai_fdb_test.L2PortForwardingTest --relax --xunit --xunit-dir "/tmp/sai_qualify/test_results_tmp" --interface '0-0@eth0' --interface '0-1@eth1' --interface '0-2@eth2' --interface '0-3@eth3' --interface '0-4@eth4' --interface '0-5@eth5' --interface '0-6@eth6' --interface '0-7@eth7' --interface '0-8@eth8' --interface '0-9@eth9' --interface '0-10@eth10' --interface '0-11@eth11' --interface '0-12@eth12' --interface '0-13@eth13' --interface '0-14@eth14' --interface '0-15@eth15' --interface '0-16@eth16' --interface '0-17@eth17' --interface '0-18@eth18' --interface '0-19@eth19' --interface '0-20@eth20' --interface '0-21@eth21' --interface '0-22@eth22' --interface '0-23@eth23' --interface '0-24@eth24' --interface '0-25@eth25' --interface '0-26@eth26' --interface '0-27@eth27' --interface '0-28@eth28' --interface '0-29@eth29' --interface '0-30@eth30' --interface '0-31@eth31' --relax "--test-params=thrift_server='$DUTIP';port_config_ini='/tmp/sai_qualify/sai_test/resources/port_config.ini';config_db_json='/tmp/sai_qualify/sai_test/resources/config_db.json'"

# ptf test set
ptf --test-dir /tmp/SAI/ptf saisanity.L2SanityTest --interface '0@eth0' --interface '1@eth1' --interface '2@eth2' --interface '3@eth3' --interface '4@eth4' --interface '5@eth5' --interface '6@eth6' --interface '7@eth7' --interface '8@eth8' --interface '9@eth9' --interface '10@eth10' --interface '11@eth11' --interface '12@eth12' --interface '13@eth13' --interface '14@eth14' --interface '15@eth15' --interface '16@eth16' --interface '17@eth17' --interface '18@eth18' --interface '19@eth19' --interface '20@eth20' --interface '21@eth21' --interface '22@eth22' --interface '23@eth23' --interface '24@eth24' --interface '25@eth25' --interface '26@eth26' --interface '27@eth27' --interface '28@eth28' --interface '29@eth29' --interface '30@eth30' --interface '31@eth31' "--test-params=thrift_server=$DUTIP;port_config_ini='/tmp/sai_qualify/sai_test/resources/port_config.ini';config_db_json='/tmp/sai_qualify/sai_test/resources/config_db.json'"
```  
    

> Note: The hardware information for the testing device as below. Please make adjustments according to your actual device.

```
Platform: x86_64-dell_s6000_s1220-r0
HwSKU: Force10-S6000
ASIC: broadcom
ASIC Count: 1
Serial Number: DQBRX42
Uptime: 07:30:52 up  2:12,  3 users,  load average: 2.06, 1.93, 1.84
```
#### cases in: test/sai_test vs ptf
There are two test sets, they are under test/sai_test or under PTF folder.

- ptf
Test cases under ``ptf`` folder are cases more concentrated on API functionality testing

- test/sai_test
Test cases under ``ptf`` folder are cases more concentrated on some certain scenario, like T0 environment, so in this case set, there is a common config for simulating the business environment.

#### Parameters
Specification for parameter ``--interface '<Port_index@eth_name>'``
- Port_index
```shell
# check local interfaces
> ifconfig
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 9216
...

eth1: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 9216
...
```

- eth_name
```python
# eth0, eth1 ... are the eth_name
# Port_index will be used in test cases like
# If we have parameter 1@eth1, then 1 will map to eth1
send_packet(self, 1, pkt)
```

- config_db_json

This file contains the port configurations, like ``fec`` and ``mtu``.

By default, if this parameter has not been set, it will use the file under the resource folder, here is a sample of the config file  [config_db.json](https://github.com/opencomputeproject/SAI/tree/master/ptf/resources/config_db.json)
> Note, by convenience, we can copy the config_db.json from the sonic device to the PTF testing environment, but it is not mandatory, you can change it and add other definitions as needed.

Below is an example
```
{
    "_comment": "This is a sample config for port configurations",
    "PORT": {
        "Ethernet0": {
            "admin_status": "up",
            "fec": "rs",
            "mtu": "9100",
            "speed": "100000"
        },
        "Ethernet4": {
            "admin_status": "up",
            "fec": "rs",
            "mtu": "9100",
            "speed": "100000"
        },
        "Ethernet8": {
            "admin_status": "up",
            "fec": "rs",
            "mtu": "9100",
            "speed": "100000"
        },
...
        "Ethernet124": {
            "admin_status": "up",
            "fec": "rs",
            "mtu": "9100",
            "speed": "100000"
        }
    }
}
```

- port_config_ini

This file contains the Port setup information with the host interface, alias, and lanes.

For example, When creating a port and bridge port, it will read the lanes and use it as port information to map the host interface and create a bridge port in order.

By default, if this parameter has not been set, it will use the file under the resource folder, here is a sample of the config file  [port_config.ini](https://github.com/opencomputeproject/SAI/tree/master/ptf/resources/port_config.ini)


Below is an example
```
# name           lanes                alias      speed       index
Ethernet0        65,66,67,68          etp1       100000      1
Ethernet4        69,70,71,72          etp2       100000      2
Ethernet8        73,74,75,76          etp3       100000      3
Ethernet12       77,78,79,80          etp4       100000      4
...
Ethernet112      113,114,115,116      etp29      100000      29
Ethernet116      117,118,119,120      etp30      100000      30
Ethernet120      121,122,123,124      etp31      100000      31
Ethernet124      125,126,127,128      etp32      100000      32
```

Finally, we can see the result as shown below:

```
Using packet manipulation module: ptf.packet_scapy 

saisanity.L2SanityTest ... Waiting for the switch to get ready, 5 seconds ... 

...

Check port31 forwarding... 

ok 

---------------------------------------------------------------------- 

Ran 1 test in 21.184s 
 

OK 
```

## Reference

### [SAI PTF introduction and manually setup Testbed](ManuallySetupTestbedGuide.md)
### [sonic-buildimage user guide](https://github.com/Azure/sonic-buildimage)
### [Deploy SAI Test Topology With SONiC-MGMT](DeploySAITestTopologyWithSONiC-MGMT.md)
### [Setup Docker builder for debugging](SetupDockerBuilderForDebugging.md)
### [SAI-PTF_resources_download](SAI-PTF_resources_download.md)
