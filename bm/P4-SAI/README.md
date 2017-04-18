# P4 SOFT SWITCH MODEL
This repository contains P4 implementation of SAI behavioural model, forked from [p4lang/behavioural_model](https://github.com/p4lang/behavioral-model/).
<a name="p4"></a>

The Model is meant to provide a conformal behavioural SAI(https://github.com/opencomputeproject/SAI) model.
It implements SAI 1.0 on top of soft switch coded via P4.  

## Compiling P4 Behavioural model 
For list of dependencies and build instructions:
[p4lang/behavioural_model](https://github.com/p4lang/behavioral-model/).

## P4 Target
P4 target is found inside [targets/P4-SAI](targets/P4-SAI).

### Create virtual interfaces
Use the ```veth_setup.sh``` script to create the virtual interfaces and hosts.
### Running P4 program
running the switch can be done with the following command: ```run_server.sh``` found in P4-SAI target directory.
This script olso implements some default configurations found in: p4src/DefaultConfig.txt


### Compiling P4 code
if any editing to the P4 program was made, before running it you first need to transform the P4 code into a json representation which can be consumed by the software switch. This
representation will tell bmv2 which tables to initialize, how to configure the
parser, ... It is produced by the [p4c-bm](https://github.com/p4lang/p4c-bm)
tool. Please take a look at the
[README](https://github.com/p4lang/p4c-bm/blob/master/README.rst) for this repo
to find out how to install it. Once this is done, 
use the ```compile_json.sh``` script that is found inside the P4-SAI target directory.
or manualy, as follows:

    p4c-bmv2 --p4-v1.1 --json <path to JSON file> <path to P4 file>
    
