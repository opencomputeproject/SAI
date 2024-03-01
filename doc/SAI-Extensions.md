# Process for Adding SAI Extensions

| **Title** | **Process for Adding SAI Extensions       ** |
| --- | --- |
| **Authors** | **Dell Technologies** |
| **Status** | **Approved** |
| **Type** | **Standards Track** |
| **Created** | **11/28/2016** |
| **Modified** | **04/20/2018** |
| **SAI-Version** | **V1.4** |

# SAI Extensions

SAI Extensions are used for introducing:

- Experimental `custom` object attributes
- Experimental SAI modules (new SAI objects)

The goals of defining SAI extensions are to:

- Allow innovation
- Increase velocity
- Provide a formalism for publishing experimental features

## Experimental Extensions Concepts

Experimental extensions have the following characteristics:

- They are optional to SAI implementations. Vendors A and B may choose to implement the same extensions – attribute A and module M.
  Vendor C may choose to implement either attribute A or module M or neither.
- Must be published
- Must follow the SAI approach to define module API's (create/get/set/delete, with key /value pairs for attributes).
- The implicit intent for experimental extensions is to eventually become part of the main SAI API – if they have enough SAI community support.
- A module or attribute should be accepted as an experimental extension if supported by at least two members of the SAI community.


## Extension (Custom) Attributes

Experimental attributes have their own range, starting at the end of the attribute range for the officially accepted object.

Custom SAI attributes are defined in an extension header file associated to the module they belong to. This avoids code churn for the main SAI module definitions.

**Limitations**. Experimental attributes **must not** require changes to existing SAI data structures or other existing SAI API's.
However, new data structures (if applicable) can be added to the extension header file – please refer to the usage notes.

Experimental extensions are included in the experimental directory. For instance:

**experimental/saiportextensions.h**

```c
#include <saiport.h>

typedef enum _sai_port_experimental_attr_t
{
    /* Start after the last attribute of the official SAI API object */
    SAI_PORT_ATTR_EXPERIMENTAL_CAPABILITY_X = SAI_PORT_ATTR_END, /* From saiport.h */
    SAI_PORT_ATTR_EXPERIMENTAL_CAPABILITY_Y,
    SAI_PORT_ATTR_EXPERIMENTAL_CAPABILITY_Z,
    …
} sai_port_experimental_attr_t;
```

A file called sai **module** extensions.h contains enum values specific to extension attributes, e.g. saiportextensions.h.
It is recommended (but not mandatory!) to use the keyword EXPERIMENTAL when defining experimental
attributes – this allows developers to easily detect usage of extension attributes in source code.

There can be only one experimental sai **module** extensions.h file.

## Experimental SAI Modules

Each experimental SAI Module must define its own header file, as is the case for any regular module.

The module **must** contain a doxygen warning - to state that it is an extension, and thus may not be supported by all SAI implementations.

The module description must state what SAI implementation(s) support this module.

```c
/**
…
* @file    saiexperimentalmodule.h
*
* @brief   This module defines an experimental feature
*
* @description Supported by: Acme Corp, Better Electrons Inc., Crossed Wires Ltd.
*
* @warning This module is a SAI experimental module.
*/
```

### SAI Extensions Files

Definitions of new extension / experimental modules APIs and associated experimental object types are added to saiextensions.h
file (rather than sai.h). The documentation of the module name must state that the module is an extension.

**saiextensions.h**

```c
#include <sai.h>

typedef enum _sai_api_extensions_t
{
    SAI_API_EXTENSIONS_RANGE_START = SAI_API_MAX,
    SAI_API_NEW_MODULE = SAI_API_EXTENSIONS_RANGE_START,
    /* Add new experimental APIs above this line */
    SAI_API_EXTENSIONS_RANGE_START_END
    …
} sai_api_extensions_t;
```

**saitypesextensions.h**

```c
#include <saitypes.h>

typedef enum _sai_object_type_extensions_t
{
    SAI_OBJECT_TYPE_EXTENSIONS_RANGE_START = SAI_OBJECT_TYPE_MAX,
    SAI_OBJECT_TYPE_NEW_OBJECT = SAI_OBJECT_TYPE_EXTENSIONS_RANGE_START,
    /* Add new experimental object types above this line */
    SAI_OBJECT_TYPE_EXTENSIONS_RANGE_END
} sai_object_type_extensions_t;
```

Reasoning: use separate files to avoid code churn for saitypes and sai.h.

## Testing Experimental Modules and Attributes

A test methodology and/or scripts should be defined for all experimental attributes and modules – but this is not mandatory. Formal testing only becomes mandatory when experimental attributes and modules are propagated to the main/official SAI API.

# Usage Notes for Experimental Attributes

Assume a vendor (vendor A) suggest to provide an extension – say a port attribute that controls a port LED state, and this attribute is agreed upon at least by one other SAI community member.

### Header File – Experimental Attribute

In this case, the vendor A adds an LED State attribute to the saiportextensions.h file:

**experimental/saiportextensions.h**

```c
typedef enum _sai_port_extensions_attr_t {
    …
    SAI_PORT_ATTR_LED_STATE = SAI_PORT_ATTR_CUSTOM_RANGE_START,
    …
} sai_port_attr_extensions_t;

_// Data type for the experimental attribute LED_STATE_

/**
 * @brief Attribute data for SAI_PORT_ATTR_LED_STATE
 */

typedef enum _sai_port_led_state_t
{
    /** Unknown */
    SAI_PORT_LED_STATE_UNKNOWN,

    /** ON */
    SAI_PORT_LED_ON,

    /** Down */
    SAI_PORT_LED_OFF,
} sai_port_led_state_t;
```


### Host Adapter – Experimental Attribute


```c
#include <saiport.h>
#include <experimental/saiportextensions.h>

…
sai_attribute_t attr;
attr.id = **SAI_PORT_ATTR_LED_STATE;**
attr.u32.value = **SAI_PORT_LED_ON** ;
sai_status_t status = sai_set_port_attribute(port_id, &amp;attr);
…
```

The new attribute does not impact in any way the code provided by a Vendor B which may not support that attribute. However, the Host Adapter implementation needs to be able to distinguish cases where the SAI implementation does or does not support the new experimental attribute.

Note. Explicitly including the `extensions` header file in the source code is intended.
Source files which refer extensions can be easily identified, e.g.  grep –r extensions . or grep –r experimental .  !

## Usage Notes for Experimental Modules

SAI community members A and B may agree to define a common experimental API for `Protocol X`.
This is the desired approach for new protocols.
SAI community members A and B should agree on a common experimental API which can eventually be propagated to the main SAI API.

### Header Files – Experimental Module

saiextensions.h defines:

```c
typedef enum _sai_api_extensions_t
{
    SAI_API_EXTENSIONS_RANGE_START = SAI_API_MAX,
    SAI_API_PROTO_X = SAI_API_EXTENSIONS_RANGE_START,
    /* Add new experimental APIs above this line */
    SAI_API_EXTENSIONS_RANGE_START_END
} sai_api_extensions_t;
```

saitypesextensions.h defines :

```c
typedef enum _sai_object_type_extensions_t
{
    SAI_OBJECT_TYPE_EXTENSIONS_RANGE_START = SAI_OBJECT_TYPE_MAX,
    SAI_OBJECT_TYPE_PROTO_X = SAI_OBJECT_TYPE_EXTENSIONS_RANGE_START,
    …
    SAI_OBJECT_TYPE_EXTENSIONS_RANGE_END
} sai_object_type_extensions_t;
```

An experimental `PROTO_X` extension header file must be defined:

**experimental/saiextensionprotox.h**

```c
…
typedef sai_status_t (*sai_protoX_object_create_fn)(_Out_ sai_object_id_t *protoX_id, …);
…

typedef struct _sai_protoX_api_t {
    sai_protoX_object_create_fn protoX_obj_create_fn;
    sai_protoX_object_delete_fn protoX_obj_delete_fn;
    …
} sai_protoX_api_t;
```

The experimental `PROTO_X` API module must use the SAI API general approach – define create, get, set and delete functions.

### Host Adapter – Experimental Module

A host adapter implementation must include the `PROTO_X` extension module header file. Using an experimental module follows the same approach as for any other modules.

```c
#include "experimental/saiextensionprotox.h"

…
sai_protoX_api_t sai_protoX_api_tbl;
sai_status_t status = sai_api_query(SAI_API_PROTO_X, (void )&amp;sai_protoX_api_tbl);
…
```
