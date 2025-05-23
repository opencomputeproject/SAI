[![Build Status](https://dev.azure.com/mssonic/build/_apis/build/status%2Fopencomputeproject.SAI?branchName=refs%2Fpull%2F2166%2Fmerge)](https://dev.azure.com/mssonic/build/_build/latest?definitionId=131&branchName=refs%2Fpull%2F2166%2Fmerge)

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
