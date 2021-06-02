FEC Configuration
-------------------------------------------------------------------------------
 Title       | SAI FEC configuration
-------------|-----------------------------------------------------------------
 Authors     | Mike Beresford, Google LLC
 Status      | In review
 Type        | Standards track
 Created     | 05/25/2021
 SAI-Version | 1.8.1


-------------------------------------------------------------------------------

This spec discusses the port Forward Error Correction (FEC) configuration
use-cases.

FEC is used to provide detection and correction of errors in transmitted data
without the need for re-transmission. There are two general FEC types supported by
the SAI interface:
* FireCode (FC) FEC, used on 10G, 25G, 40G and 50G ports
* Reed-Solomon (RS) FEC, a family of FEC used on 50G, 100G, 200G and 400G ports

There are several variants of RS-FEC that may be used depending on the port's
configured speed.

### FEC configuration (non-extended)

The base FEC configuration provides the following FEC configuration options:

```
typedef enum _sai_port_fec_mode_t
{
    /** No FEC */
    SAI_PORT_FEC_MODE_NONE,

    /** Enable RS-FEC - 25G, 50G, 100G ports */
    SAI_PORT_FEC_MODE_RS,

    /** Enable FC-FEC - 10G, 25G, 40G, 50G ports */
    SAI_PORT_FEC_MODE_FC,
} sai_port_fec_mode_t;
```

This is the enum used for the following port attributes:
* `SAI_PORT_ATTR_SUPPORTED_FEC_MODE`: used to read the supported FEC modes for
  the port.
* `SAI_PORT_ATTR_REMOTE_ADVERTISED_FEC_MODE`: used to read the remote advertised
  Auto Negotation FEC mode.
* `SAI_PORT_ATTR_ADVERTISED_FEC_MODE`: used to configure the advertised Auto
  Negotiation mode for the port.
* `SAI_PORT_ATTR_FEC_MODE`: used to configure the FEC mode for the port.

When `SAI_PORT_ATTR_FEC_MODE` is used to configure the FEC mode of the port to
`SAI_PORT_FEC_MODE_RS`, the SAI adapter will select an appropraite RS-FEC mode
based on the port, number of lanes and modulation type. For example, a port
configured as 100G using 4 lanes and NRZ modulation would select RS-528 (CL91)
FEC. If there is ambiguity in the FEC mode allowed, the adapter will select one
of the options. Likewise, on read any RS-FEC mode will be reported as
`SAI_PORT_FEC_MODE_RS`. `SAI_PORT_ATTR_FEC_MODE` and
`SAI_PORT_ATTR_ADVERTISED_FEC_MODE` are only used if
`SAI_PORT_ATTR_USE_EXTENDED_FEC` is configured to `false` or is unset
(defaulting to false).

### Extended FEC Configuration

In order to allow more direct control of the FEC configuration without breaking
backward compatibility with the pre-existing FEC configuration, there is an
extended enum defined: `sai_port_fec_mode_extended_t`. The extended FEC mode
overlaps with the base `sai_port_fec_mode_t`, with the key difference being that
the specific RS-FEC mode may be configured. In order to set the extended FEC
mode configuration attributes, `SAI_PORT_ATTR_USE_EXTENDED_FEC` must be
configured to `true`. If `SAI_PORT_ATTR_USE_EXTENDED_FEC` is not configured or
is configured to `false`, the extended configuration will not be used.

```
typedef enum _sai_port_fec_mode_extended_t
{
    /** No FEC */
    SAI_PORT_FEC_MODE_EXTENDED_NONE,

    /** Enable RS-528 (CL91) FEC - 25G, 50G, 100G ports */
    SAI_PORT_FEC_MODE_EXTENDED_RS528,

    /** Enable RS544-FEC - 100G PAM4, 200G ports */
    SAI_PORT_FEC_MODE_EXTENDED_RS544,

    /** Enable RS544-FEC (interleaved) - 100G, 200G, 400G ports */
    SAI_PORT_FEC_MODE_EXTENDED_RS544_INTERLEAVED,

    /** Enable FC-FEC (CL74) - 10G, 25G, 40G, 50G ports */
    SAI_PORT_FEC_MODE_EXTENDED_FC,
} sai_port_fec_mode_extended_t;
```

This is the enum used for the following port attributes:
* `SAI_PORT_ATTR_SUPPORTED_FEC_MODE_EXTENDED`: used to read the supported FEC
  modes for the port.
* `SAI_PORT_ATTR_REMOTE_ADVERTISED_FEC_MODE_EXTENDED`: used to read the remote
  advertised Auto Negotiation FEC mode.
* `SAI_PORT_ATTR_ADVERTISED_FEC_MODE_EXTENDED`: used to configure the advertised
  Auto Negotation mode for the port. **Only used in set operations if
  `SAI_PORT_ATTR_USE_EXTENDED_FEC` is true**
* `SAI_PORT_ATTR_FEC_MODE_EXTENDED`: used to configure the FEC mode for the
  port. **Only used in set operations if `SAI_PORT_ATTR_USE_EXTENDED_FEC` is true**
