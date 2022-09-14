# Start SaiServer Docker In DUT

*In this article, you will get to know how to start a saiserver docker in a dut(Device under test).*
1. pull or upload the saiserver docker in your dut.
```
#if your docker registry has the saisever docker, you can pull it 
docker push <docker-registry-address>/docker-saiserver-<SHORTEN_ASIC>:<TAG_WITH_OS_VERSION>
#like
docker pull soniccr1.azurecr.io/docker-saiserver-brcm:20201231.29
```
Otherwise, you can upload the docker file from a local building, please refer to doc for how to build a saiserver docker
[Example: Check Sonic Version And Build Saiserver Docker](./ExampleCheckSonicVersionAndBuildSaiserverDocker.md)

then import and start the docker
```shell
docker load -i ./<DOCKERFILE>
```

2.  Config saiserver running env and start saiserver in SONiC console

> **Before start the saiserver docker, you'd better stop other services that running in DUT.**
```shell
SONIC_CFGGEN="sonic-cfggen"
SONIC_DB_CLI="sonic-db-cli"
PLATFORM=${PLATFORM:-`$SONIC_CFGGEN -H -v DEVICE_METADATA.localhost.platform`}
HWSKU=${HWSKU:-`$SONIC_CFGGEN -d -v 'DEVICE_METADATA["localhost"]["hwsku"]'`}
DOCKERNAME=saiserver
#DOCKERIMG=<DOCKERIMAGE>   <--- set the docker image name
docker create --privileged --net=host \
        -v /usr/share/sonic/device/$PLATFORM/$HWSKU:/usr/share/sonic/hwsku:ro \
        --name=$DOCKERNAME $DOCKERIMG
docker start $DOCKERNAME

```


