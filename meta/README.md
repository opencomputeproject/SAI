[![Build Status](https://sonic-jenkins.westus2.cloudapp.azure.com/buildStatus/icon?job=sai-meta-build)](https://sonic-jenkins.westus2.cloudapp.azure.com/job/sai-meta-build)

Metadata for SAI
================

Metadata for SAI is a set of auto generated (based on SAI headers) data
and functions which allow SAI attributes validation and more.

Metadata is generated as ANSI C source and header.

Parser also forces headers to be well formated when adding new code.

To test your changes just type:

```sh
[GEN_SAIRPC_OPTS=<option flags>] make
```
e.g. 
```
GEN_SAIRPC_OPTS="-ve" make
```
