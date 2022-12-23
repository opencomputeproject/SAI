# SAI-PTF for Warm reboot

| **Title** | **SAI-PTF for Warm reboot** |
| --- | --- |
| **Authors** | **Richard Yu, Junyi Xiao** |
| **Status** | **In review** |
| **Created** | **22/03/2022** |
| **Modified** | **22/03/2022** |
| **SAI-Version** | **V1.7** |

- [SAI-PTF for Warm reboot](#sai-ptf-for-warm-reboot)
  - [Background for Warm reboot test](#background-for-warm-reboot-test)
  - [SAI warm reboot feature points](#sai-warm-reboot-feature-points)
    - [Containerized test environment](#containerized-test-environment)
    - [Expose SAI local API as RPC APIs](#expose-sai-local-api-as-rpc-apis)
    - [Open interface for test stage control (support manual or tools)](#open-interface-for-test-stage-control-support-manual-or-tools)
    - [One tag only to Upgrade existing test cases - @warm\_test](#one-tag-only-to-upgrade-existing-test-cases---warm_test)
    - [Automated whole process with SONiC-MGMT](#automated-whole-process-with-sonic-mgmt)
  - [Warm reboot on SAI-PTF Automatic structure](#warm-reboot-on-sai-ptf-automatic-structure)
    - [SAI PTF v2](#sai-ptf-v2)
      - [PTF](#ptf)
      - [DUT](#dut)
    - [SONiC-MGMT](#sonic-mgmt)
  - [Warm reboot workflow](#warm-reboot-workflow)
    - [dut-ptf](#dut-ptf)
    - [mgmt-dut](#mgmt-dut)
    - [mgmt-ptf](#mgmt-ptf)
  - [sample code \& example](#sample-code--example)
    - [dut-ptf](#dut-ptf-1)
    - [dut-mgmt](#dut-mgmt)
      - [Mounting of sai.profile](#mounting-of-saiprofile)
      - [Mounting of sai-warmboot.bin](#mounting-of-sai-warmbootbin)
      - [Prepare for first start](#prepare-for-first-start)
      - [Prepare for second start](#prepare-for-second-start)
      - [Restore after warmboot test](#restore-after-warmboot-test)
    - [ptf-mgmt](#ptf-mgmt)
  - [test result](#test-result)


## Background for Warm reboot test
For warm reboot test we need to meet following requirements
1. Preparing configurations on DUT(device under test) for warm reboot
2. Setup DUT for testing purpose, like deployment test purpose dockers, expose SAI API for testing
3. Controlling and monitoring DUT for different warm reboot stage
4. Controlling case running steps on different DUT status(prepare reboot, rebooting or post reboot)
5. Ability to check the DUT function during warm reboot(packat can forwarded)
6. Nice to be able to automate the whole process
7. Nice to be able to reuse all the functionality testcases

## SAI warm reboot feature points
### Containerized test environment
Lightweight docker which can be easily deployed in DUT to satisfy testing pexpose

### Expose SAI local API as RPC APIs
Lightweight docker contains the RPC server which can expose the SAI interface to invoke the SAI interface remotely

### Open interface for test stage control (support manual or tools)
Human readable file to control the cases running stage, which can be used by automaticatic tools or manual test

### One tag only to Upgrade existing test cases - @warm_test
Just need add ``@warm_test`` to enable the warm reboot tests, then you can use the output file to control the whole process for warm reboot.

### Automated whole process with SONiC-MGMT
SONiC-MGMT can control DUT side and control the whole Test process automatically, from test environment setup, warm reboot configurations, warm reboot process. 

## Warm reboot on SAI-PTF Automatic structure
SAI-PTF Automatic, we seperate the whole system into two components, they are
 - SAI PTF v2
 - SONiC MGMT


![warm_logic_connection](img/warm_logic_connection.png)

For more details please refer to doc [SAI-PTFv2 Overview](https://github.com/opencomputeproject/SAI/blob/master/ptf/docs/SAI-PTFv2Overview.md). 

Base on the existing SAI-PTFv2 structure, we upgrade the whole system for warm reboot test.
The new structure as below.

![architecture](img/warm_architecture.png)
For each part, the details as below.

### SAI PTF v2

#### PTF

 1. To make the test running process can be controlled outside, by tools or manually, create file `/tmp/warm_reboot` as an open interface to communicate with outside, like `sonic-mgmt`
 2. To reuse existing cases, we need to add tag to enable the wrapper for the runTest function. 
     - The warpper ``@warm_test`` contains the process of warm-reboot.
more detail see [ptf-mgmt](#ptf-mgmt)
    ```python
    def setUp(self): # make settings before reboot
    @warm_test(is_test_rebooting=True) # does test run at rebooting
    def runTest(self): # run test 
    def tearDown(self): # remove the setting and clear the test environment
    ```
  3. In order to trigger a warm reboot test, we use warm reboot API to make a warm reboot
  
#### DUT

In DUT, in order to support warm reboot, we make the following changes
  1. sai-warmboot.bin: Save the configuration of dut to sai-warmboot.bin
  2. sai.profile: Decide how to start `saiserver`
 
### SONiC-MGMT

SONiC-MGMT component, It mainly has the following functions

1. SONiC testbed deployment and setup
2. pull `SAI` to the ptf ， the script about `saiserver` is pulled to dut
3. start `saiserver` contatiner
4. check whether can connect rpc server in `saiserver`
5. prf test running
6. stop `saiserver` container
7. organize and upload result
8. teardown
Each SAI test case will repeat 3-6 steps. Wait until all the cases in the caselist are executed, and then go to the 7th step.

For warm-reboot, we made following upgrade
1. Set warm reboot configurations: setup the files in [DUT Upgrade](#DUT-Upgrade)
2. creat Warmboot-Watcher daemon: check the Open Interface [PTF-Upgrade](#PTF-Upgrade)
3. Coordinate DUT and PTF: Monitor status DUT status in step1 and test status in step2

> Note, SONiC-MGMT is a coordanitor between DUT and PTF, we can manually manipulate the status file and control the startup and shutdown of `saiserver` or use other automatic tools/script for that, we can also manually modify .

## Warm reboot workflow
The entire automated system for testing SAI in the warmboot scenario includes three parts(PTF,DUT,sonic-mgmt), and they are closely related. Next I will introduce the communication between the modules.  

The sequence graph among dut,ptf and sonic-mgmt is as follows
![sequence](img/sequence.png)
### dut-ptf
 Warm shut down `saiserver` automatically after the setUp method.
 In order not to affect the previous test, add a wrapper to handle the rebooing stage
### mgmt-dut
Mgmt modify the `\etc\sai.d\sai.profile` and control the startup and shutdown of `saiserver` by ansible.
1. Before starting `saiserver` for the first time, we will update `sai.profile`, save the configuration of dut in the setup function of the case, for the next warmreboot of saiserver.
2. Start `saiserver` container
3. After receiving the signal of closing container from dut, close `saiserver`
4. Before starting  `saiserver` for the second time, update `sai.profile` to set the startup mode to warm boot, and read the configuration from warmreboot.bin
5. Start `saiserver` container
6. when case is finished, close `saiserver` and  we will restore the settings of `sai.profile` to prepare for the next test.

When we create the `saiserver` container, we will mount the directory where the `sai.profile` and `sai-warmboot.bin` of dut are located to the `saiserver` container. Because `saiserver` will start according to `sai.profile`. So if we mount `sai.profile`, we can modify the configuration file before starting `saiserver` to prepare for startup.

For more about how `sai.profile` and `sai-warmboot.bin` mount, and how sonic-mgmt modifies sai.profile at different stages, we can see [mgmt-dut](#mgmt-dut)
### mgmt-ptf

Mgmt can remotely control ptf through anisble and execute shell commands. So we create a file `/tmp/warm_reboot` in ptf as shared memory for communication between mgmt and ptf.
`sonic-mgmt` finishes the following process in `start_warm_reboot_watcher`, `ptf` finishes the following process in wrapper `@warm_test(is_test_rebooting=True)`.
   
   1. ptf writes 'rebooting' to `/tmp/warmboot` to notify sonic-mgmt after setup and warm shutdown down
   2. ptf checkes if warmreboot is down in 1 mins
      1. if not, do something in rebooting stage
      2. if done, continue running the case in post-reboot stage
      3. if timeout, raise error
   3. sonic-mgmt restart `saiserver` once reading rebooting in `/tmp/warmboot`
   4. ptf writes 'post_reboot_done' to `/tmp/warmboot` to notify ptf that warmreboot is done.


## sample code & example

### dut-ptf
Before dut notifies mgmt to close saiserver for the first time，Warm shut down automatically.  
The code for making the warm shutdown is 

   ```python
  print("shutdown the swich in warm mode")
  sai_thrift_set_switch_attribute(self.client, restart_warm=True)
  sai_thrift_set_switch_attribute(self.client, pre_shutdown=True)
  sai_thrift_remove_switch(self.client)
  sai_thrift_api_uninitialize(self.client)
   ```
  
### dut-mgmt

#### Mounting of sai.profile
Path on the `saiserver`: `/etc/sai.d/sai.profile`  
Path on the dut host varies with different PLATFORMs and HWSKUs: it can be obtained through shell commands

```shell
# Obtain our platform as we will mount directories with these names in each dockers
PLATFORM=${PLATFORM:-`$SONIC_CFGGEN -H -v DEVICE_METADATA.localhost.platform`}
# Obtain our HWSKU as we will mount directories with these names in each docker
HWSKU=${HWSKU:-`$SONIC_CFGGEN -d -v 'DEVICE_METADATA["localhost"]["hwsku"]'`}
# The path to store sai.porfile
profile_path=/usr/share/sonic/device/$PLATFORM/$HWSKU
```

#### Mounting of sai-warmboot.bin
Path on the `saiserver`:`/var/warmboot`  
Path on the dut host:`/host/warmboot`

#### Prepare for first start
1.Save the initial `sai.profile` to `sai.profile.bak`，which is for restoring files after warm reboot.

```shell
profile='sai.profile'
cp $profile $profile.bak
-------------------------------
#Sample profile in brcm s6000
cat /etc/sai.d/sai.profile
SAI_INIT_CONFIG_FILE=/usr/share/sonic/hwsku/td2-s6000-32x40G.config.bcm
```

2.add the `WARM_REBOOT_WRITE_FILE` and `SAI_WARM_BOOT_READ_FILE` in the profile。

```shell
echo "SAI_WARM_BOOT_WRITE_FILE=/var/warmboot/sai-warmboot.bin" >> $profile
echo "SAI_WARM_BOOT_READ_FILE=/var/warmboot/sai-warmboot.bin" >> $profile
-------------------------------
#Sample profile in brcm s6000
cat /etc/sai.d/sai.profile
SAI_INIT_CONFIG_FILE=/usr/share/sonic/hwsku/td2-s6000-32x40G.config.bcm
SAI_NUM_ECMP_MEMBERS=32
SAI_WARM_BOOT_WRITE_FILE=/var/warmboot/sai-warmboot.bin
SAI_WARM_BOOT_READ_FILE=/var/warmboot/sai-warmboot.bin
```

`SAI_WARM_BOOT_WRITE_FILE` and `SAI_WARM_BOOT_READ_FILE` are used to define where SAI will save and load the data backup file.
The configuration data of dut in setup will be backed up to /var/warmboot/sai-warmboot.bin. So even after closing the saiserver container, the backup data will be saved in dut.
#### Prepare for second start
Enable the warm start
```shell
echo "SAI_BOOT_TYPE=1" >> $profile
-------------------------------
# Sample in a brcm s6000
cat /etc/sai.d/sai.profile
SAI_INIT_CONFIG_FILE=/usr/share/sonic/hwsku/td2-s6000-32x40G.config.bcm
SAI_NUM_ECMP_MEMBERS=32
SAI_WARM_BOOT_WRITE_FILE=/var/warmboot/sai-warmboot.bin
SAI_WARM_BOOT_READ_FILE=/var/warmboot/sai-warmboot.bin
SAI_BOOT_TYPE=1
```
#### Restore after warmboot test
```shell
cp $profile.bak $profile
-------------------------------
#Sample profile in brcm s6000
cat /etc/sai.d/sai.profile
SAI_INIT_CONFIG_FILE=/usr/share/sonic/hwsku/td2-s6000-32x40G.config.bcm
SAI_NUM_ECMP_MEMBERS=32
```
### ptf-mgmt

## test result