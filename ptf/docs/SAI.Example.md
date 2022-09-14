# Run SAI PTF Case
### Prerequisites

Through previous docs make sure the PTF docker, sonic-mgmt docker, DUT (running on SONiC.202012 image in this case) are all set.

### Prepare test cases on PTF
In this section, we will prepare SAI test cases on the PTF docker.
1. Logon to your PTF docker, make sure the PTF docker could access to GitHub, then type the following commands to clone SAI repo:
- SAI **v1.7** will be used for this example since v1.7 is one of the supported SAI versions in SONiC.202012, for more information please check [Check SAI header with SONiC branch](CheckSAIHeaderVersionAndSONiCBranch.md) 

```
rm -rf ./SAI
git init SAI
cd SAI
git remote add origin https://github.com/opencomputeproject/SAI.git
git fetch origin
git checkout -b v1.7 origin/v1.7
```
​	It will have the following result:
```
root@314fb9c4a38f:/tmp# rm -rf ./SAI
root@314fb9c4a38f:/tmp# git init SAI
Initialized empty Git repository in /tmp/SAI/.git/
root@314fb9c4a38f:/tmp# cd SAI
root@314fb9c4a38f:/tmp/SAI# git remote add origin https://github.com/opencomputeproject/SAI.git
root@314fb9c4a38f:/tmp/SAI# git fetch origin
remote: Enumerating objects: 7962, done.
remote: Counting objects: 100% (534/534), done.
remote: Compressing objects: 100% (302/302), done.
remote: Total 7962 (delta 339), reused 350 (delta 231), pack-reused 7428
Receiving objects: 100% (7962/7962), 63.32 MiB | 30.71 MiB/s, done.
Resolving deltas: 100% (5490/5490), done.
From https://github.com/opencomputeproject/SAI
 * [new branch]      master                     -> origin/master
 * [new branch]      rajeevsharma1-patch-1      -> origin/rajeevsharma1-patch-1
 * [new branch]      revert-648-vlanigmpcontrol -> origin/revert-648-vlanigmpcontrol
 * [new branch]      v0.9.1                     -> origin/v0.9.1
 * [new branch]      v0.9.2                     -> origin/v0.9.2
 * [new branch]      v0.9.4                     -> origin/v0.9.4
 * [new branch]      v0.9.5                     -> origin/v0.9.5
 * [new branch]      v0.9.6                     -> origin/v0.9.6
 * [new branch]      v1.0                       -> origin/v1.0
 * [new branch]      v1.1                       -> origin/v1.1
 * [new branch]      v1.2                       -> origin/v1.2
 * [new branch]      v1.3                       -> origin/v1.3
 * [new branch]      v1.4                       -> origin/v1.4
 * [new branch]      v1.5                       -> origin/v1.5
 * [new branch]      v1.6                       -> origin/v1.6
 * [new branch]      v1.7                       -> origin/v1.7
 * [new branch]      v1.8                       -> origin/v1.8
 * [new branch]      v1.9                       -> origin/v1.9
 ........
```

```
root@314fb9c4a38f:/tmp/SAI# git checkout -b v1.7 origin/v1.7
Branch v1.7 is set up to track remote branch v1.7 from origin.
Switched to a new branch 'v1.7'
root@314fb9c4a38f:/tmp/SAI# ls -l
total 160
-rw-r--r--  1 root root 106691 Oct 20 06:40 Doxyfile
-rw-r--r--  1 root root   2700 Oct 20 06:40 LICENSE.txt
-rw-r--r--  1 root root   1116 Oct 20 06:40 Makefile
-rw-r--r--  1 root root    463 Oct 20 06:40 README.md
drwxr-xr-x  5 root root   4096 Oct 20 06:40 bm
drwxr-xr-x  3 root root   4096 Oct 20 06:40 data
drwxr-xr-x  2 root root   4096 Oct 20 06:40 debian
drwxr-xr-x 32 root root   4096 Oct 20 06:40 doc
drwxr-xr-x  2 root root   4096 Oct 20 06:40 experimental
drwxr-xr-x  3 root root   4096 Oct 20 06:40 flexsai
drwxr-xr-x  2 root root   4096 Oct 20 06:40 inc
drwxr-xr-x  2 root root   4096 Oct 20 06:40 meta
drwxr-xr-x  5 root root   4096 Oct 20 06:40 stub
drwxr-xr-x  6 root root   4096 Oct 20 06:40 test
```

### Prepare testing environment on DUT
1. Logon to the DUT and stop all services before starting saiserver:

- It'd be better to stop other services/containers running on DUT, especially syncd and swss.
```
sudo systemctl stop <services_name>
```
2. After [Example: Check sonic version and get a saiserver docker with a related builder](GetDockerBuildingRelatedResources.md), the docker registry should have the **docker-saiserver-brcm** images.

- Pull the image from the registry, in this case, the os version is: 202012

```
admin@s6000:~$ docker pull <docker-registry>/docker-saiserver-brcm:202012
Status: Downloaded newer image for <docker-registry>/docker-saiserver-brcm:202012
```
- Tag the images with name **docker-saiserver-brcm**
```
admin@s6000:~$ docker tag ${SONIC_REG}/docker-saiserver-brcm:202012 docker-saiserver-brcm
```
- Execute step2 from [Example: Start SaiServer Docker from a DUT](ExampleStartSaiServerDockerInDUT.md)

```
admin@s6000:~$ SONIC_CFGGEN="sonic-cfggen"
admin@s6000:~$ SONIC_DB_CLI="sonic-db-cli"
admin@s6000:~$ PLATFORM=${PLATFORM:-`$SONIC_CFGGEN -H -v DEVICE_METADATA.localhost.platform`}
admin@s6000:~$ HWSKU=${HWSKU:-`$SONIC_CFGGEN -d -v 'DEVICE_METADATA["localhost"]["hwsku"]'`}
admin@s6000:~$ DOCKERNAME=saiserver
admin@s6000:~$ DOCKERIMG=docker-saiserver-brcm:latest
admin@s6000:~$ docker create --privileged --net=host \
>         -v /usr/share/sonic/device/$PLATFORM/$HWSKU:/usr/share/sonic/hwsku:ro \
>         --name=$DOCKERNAME $DOCKERIMG
59b7fec7645fbf448a5c4db53a474f36695deec98b40dd1656441abd67ac03d0
admin@s6000:~$ docker start $DOCKERNAME
saiserver
```
- Check current running docker containers with```docker ps```, **docker-saiserver-brcm:latest** container is running means DUT has the target saiserver docker up.

```
admin@s6000:~$ docker ps
CONTAINER ID        IMAGE                          COMMAND                  CREATED             STATUS              PORTS               NAMES
59b7fec7645f        docker-saiserver-brcm:latest   "/usr/local/bin/supe…"   25 seconds ago      Up 16 seconds                           saiserver
```
- Use ```docker exec -it saiserver bash``` to login to saiserver docker bash

```
admin@s6000:~$ docker exec -it saiserver bash
```
- Also make sure the port **9092** is up, which the saiserver is listening to

```
admin@s6000:~$ sudo netstat -tulpn | grep LISTEN
<some other ports>           
tcp6       0      0 :::9092                 :::*                    LISTEN      1224720/saiserver
```
Now the saiserver on DUT should be ready for SAI testing.

#### Start SAI testing on PTF

On your **PTF** docker, prepare a file named **default_interface_to_front_map.ini** an example could be found at [Example](https://github.com/opencomputeproject/SAI/blob/master/test/saithrift/src/msn_2700/default_interface_to_front_map.ini).
execute the following command to start SAI test:
```
ptf --test-dir <path_to_test_folder> sail2.L2AccessToAccessVlanTest --interface '1@eth1' --interface '2@eth2' -t "server='<DUT ipaddr>';port_map_file='<path_to_default_interface_to_front_map.ini>'"
```
It will have the result like this:
```
root@314fb9c4a38f:/tmp/SAI/test/saithrift# ptf --test-dir tests sail2.L2AccessToAccessVlanTest --interface '1@eth1' --interface '2@eth2' -t "server='<DUT ipaddr>';port_map_file='default_interface_to_front_map.ini'"
WARNING: No route found for IPv6 destination:: (no default route?)
sail2.L2AccessToAccessVlanTest ... 
Sending L2 packet port 1 -> port 2 [access vlan=10])
ok

----------------------------------------------------------------------
Ran 1 test in 1.662s

OK
```