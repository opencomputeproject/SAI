# Building Related Resources In Dockers

## Background
In order to build the artifacts in SONiC, we always need some specific environment, for convenience, we build them in some docker container. The docker container defined in repo [sonic-buildimage](https://github.com/Azure/sonic-buildimage)
After the build process, the build-docker will be deleted.
But we might need some docker environment for building something, like debug code.

Then we might need to get a build-docker in the local environment

## Steps
1. Check out [sonic-buildimage](https://github.com/Azure/sonic-buildimage/blob/master/README.md) repo
2. Go through the [doc](https://github.com/Azure/sonic-buildimage/blob/master/README.md), to understand the build system.
3. Choice a target (for different purposes, the command might be different)
```
# build EOS image
make target/sonic-aboot-broadcom.swi
# example:
make configure PLATFORM=marvell-armhf PLATFORM_ARCH=armhf
make target/sonic-marvell-armhf.bin
```
3. Add the parameters [KEEP_SLAVE_ON](https://github.com/Azure/sonic-buildimage/blob/aa59bfeab7eaa569ecf99c8ba62745126ac92602/Makefile.work#L19) when building the target docker
4. Commit the build docker to the local image repository
   Then you can use the [docker command](https://docs.docker.com/engine/reference/commandline/save/) to save the docker to a local image
```
docker commit [OPTIONS] CONTAINER [REPOSITORY[:TAG]]

Example: musing_payne is the docker name, slave-syncd-4334 is the local image name, 0.0.1 is the image tag
	
docker commit musing_payne slave-syncd-4334:0.0.1
```
5. Then you can use the local commit docker for other SAI related buildings

Here is a example [Example: Check sonic version and get a related builder](./ExampleCheckSonicVersionAndBuildSaiserverDocker.md)