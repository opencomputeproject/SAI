SAI Versioning 
==============

This document describes various proposals to include versioning in the SAI library. 

## Requirements
Provide a mechanism for vendors to find the current version of SAI headers at compile time and at runtime if it is warranted. 

Currently, if a vendor wants to support multiple versions of SAI, 

## Binaries 
A vendor publishes different libSAI implementations each compatible with different SAI versions. The users of the binaries can choose a version of the binary depending on which version of SAI they use for Sonic. The vendor usually publishes these binaries from different development branches dedicated to each SAI version. 

## Source Code 
A vendor publishes their SDK in source form and advice their users of which version of their SDK can be used for the requested SAI version. It is assumed the most common form this takes is each vendor maintains a trunk branch with the latest code and patches are backported to specific branches created for each SAI version. 

> ## Example 
> Customer currently using Sonic version 201911 which is compatible with SAI 1.5.4. The vendor SDK version is 1.1. They customer wants to upgrade to a newer version of the SDK which supports SAI objects/attributes that are only supported in 1.2. The common method to support this is to backport the supported attributes to 1.1.  

This proposal simply wishes to solve the following question. 
How does a vendor support multiple SAI versions using only the trunk branch? 

Proposal 1: 
This proposal provides a static configuration file with version numbers. 
 
### version.h 

    static const unsigned SAI_VERSION_MAJOR = 1; 
    static const unsigned SAI_VERSION_MINOR = 6; 
    static const unsigned SAI_VERSION_REVISION = 4; 

    static const unsigned SAI_VERSION = SAI_VERSION_MAJOR * 100 + 
                                        SAI_VERSION_MINOR * 10 + 
                                        SAI_VERSION_REVISION; 

### version.mk 
SAI_VERSION_MAJOR := 1 
SAI_VERSION_MINOR := 6 
SAI_VERSION_REVISION := 4 
SAI_VERSION := 164 # derived somehow from above values TBD 

version.h can be used to do runtime version comparisons while version.mk can be used to generate a static compile time macro. 

For a Vendor: 
A vendor can publish their SDK using the above versioning system the following. 
Assuming a SAI_OBJECT_TYPE_TEST and SAI_TEST_ATTR_TYPE. The “type” attribute is only available after SAI 1.6.4. A vendor implementation can then simply be, 

### Example usage:
sai_status_t sai_set_test_attribute(sai_object_id_t id, 
                                    const sai_attribute_t *attr) { 
    switch (attr->id) { 
        case SAI_TEST_ATTR_START: 
            break; 
#if SAI_VERSION >= 164 
        case SAI_TEST_ATTR_TYPE: 
            // do your thing 
            break; 
#endif 
#if SAI_VERSION >= 174 
        case SAI_TEST_ATTR_TYPE2: 
            // do your thing 
            break; 
#endif 
        default: 
            break; 
    } 
} 

For an application/NOS: 
make CPPFLAGS=-DSAI=164 
