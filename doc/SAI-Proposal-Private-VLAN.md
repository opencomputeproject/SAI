Switch Abstraction Interface Change Proposal

Title       | Generic Resource Query
------------|----------------
Authors     | Dell Technologies
Status      | In review
Type        | Standards track
Created     | 05/03/2019
----------

## Overview
Add provisions in SAI to configure VLAN types (Primary/Community/Isolated) for the Private VLAN 
functionality (Reference: RFC5517 : https://tools.ietf.org/html/rfc5517).

Additionally PRIVATE_VLAN_TRUNK_MODE attribute is added to VLAN member attributes to distinguish
between promiscuous/host ports and trunk ports of the Private VLANs.

## Spec

##### Changes to existing SAI header file
In saivlan.h 

1. Add new enumeration sai_vlan_private_vlan_type_t for private VLAN types.

```
/**
 * @brief Attribute data for private vlan type
 */
typedef enum _sai_vlan_private_vlan_type_t
{
	/** Not a private vlan */
	SAI_VLAN_PRIVATE_VLAN_TYPE_DEFAULT,

	/** Primary vlan */
	SAI_VLAN_PRIVATE_VLAN_TYPE_PRIMARY,

	/** Community vlan */
	SAI_VLAN_PRIVATE_VLAN_TYPE_COMMUNITY,

	/** Isolated vlan */
	SAI_VLAN_PRIVATE_VLAN_TYPE_ISOLATED,

} sai_vlan_private_vlan_type_t;
```

2) Add new attribute SAI_VLAN_ATTR_PRIVATE_VLAN_TYPE in sai_vlan_attr_t to identify the type of VLAN.
3) Add new attribute SAI_VLAN_ATTR_PRIMARY_VLAN_ID in sai_vlan_attr_t to associate Community/Isolated
   VLANs to its corresponding Primary VLAN.

```
typedef enum _sai_vlan_attr_t
{
     .
     .
     .
 
     */
     SAI_VLAN_ATTR_LEARN_DISABLE,

+    /**
+     * @brief To set Private vlan type on a VLAN
+     *
+     * @type sai_vlan_private_vlan_type_t
+     * @flags CREATE_AND_SET
+     * @default SAI_VLAN_PRIVATE_VLAN_TYPE_DEFAULT
+     */
+    SAI_VLAN_ATTR_PRIVATE_VLAN_TYPE,
+
+    /**
+     * @brief Primary vlan id for a secondary VLAN
+     *
+     * @type sai_object_id_t
+     * @flags CREATE_AND_SET
+	 * @objects SAI_OBJECT_TYPE_VLAN
+	 * @allownull true
+     * @default SAI_NULL_OBJECT_ID
+     * @validonly SAI_VLAN_ATTR_PRIVATE_VLAN_TYPE ==
+     * SAI_VLAN_PRIVATE_VLAN_TYPE_COMMUNITY or
+	 * SAI_VLAN_ATTR_PRIVATE_VLAN_TYPE == SAI_VLAN_PRIVATE_VLAN_TYPE_ISOLATED
+	 *
+     */
+	 SAI_VLAN_ATTR_PRIMARY_VLAN_ID,
+
    /**
    .
    .
    .
} sai_vlan_attr_t;
```

4) Add a boolen attribute SAI_VLAN_MEMBER_ATTR_PRIVATE_VLAN_TRUNK_MODE to sai_vlan_member_attr_t to
   distinguish between host/promiscous ports and trunk ports of the VLANs.
   
```
typedef enum _sai_vlan_member_attr_t
{
    .
    .
    .
    */
    SAI_VLAN_MEMBER_ATTR_VLAN_TAGGING_MODE,

 +   /**
 +    * @brief Private VLAN Trunk  mode
 +    *
 +    * @type bool
 +   * @flags CREATE
 +    * @default false
 +    */
 +   SAI_VLAN_MEMBER_ATTR_PRIVATE_VLAN_TRUNK_MODE,
 +
    /**
     * @brief End of attributes
     */
    SAI_VLAN_MEMBER_ATTR_END,
    
    /** Custom range base value */
    SAI_VLAN_MEMBER_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_VLAN_MEMBER_ATTR_CUSTOM_RANGE_END

} sai_vlan_member_attr_t;
```
