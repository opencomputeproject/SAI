# SAI_PACKET_ACTION_DONOTDROP packet action
-------------------------------------------------------------------------------
 Title       | SAI_PACKET_ACTION_DONOTDROP packet action
-------------|-----------------------------------------------------------------
 Authors     | Ashish Singh, Google LLC
 Status      | In review
 Type        | Standards track
 Created     | 01/25/2022

-------------------------------------------------------------------------------

This spec discusses the SAI_PACKET_ACTION_DONOTDROP packet action use-cases.

Goal of this packet action attribute (SAI_PACKET_ACTION_DONOTDROP) is to resolve the conflicts of ACL actions when a packet hits more than one ACLs in an ACL stage such that when this packet action is used with an ACL, it serves as an action that prevents the possible drop from another ACL that is in a lower priority table.

This new packet action attribute is in line with the current SAI architecture - actions in higher priority groups take precedence. If an ACL with DROP action is in a higher priority group than this ACL, DROP action is expected to be honored. Table priority group of a table can be set using SAI_ACL_TABLE_GROUP_MEMBER_ATTR_PRIORITY.

Scope of the SAI_PACKET_ACTION_DONOTDROP packet action is limited to the ACL stage where it is used in the ACL as packet action - it would not override the DROP action in subsequent ACL stage(s) tables.

Few cases are explained with examples in next section.

# Usage Examples
### Case 1

| ACL in table T1                     | ACL in table T2                       |
|-------------------------------------|---------------------------------------|
| Table priority group:  P1           | Table priority group:  P2             |
|Action: SAI_PACKET_ACTION_DONOTDROP  | Action: SAI_PACKET_ACTION_DROP        |



If P1 > P2: action is “no DROP”


If P2 > P1: action is “DROP”



### Case 2

| ACL in table T1                     | ACL in table T2                       |
|-------------------------------------|---------------------------------------|
| Table priority group:  P1           | Table priority group:  P2             |
|Action: SAI_PACKET_ACTION_DONOTDROP  | Action: neither SAI_PACKET_ACTION_DROP nor SAI_PACKET_ACTION_DONOTDROP          |


If P1 > P2: action is “no DROP”


If P2 > P1: action is “no DROP”


### Case 3

| ACL in table T1                     | ACL in table T2                       |
|-------------------------------------|---------------------------------------|
| Table priority group:  P1           | Table priority group:  P2             |
|Action: neither SAI_PACKET_ACTION_DROP conflicting action nor SAI_PACKET_ACTION_DONOTDROP  | Action: SAI_PACKET_ACTION_DROP     |


If P1 > P2: action is “DROP”


If P2 > P1: action is “DROP”

### Case 4

For SAI actions that are implemented as a copy of other actions including a DROP, the SAI_PACKET_ACTION_DONOTDROP action acts on only the DROP portion of the action.

TRAP action may be implemented as COPY + DROP. A higher-priority DONOTDROP action will cancel the DROP action only, resulting in the packet being copied to CPU but not dropped.

| ACL in table T1                     | ACL in table T2                       |
|-------------------------------------|---------------------------------------|
| Table priority group:  P1           | Table priority group:  P2             |
|Action: SAI_PACKET_ACTION_DONOTDROP  | Action: SAI_PACKET_ACTION_TRAP        |


If P1 > P2: action is "copy to CPU"


If P2 > P1: action is "TRAP"

### Case 5

DENY action may be implemented as COPY_CANCEL + DROP. A higher-priority DONOTDROP action will cancel the DROP action only, resulting action as COPY_CANCEL.

| ACL in table T1                     | ACL in table T2                       |
|-------------------------------------|---------------------------------------|
| Table priority group:  P1           | Table priority group:  P2             |
|Action: SAI_PACKET_ACTION_DONOTDROP  | Action: SAI_PACKET_ACTION_DENY        |


If P1 > P2: action is "COPY_CANCEL"


If P2 > P1: action is "DENY"

