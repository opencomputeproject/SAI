# SAI-PTF for Warm reboot

| **Title** | **SAI-PTF for Warm reboot** |
| --- | --- |
| **Authors** | **Microsoft** |
| **Status** | **In review** |
| **Type** | **Richard Yu** |
| **Created** | **22/03/2022** |
| **Modified** | **22/03/2022** |
| **SAI-Version** | **V1.7** |

## Overriew

### SAI-PTF for Warm reboot
In order to use the SAI-PTF structure to verify the functionality in warm reboot scraniro, we need to add the following feature into the SAI-PTF structure
1. Light weight docker which can expose the sai interface to invoke the sai interface remotely
1. PTF test case can support differernt DUT running status with its different process - setup, runTest and teardown
1. one test case can run in different DUT running status - before restart, starting and started
1. Reuse already existing functional test cases and re-organize them into warm reboot structure

## Saiserver container and test structure
In order to test SAI interfaces, we need a light weight docker container which can help expose the SAI interfaces for a remote invocation.
Test bed structure as below.
### Test bed
Those test will be run on the test bed structure as below, the components are:
* PTF - running in a server which can connect to the target DUT
* SAI server - running on a dut
   ![Device_topology](img/Device_topology.jpg)
*p.s. cause the sai testing will not depends on any sonic components, then there will be no specifical topology(T0 T1 T2) for testing.*
### Test Structure
---
![Components](img/Component_topology.jpg)
Test structure the chart above, components are:
* PTF container - run test cases, and use a RPC client to invoke the SAI interfaces on DUT
* SAI Server container - run inside DUT/switch, which exposes the SAI SDK APIs from the libsai
* SAI-Qualify - Test controller, which is used to deploy and control the test running, meanwhile, manipulate the DUT on warm reboot.

*For how to start a saiserver container please check the doc at
[PTF-SAIv2 testing guide](https://github.com/Azure/sonic-mgmt/blob/master/docs/testbed/sai_quality/PTF-SAIv2TestingGuide.md) and 
[Example: Start SaiServer Docker In DUT](https://github.com/Azure/sonic-mgmt/blob/master/docs/testbed/sai_quality/ExampleStartSaiServerDockerInDUT.md)*


## PTF-SAI test supports DUT differernt running stage during warm reboot
For PTF-SAI structure, it use three different method for three different steps in test
- setUp, make settings
- runTest, run test
- tearDown, remove the setting and clear the test environment

During a warm reboot, we need to split those three method in PTF-Test into three different stage
- Before start, three methods
  - setup, runTest, tearDown 
- Starting, three methods
  - setUp_starting, test_starting, tearDown_starting
- After Starting, three methods.
  - setUp_post_start, test_post_start, tearDown_post_start

All those method extended by tag
```
@warm_setup
@warm_test
@warm_teardown
```
there is the sample code
- Setup methods
   ```python
    @warm_setup
    def setUp(self):
        print("setUp WarmL2SanityTest")
        L2SanityTest.setUp(self)


    def setUp_starting(self):
        print("setUp_starting WarmL2SanityTest")
        SaiHelperBase.setUp(self)
        super().param_init()


    def setUp_post_start(self):
        print("setUp_post_start WarmL2SanityTest")
        SaiHelperBase.setUp(self)
        super().param_init()
   ```
- runTest methods
   ```python
    @warm_test
    def runTest(self):
        print("Run test WarmL2SanityTest")
        super().runTest()


    def test_starting(self):
        print("test_starting WarmL2SanityTest")
        super().runTest()


    def test_post_start(self):
        print("test_post_start WarmL2SanityTest")
        super().runTest()   
   ```
- tearDown Methods
   ```python
    @warm_teardown
    def tearDown(self):
        print("tearDown WarmL2SanityTest")
        print("Skip the teardown and make a warm shut down for warm boot testing")
        self.warm_shutdown()


    def tearDown_starting(self):
        print("tearDown_starting WarmL2SanityTest")
        print("Skip the teardown for warm boot testing")


    def tearDown_post_start(self):
        print("tearDown_post_start WarmL2SanityTest")
        print("Skip the teardown after warm boot testing")
   ```

### Code for extending the method in SAI-PTF
There is the code logic for splitting the runTest method
```python
def warm_test(f):
    """
    Method decorator for the method on warm testing.
    
    Depends on parameters [test_reboot_mode] and [test_reboot_stage].
    Runs different method, test_starting, setUp_post_start and runTest
    """
    def test_director(inst, *args):
        if inst.test_reboot_stage == 'starting':
            return inst.test_starting()
        if inst.test_reboot_stage == 'post':
            return inst.setUp_post_start()
        return f(inst)
    return test_director
```

## Support DUT differernt running stage during warm reboot
During a warm reboot, we can split the DUT into three different stage
- Before start, in this stage, we will make some configuration or brenchmark on the DUT, and use those setting to check the DUT functionality during warm reboot. All the preparation for the warmboot will be finished.
- Starting, in this stage, DUT will run a warm-reboot command and not start any other docker services and sai-sdk will not start. We expect all the traffic will behivor as normal.
- After Starting, in this stage, sai-sdk will start, data will be restore from the backup file.

In order to do that, we need the followings.

### Setup the warmboot - Before warm reboot
Before starting the test for the warm reboot, we need the following steps
1. remove all the services which can start from the reboot
For remove all the sevices we can use the scipt in [all_service.sh](../../ptf/scripts/all_service.sh)
   ```shell
   #stop all the services
   all_service.sh -o stop
   #backup and remove all the services
   all_serivce.sh -o remove
   ```
2. start the saiserver and add the WARM_REBOOT_WRITE_FILE and SAI_WARM_BOOT_READ_FILE in the profile 
   ```shell
   #Sample profile in brcm s6000
   cat /etc/sai.d/sai.profile
   SAI_INIT_CONFIG_FILE=/usr/share/sonic/hwsku/td2-s6000-32x40G.config.bcm
   SAI_NUM_ECMP_MEMBERS=32
   SAI_WARM_BOOT_WRITE_FILE=/var/warmboot/sai-warmboot.bin
   SAI_WARM_BOOT_READ_FILE=/var/warmboot/sai-warmboot.bin
   ```
3. start the test 
In order to start the PTF tests, we need to copy the test cases into PTF (refer [PTF-SAIv2 testing guide](https://github.com/Azure/sonic-mgmt/blob/master/docs/testbed/sai_quality/PTF-SAIv2TestingGuide.md)) and add the warm boot parameters
   ```shell
   ptf --test-dir /tmp/test/SAI/ptf/warm_boot warm_saiswitch.WarmAvailableIPv4RouteEntryTest --interface '0-0@eth0' ... '0-31@eth31' --relax "--test-params=thrift_server='Ip_address';test_reboot_mode='warm';test_reboot_stage='setup'"
   ```
   *p.s. Need to add the parameters test_reboot_mode='warm';test_reboot_stage='setup'*

4. trigger the warm shutdown automatically after runTest method with @warm_test
The code for making the warm shutdown is
   ```python
    def warm_shutdown(self):
        """
        Shut down swithc in warm boot mode
        """
        print("shutdown the swich in warm mode")
        sai_thrift_set_switch_attribute(self.client, restart_warm=True)
        sai_thrift_set_switch_attribute(self.client, pre_shutdown=True)
        sai_thrift_remove_switch(self.client)
        sai_thrift_api_uninitialize(self.client)
   ```
*p.s. we don't need to call this method manually.*

### Starting DUT - During warm reboot
1. warm-reboot the DUT
   we can use the shell [sai_warmboot.sh]()../../ptf/scripts/sai_warmboot.sh)
   ```shell
   sai_warmboot.sh
   ```
1. change the profile, enable the warm start
   ```shell
   # Sample in a dell s6000
   cat /etc/sai.d/sai.profile
   SAI_INIT_CONFIG_FILE=/usr/share/sonic/hwsku/td2-s6000-32x40G.config.bcm
   SAI_NUM_ECMP_MEMBERS=32
   SAI_WARM_BOOT_WRITE_FILE=/var/warmboot/sai-warmboot.bin
   SAI_WARM_BOOT_READ_FILE=/var/warmboot/sai-warmboot.bin
   SAI_BOOT_TYPE=1
   ```

1. trigger the test in starting mode
   ```shell
   ptf --test-dir /tmp/test/SAI/ptf/warm_boot warm_saiswitch.WarmAvailableIPv4RouteEntryTest --interface '0-0@eth0' ... '0-31@eth31' --relax "--test-params=thrift_server='Ip_address';test_reboot_mode='warm';test_reboot_stage='starting'"
   ```
   *p.s. Need to add the parameters test_reboot_mode='warm';test_reboot_stage='starting'*

### Post warmboot - After warm reboot
1. start saiserver and trigger the test in post mode
   ```shell
   ptf --test-dir /tmp/test/SAI/ptf/warm_boot warm_saiswitch.WarmAvailableIPv4RouteEntryTest --interface '0-0@eth0' ... '0-31@eth31' --relax "--test-params=thrift_server='Ip_address';test_reboot_mode='warm';test_reboot_stage='post'"
   ```
   *p.s. Need to add the parameters test_reboot_mode='warm';test_reboot_stage='post'*


## Example of re-organize the case into warm reboot
In order to reuse the existing test cases in warm reboot, we need to re-organize the existing test cases into the warm reboot structure.

There is a sample for re-organize the test https://github.com/opencomputeproject/SAI/pull/1440