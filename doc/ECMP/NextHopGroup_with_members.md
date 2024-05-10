### Next hop group with members

Introduce a new type of Next hop group, that is created/modified by specifying the list of next hop members along with their weights.

### Motivation

The existing workflow to create Next hop groups involves these steps
* Create a Next hop group
* Add next hop to this group

Next hop groups can be modified by
* Removing existing next hop group members
* Modifying attributes of existing next hop group members
* Adding new next hop group members

This multi-step process to modify Next hop groups can lead to the Next hop group to be in intermediate state(s) before finally matching the application intent. The sequence of add/modify/delete must ensure that the number of next hop group members do not exceed the maximum size of the Next hop group. The group should not be unintentionally empty. This sequence may lead to the Next hop group to have less forwarding capacity in the intermediate states.

If we could instead specify the current list of next hop group members, we could modify the Next hop group in one step.

### Proposal

* Introduce a new type of Next hop group
* Add a new Next hop group attribute for the list of Next hops
* Add a new Next hop group attribute for the list of weights
* Create/modify the Next hop group by specifying both of the above lists

### Example

Create a next hop group.

```
        std::vector<sai_object_id_t> nh_oids{nh1, nh2, nh3};
        std::vector<uint32_t> weights{1, 2, 1};
        std::vector<sai_attribute_t> group_attr;

        attr.id = SAI_NEXT_HOP_GROUP_ATTR_TYPE;
        attr.value.s32 = SAI_NEXT_HOP_GROUP_TYPE_ECMP_WITH_MEMBERS;
        group_attr.push_back(attr);

        attr.id = SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_MEMBER_WEIGHT_LIST;
        attr.value.u32list.count = (uint32_t)weights.size();
        attr.value.u32list.list = weights.data();
        group_attr.push_back(attr);

        attr.id = SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST;
        attr.value.objlist.count = (uint32_t)nh_oids.size();
        attr.value.objlist.list = nh_oids.data();
        group_attr.push_back(attr);

        sai_status_t status =
          sai->create(SAI_OBJECT_TYPE_NEXT_HOP_GROUP, &group_oid, switchid,
                      (uint32_t)group_attr.size(), group_attr.data());
```

Modify follows the same pattern by specifying the new list of next hops and weights via the bulk set api

```
        std::vector<sai_object_id_t> nh_oids{nh1, nh3, nh4};
        std::vector<uint32_t> weights{1, 2, 3};
        std::vector<sai_attribute_t> group_attr;
        std::vector<sai_object_id_t> nhg_oids{nhg1, nhg1};
        std::vector<sai_status_t> statuses;

        attr.id = SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_MEMBER_WEIGHT_LIST;
        attr.value.u32list.count = (uint32_t)weights.size();
        attr.value.u32list.list = weights.data();
        group_attr.push_back(attr);

        attr.id = SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_LIST;
        attr.value.objlist.count = (uint32_t)nh_oids.size();
        attr.value.objlist.list = nh_oids.data();
        group_attr.push_back(attr);

        sai_status_t status =
          sai->bulkSet(SAI_OBJECT_TYPE_NEXT_HOP_GROUP,
                      (uint32_t)nhg_oids.size(),
                      nhg_oids.data(), group_attr.data(),
                      SAI_BULK_OP_ERROR_MODE_STOP_ON_ERROR,
                      statuses.data());
```
