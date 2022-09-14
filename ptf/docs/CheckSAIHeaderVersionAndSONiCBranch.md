# Check SAI Header Version And SONiC Branch
*In this article, you will get known how to get the SAI header version, and how to check and get it from the matching [buildimage](https://github.com/Azure/sonic-buildimage) repo.*

For sai, it has many versions and spread with those versions, different SAI header(SPI) defined across them, for testing different SAI headers, we need different SAI binary and SAI Server
	
1. For understanding the testbed topology, make sure you go through the doc at 
https://github.com/Azure/sonic-mgmt/tree/master/docs/testbed
2. Registry the device you want to use
```
Here we need an s6000, t0
```
3. Follow this page to get the testbed info 
[Example of Testbed Configuration - Overview (azure.com)](https://github.com/Azure/sonic-mgmt/blob/master/docs/testbed/README.testbed.Example.Config.md)

4. log in to a sonic device, and check the installed sai version within a `syncd` docker.
```
docker exec -it syncd bash
```
Then, check the installed SAI
```
apt list --installed| grep libsai
```
5. Check out the code from [sonic-buildimage](https://github.com/Azure/sonic-buildimage.git), 
> *Note: remember to change the branch*
6. Check out the code for the matching sonic version [Example: Check sonic version and build saiserver docker](./ExampleCheckSonicVersionAndBuildSaiserverDocker.md) ,
Check the file content at [platform/broadcom/sai.mk](https://github.com/Azure/sonic-buildimage/blob/master/platform/broadcom/sai.mk), there is the link to the binary we installed.
```
The sai binary file and its name will be there
cat sai.mk
BRCM_SAI = libsaibcm_4.3.3.8-1_amd64.deb
$(BRCM_SAI)_URL = "https://sonicstorage.blob.core.windows.net/packages/bcmsai/4.3/202012/libsaibcm_4.3.3.8-1_amd64.deb?*******"
BRCM_SAI_DEV = libsaibcm-dev_4.3.3.8-1_amd64.deb
$(eval $(call add_derived_package,$(BRCM_SAI),$(BRCM_SAI_DEV)))
$(BRCM_SAI_DEV)_URL = "https://sonicstorage.blob.core.windows.net/packages/bcmsai/4.3/202012/libsaibcm-dev_4.3.3.8-1_amd64.deb?********"
```