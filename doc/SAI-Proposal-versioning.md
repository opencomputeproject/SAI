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
Provide a mechanism for vendors to find the current version of SAI headers at compile time and run time.

Today, if a vendor wants to support multiple versions of SAI,

### Binaries
A vendor publishes different libSAI implementations each compatible with different SAI versions. The users of the binaries can choose a version of the binary depending on which version of SAI they use for Sonic. The vendor publishes these binaries from different development branches dedicated to each SAI version.

### Source Code
A vendor publishes their SDK in source form and advice their users of which version of their SDK can be used for the requested SAI version. It is assumed the most common form this takes is each vendor maintains a trunk branch with the latest code and patches are back ported to specific branches created for each SAI version.

> ### Example
> Customer using Sonic version 201911 which is compatible with SAI 1.5.4. The vendor SDK version is 1.1. Their customer wants to upgrade to a newer version of the SDK which supports SAI objects/attributes that are only supported in SDK 1.2. The common method to support this is to backport the supported attributes to 1.1.

Another, use case for versioning is the following scenario:

When SAI client's program is compiled using SAI headers version X and linked against SAI library compiled against version Y, except for the cases when ABI changes, there are going to be problems at runtime caused by attributes ID mismatch, enum values mismatch, etc.
The result is that client program will behave incorrect or terminate due to invalid usage of attribute values.

The solution is to provide client's program an ability to determine SAI library version number in run time and let the client's code decide wether it can work with that version or not.


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

A new API is added to query the SAI API version that this library is compiled against.

```c
/**
 * @brief Retrieve a SAI API version this implementation is aligned to
 *
 * @param[out] version Version number
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
sai_status_t sai_query_api_version(
        _Out_ sai_api_version_t *version);
```

The implementation by SAI vendor:

```c
sai_status_t sai_query_api_version(
        _Out_ sai_api_version_t *version)
{
    *version = SAI_API_VERSION;
    return SAI_STATUS_SUCCESS;
}
```

This SAI_API_VERSION is the one derived from headers that were used by vendor SAI.

Using the above API it is possible for client's code to query SAI for an API version it is aligned to
and check if client's code can support the returned version or fail on early stage of client's program.


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

Runtime versioning can be used in client's build system to check if the SAI library version is the same as SAI headers version used to compile client's program, e.g:
```
AC_MSG_CHECKING([SAI headers API version and library version check])
AC_TRY_RUN([
#include <sai.h>
int main() {
    sai_api_version_t version;
    if (SAI_STATUS_SUCCESS != sai_query_api_version(&version))
    {
        return 1;
    }
    return (version != SAI_API_VERSION);
}],
[AC_MSG_RESULT(ok)],
[AC_MSG_RESULT(failed)
AC_MSG_ERROR("SAI headers API version and library version mismatch")])
```

Which will result in an error during "./configure" step of the build process.

More examples for other build systems like cmake are easy to find.
