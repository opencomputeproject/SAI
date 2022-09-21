# Check SAI Header Version And SONiC Version
*In this article, you will get known how to get the SAI header version and SONiC Version.*

- [Check SAI Header Version And SONiC Version](#check-sai-header-version-and-sonic-version)
  - [Check SAI Header Version](#check-sai-header-version)
  - [Check SONiC Version](#check-sonic-version)
  - [Get the commit id from sonic-buildimage.](#get-the-commit-id-from-sonic-buildimage)

## Check SAI Header Version

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
6. Check out the code for the matching sonic version [Example: Check sonic version and build saiserver docker](CheckVersion.md#check-sonic-version) ,
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

## Check SONiC Version

Check SONiC version in a DUT

   **Old version might hit some issue caused by a related package upgrade, you can always use the latest tag of a major version(i.e major is 20201231) but notice the matching image version.**

   ```
   # Image build with tag
   show version
   SONiC Software Version: SONiC.20201231.39
   ```
   ```
   # Image build without tag
   SONiC Software Version: SONiC.master.39085-dirty-20210923.145659
   ```


##  Get the commit id from sonic-buildimage.

   > *ps. sonic-buildimage is a repository used to build sonic images and docker images. SAI is a submodule in sonic-buildimage(/sonic-buildimage/tree/master/src/sonic-sairedis). The commit id in sonic-buildimage can be used to get all the submodules for its submodules, like sai.*

   - Get commit id from tag.

      ```	
      git clone https://github.com/Azure/sonic-buildimage.git
      cd sonic-buildimage

      # git checkout tags/<tag> -b <branch>
      # Example:
      git checkout tags/20201231.39 -b richardyu/20201231-39
      #check the commit id
      git rev-list -n 1 20201231.39
      ```
   - Get commit id from docker image
      ```
      # Get image name
      docker images
      REPOSITORY                                        TAG                                  IMAGE ID            CREATED             SIZE   
      ...   
      docker-orchagent                                  latest                               99d39d932020        6 weeks ago         443MB
      ```
      Check image information
      ```
      docker image inspect docker-orchagent:latest
      "Image": "sha256:...",
            "Volumes": null,
            "WorkingDir": "",
            ...
            "OnBuild": null,
            "Labels": {
                "Tag": "master.39085-bc06c6fcb",

      ```
      **bc06c6fcb is the commit id in sonic-buildimage** 

   > *Note: Check submodule recursively*
   ```
   git submodule update --init --recursive

   # Execute make init once after cloning the repo, or after fetching the remote repo with submodule updates

   make init
   ```
   > *Note: Follow the resource to get how to build a binary and docker*

   [GitHub - Azure/sonic-buildimage: Scripts which perform an installable binary image build for SONiC](https://github.com/Azure/sonic-buildimage)
