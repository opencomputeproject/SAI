SAI Versioning Proposal
=======================

Title    | SAI versioning
-------- | ---
Authors  | Intel
Status   | In review
Type     | Standards track
Created  | 01/21/2020
SAI-Version | 1.7.1

----------

This document describes proposal to include versioning in the SAI library. This proposal only solves the following question. <br>
**How does a vendor support multiple SAI versions using only the trunk/master branch?**

Requirements
------------
Provide a mechanism for vendors to find the current version of SAI headers at compile time.

Today, if a vendor wants to support multiple versions of SAI,

### Binaries
A vendor publishes different libSAI implementations each compatible with different SAI versions. The users of the binaries can choose a version of the binary depending on which version of SAI they use for Sonic. The vendor publishes these binaries from different development branches dedicated to each SAI version.

### Source Code
A vendor publishes their SDK in source form and advice their users of which version of their SDK can be used for the requested SAI version. It is assumed the most common form this takes is each vendor maintains a trunk branch with the latest code and patches are back ported to specific branches created for each SAI version.

> ### Example
> Customer using Sonic version 201911 which is compatible with SAI 1.5.4. The vendor SDK version is 1.1. Their customer wants to upgrade to a newer version of the SDK which supports SAI objects/attributes that are only supported in SDK 1.2. The common method to support this is to backport the supported attributes to 1.1.

Proposal
--------
This proposal provides a static version file with version numbers hard-coded per release.

### version.h
```c
#define SAI_MAJOR 1
#define SAI_MINOR 7
#define SAI_REVISION 1

#define SAI_VERSION(major, minor, micro) \
    (10000 * (major) + 100 * (minor) + (micro))
#define SAI_API_VERSION SAI_VERSION(SAI_MAJOR, SAI_MINOR, SAI_REVISION)
```

### For a Vendor
A vendor can publish their SDK using the above versioning using static checks.

Assuming a SAI_OBJECT_TYPE_TEST object with attributes SAI_TEST_ATTR_TYPE1 and SAI_TEST_ATTR_TYPE2. <br>
The “type1” attribute is only available after SAI 1.6.4. <br>
The “type2” attribute is only available after SAI 1.7.4 <br>
A vendor implementation can now be,

### Usage
```c
sai_status_t sai_set_test_attribute(sai_object_id_t id,
                                    const sai_attribute_t *attr) {
    switch (attr->id) {
        case SAI_TEST_ATTR_START:
            break;

#if SAI_API_VERSION >= 164
        case SAI_TEST_ATTR_TYPE1:
            // do your thing
            break;
#endif

#if SAI_API_VERSION >= 174
        case SAI_TEST_ATTR_TYPE2:
            // do your thing
            break;
#endif

        default:
            break;
    }
}
```

More versioning
---------------
Versioning can be exported for the application build system to access without having to include the above header file.
### saiversion.makefile
```makefile
SAI_MAJOR := 1
SAI_MINOR := 7
SAI_REVISION := 1

SAI_API_VERSION := $(shell echo $$(( $(SAI_MAJOR)*10000+$(SAI_MINOR)*10+$(SAI_REVISION) )))
```
An application can then include this makefile to access the values
```makefile
include saiversion.makefile

CXXFLAGS += -DSAI_API_VERSION=$(SAI_API_VERSION)
```
More examples for other build systems like cmake are easy to find.
