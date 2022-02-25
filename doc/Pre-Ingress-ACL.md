# Overview

### Virtual Router derivation
In a standard router, a packet is received on a port. Based on the port property and relevant packet header fields, the ingress interface is derived.\
L3 interfaces are assigned to a virtual router (VRF) when they are created. This VRF is used in route lookup.

### Flexible Virtual Router assignment
It can be beneficial to have some flexibility in VRF assignment. Specifically it is useful to have the ability to match on packet header fields to override the VRF derived from the router interface.\
An example use case would be to use the DSCP value from the packet’s header to set VRF in order to be able to forward high priority traffic on optimal paths.

### Proposal
#### Pre-Ingress ACL stage
The packet header based VRF assignment functionality can be provided by an ACL that must be applied before the L3 forwarding lookup.\
To enable this a new ACL stage is defined

```
inc/saitypes.h

 typedef enum _sai_acl_stage_t
 {
     ...

     /** Pre-ingress Stage */
     SAI_ACL_STAGE_PRE_INGRESS,

 } sai_acl_stage_t;

```

in addition to an attribute to bind ACLs with this stage to the switch.

```
inc/saiswitch.h
 typedef enum _sai_switch_attr_t
 {
     ...

    /**
     * @brief Switch/Global bind point for pre-ingress ACL object
     *
     * Bind (or unbind) an pre-ingress ACL table or ACL group globally. Enable/Update
     * pre-ingress ACL table or ACL group filtering by assigning the list of valid
     * object id. Disable pre-ingress filtering by assigning SAI_NULL_OBJECT_ID
     * in the attribute value.
     *
     * @type sai_object_id_t
     * @flags CREATE_AND_SET
     * @objects SAI_OBJECT_TYPE_ACL_TABLE, SAI_OBJECT_TYPE_ACL_TABLE_GROUP
     * @allownull true
     * @default SAI_NULL_OBJECT_ID
     */
     SAI_SWITCH_ATTR_PRE_INGRESS_ACL,

     ...
} sai_switch_attr_t;
```
Binding a Pre-Ingress ACL to the switch bind point allows all traffic to match on the rules before the L3 lookup. (This helps with scale when the rules in the Pre-Ingress ACL are not port specific)\
Pre-Ingress ACLs take effect before Ingress ACL. So Ingress ACLs can override any non-terminal actions taken by the Pre-Ingress ACL.\
The currently defined ACL match fields will be used for the Pre-Ingress ACL
stage as well.\

#### Set VRF action
This ACL will support a new ACL action to set VRF in the packet metadata.\
This VRF will take precedence over the VRF derived from the ingress interface.

```
inc/saiacl.h
typedef enum _sai_acl_action_type_t
{
     ...

     /** Associate with virtual router */
     SAI_ACL_ACTION_TYPE_SET_VRF

} sai_acl_action_type_t;
```
```
typedef enum _sai_acl_entry_attr_t
{
     ...

     /**
      * @brief Set virtual router
      *
      * @type sai_acl_action_data_t sai_object_id_t
      * @flags CREATE_AND_SET
      * @objects SAI_OBJECT_TYPE_VIRTUAL_ROUTER
      * @default disabled
      */
     SAI_ACL_ENTRY_ATTR_ACTION_SET_VRF,

     ...

} sai_acl_entry_attr_t;
```




### Usage
Create an ACL table
```
    sai_object_id_t acl_table_id = 0ULL;
    acl_table_attrs[0].id = SAI_ACL_TABLE_ATTR_ACL_STAGE;
    acl_table_attrs[0].value.s32 = SAI_ACL_STAGE_PRE_INGRESS;

    acl_table_attrs[1].id = SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST;
    acl_table_attrs[1].value.objlist.count = 1;
    acl_table_attrs[1].value.objlist.list[0] = SAI_ACL_BIND_POINT_TYPE_SWITCH;

    acl_table_attrs[2].id = SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL;
    acl_table_attrs[2].value.booldata = true;
    status = sai_acl_api->create_acl_table(&acl_table_id, 3, acl_table_attrs);
    if (status != SAI_STATUS_SUCCESS) {
        return status;
    }
```

Create an ACL entry
```
    sai_object_id_t acl_entry_id = 0ULL;
    acl_entry_attrs[0].id = SAI_ACL_ENTRY_ATTR_TABLE_ID;
    acl_entry_attrs[0].value.oid = acl_table_id;

    acl_entry_attrs[1].id = SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL;
    acl_entry_attrs[1].value.aclfield.data.u8 = 17;
    acl_entry_attrs[1].value.aclfield.mask.u8 = 255;

    acl_entry_attrs[2].id = SAI_ACL_ENTRY_ATTR_ACTION_SET_VRF;
    acl_entry_attrs[2].value.aclaction.enable = true;
    acl_entry_attrs[2].value.aclaction.parameter.oid = vrf_oid;

    status = sai_acl_api->create_acl_entry(&acl_entry_id, 3, acl_entry_attrs);
    if (status != SAI_STATUS_SUCCESS) {
        return status;
    }
```

Bind ACL table to switch
```
   switch_attr.id = SAI_SWITCH_ATTR_PRE_INGRESS_ACL;
   switch_attr.value.oid = acl_table_id;

   status = sai_switch_api->set_switch_attribute(switch_id, &switch_attr);
   if (status != SAI_STATUS_SUCCESS) {
       return status;
   }
```

### Questions raised in earlier meeting
*   Would this apply to non-L3 traffic?
    *   Yes if ACL contains L2 only match fields, but the VRF would be ignored as L3 lookup will not happen
*   What VRF if any would be in the punted packet metadata
    *   Should be the VRF used for L3 lookup
*   Is this before or after decap?
    *   The intent is to override the VRF used for L3 lookup. Tunnel decap yields the Tunnel Interface which is part of a VRF. After decap the inner packet’s DIP is used for the forwarding lookup. Ideally the inner packet’s headers should be used for VRF override in case the tunnel decap has happened.
*   What about other RIF properties?
    *   Only VRF is overridden, other RIF properties are retained.
