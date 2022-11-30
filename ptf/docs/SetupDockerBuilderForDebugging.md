# Setup Docker builder for debugging
*In this article you will get know how to get a docker debugging environment.*
- [Setup Docker builder for debugging](#setup-docker-builder-for-debugging)
  - [Background](#background)
  - [Persist a docker for debugging](#persist-a-docker-for-debugging)

## Background

In order to build the artifacts in SONiC, we always need some specific environment for debugging. But after the build process, the docker-builder will be deleted.

> The docker container build process defined in repo [sonic-buildimage](https://github.com/Azure/sonic-buildimage.

## Persist a docker for debugging

Let's see how to get a docker-builder.

1. Check out [sonic-buildimage](https://github.com/Azure/sonic-buildimage/blob/master/README.md) repo


3. Go through the [doc](https://github.com/Azure/sonic-buildimage/blob/master/README.md), to understand the build system.


4. Start a local build (for different purposes, the command might be different)
   > Make sure you add parameters [KEEP_SLAVE_ON](https://github.com/Azure/sonic-buildimage/blob/aa59bfeab7eaa569ecf99c8ba62745126ac92602/Makefile.work#L19) 
   ```
   git clone https://github.com/Azure/sonic-buildimage.git
   cd sonic-buildimage
   git checkout 202012

   # Clean environment as needed
   make reset

   # Init env
   make init

   # NOSTRETCH=y : Current image is a buster
   # KEEP_SLAVE_ON=yes: Keeps slave container up and active after building process concludes.

   #setup environment as Broadcom flatform
   make configure PLATFORM=broadcom

   #start build
   NOSTRETCH=y NOJESSIE=y ENABLE_SYNCD_RPC=y make target/debs/buster/saiserver_0.9.4_amd64.deb
   ```
   **You can get this build target by running commands like NOSTRETCH=y NOJESSIE=y ENABLE_SYNCD_RPC=y make list**
 
5. Wait for the build process 


6. In the end, you will get something like this, and prompt as below (inside docker)
   ```
   # Check if thrift installed
   richardyu@a0363ed6ca36:/sonic$ thrift
   Usage: thrift [options] file

   Use thrift -help for a list of options
   ```

7. Keep this terminal and start another terminal, log in to the same host
 - Check the docker, the builder appears with the name sonic-slave-***, it is always the recently created one
   ```
   docker ps
   CONTAINER ID   IMAGE                                                 COMMAND                  CREATED          STATUS          
   PORTS                                     NAMES
   e1df2df072c4   sonic-slave-buster-richardyu:86ef76a28e6              "bash -c 'make -f sl…"   36 minutes ago   Up 36 minutes   
   22/tcp                                         condescending_lovelace
   ```
 - Commit that docker as a saiserver-docker builder for other debugging or related resource building usages.
   ```
   docker commit <docker_name> <docker_image>:<tag>
   docker commit condescending_lovelace saisever-builder-20201231-39:0.0.1
   ```

7. Then, exit from the docker above (console as 'richardyu@e1df2df072c4'), you can get your buildout artifacts in folder `./target`, there also contains the logs and other accessories


8. For building the saiserver binary, you can mount your local SAI repository to that docker and just start that docker for your building purpose.
   ```
   # SAI repo is located inside the local /code folder
   docker run --name saisever-builder-20201231-39 -v  /code:/data -di saisever-builder-20201231-39:0.0.1 bash
   ```

Then you can use the local commit docker for other SAI-related buildings

