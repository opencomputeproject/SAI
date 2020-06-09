# MPLS Outsegment

Existing [MPLS SAI Specification](https://github.com/opencomputeproject/SAI/blob/master/doc/MPLS/SAI-Proposal-MPLS-ver4.docx) specifies MPLS Outsegment as follows :

>1.2	OutSegment(SAI next hop)
>An OutSegment defines outgoing parameters from Label Switch Router to a MPLS network. This >object represents outgoing top MPLS label. This object performs PUSH and PHP operations.
>
>OutSegment has following parameters:
>-	labelStack object
>-	Next IP hop
>OutSegment can be bound to InSegment LSR flow  , and can be bound to route in in order create ingress LER .

Howevever this definition lacks in defining two other important behaviors
- TTL of Outsegment
- QoS attribute of Outsegment

It also skips mention of SWAP operation which is needed by transit label switch routers, as SWAP operation may update label stack.

The document attempts to formalize Outsegment further to provide ability to program these behaviors.

# Label Switched Path Models

MPLS tunnel is also called as label switched path.  TTL and QoS attribute of Outsegment depends label switched path models
- Uniform Model
- Short Pipe Model
- Pipe Model

# TTL and QoS Processing for Uniform Model LSPs

[RFC 3443](https://tools.ietf.org/html/rfc3443#section-3.1) describes TTL processing for this model.

[RFC 3270](https://tools.ietf.org/html/rfc3270#section-2.6.3) describes this QoS attribute processing for this model.

At ingress node
- outer TTL = inner TTL - 1
- outer traffic class = inner traffic class

```
nexthop = {
    {SAI_NEXT_HOP_ATTR_TYPE, SAI_NEXT_HOP_TYPE_MPLS},
    {SAI_NEXT_HOP_ATTR_IP, <next hop ip>},
    {SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID, <interface object>},
    {SAI_NEXT_HOP_ATTR_LABELSTACK, <outgoing label stack>},
    _{SAI_NEXT_HOP_ATTR_OUTSEG_TYPE, SAI_OUTSEG_TYPE_PUSH}_,
    _{SAI_NEXT_HOP_ATTR_OUTSEG_TTL_MODE, SAI_OUTSEG_TTL_MODE_UNIFORM}_,
    _{SAI_NEXT_HOP_ATTR_OUTSEG_EXP_MODE, SAI_OUTSEG_EXP_MODE_UNIFORM}_,
}
```

At egress node (with or without PHP)
- inner TTL = outer TTL - 1
- inner traffic class = outer traffic class
- PHB is applied based on outer header

```
inseg_entry = {
    {SAI_INSEG_ENTRY_ATTR_NUM_OF_POP, 1},
    {SAI_INSEG_ENTRY_ATTR_NEXT_HOP_ID, <mpls-rif or next hop>},
    _{SAI_INSEG_ENTRY_ATTR_POP_TTL_MODE, SAI_INSEG_ENTRY_POP_TTL_MODE_UNIFORM}_,
    _{SAI_INSEG_ENTRY_ATTR_POP_QOS_MODE, SAI_INSEG_ENTRY_POP_QOS_MODE_UNIFORM}_,
}
```

# TTL and QoS Processing for variation of Pipe Model LSPs

## TTL Processing for Pipe Model LSPs (without PHP only)
Described in [RFC3443](https://tools.ietf.org/html/rfc3443#section-3.3)
## TTL Processing for Short Pipe Model LSPs (with or without PHP)
Described in [RFC3443](https://tools.ietf.org/html/rfc3443#section-3.2.1)
## QoS Attribute processing for Pipe Model (without PHP only)
Described in [RFC3270](https://tools.ietf.org/html/rfc3270#section-2.6.2)
## QoS Attribute processing for Short Pipe Model (with PHP only)
Described in [RFC3270](https://tools.ietf.org/html/rfc3270#section-2.6.2.1)

Following configuration applies to all above models

At ingress node
- inner TTL = inner TTL - 1
- outer TTL = new TTL
- outer traffic class = new traffic class

```
nexthop = {
    {SAI_NEXT_HOP_ATTR_TYPE, SAI_NEXT_HOP_TYPE_MPLS},
    {SAI_NEXT_HOP_ATTR_IP, <next hop ip>},
    {SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID, <interface object>},
    {SAI_NEXT_HOP_ATTR_LABELSTACK, <outgoing label stack>},
    _{SAI_NEXT_HOP_ATTR_OUTSEG_TYPE, SAI_OUTSEG_TYPE_PUSH}_,
    _{SAI_NEXT_HOP_ATTR_OUTSEG_TTL_MODE, SAI_OUTSEG_TTL_MODE_PIPE}_,
    _{SAI_NEXT_HOP_ATTR_OUTSEG_TTL_VALUE, <new TTL value>}_,
    _{SAI_NEXT_HOP_ATTR_OUTSEG_EXP_MODE, SAI_OUTSEG_EXP_MODE_PIPE}_,
    _{SAI_NEXT_HOP_ATTR_OUTSEG_EXP_VALUE, <new EXP value>}_,
}
```

At egress node (with or without PHP)
- inner TTL = inner TTL - 1
- PHB is applied based on outer traffic class
- inner traffic class is untouched

```
inseg_entry = {
    {SAI_INSEG_ENTRY_ATTR_NUM_OF_POP, 1},
    {SAI_INSEG_ENTRY_ATTR_NEXT_HOP_ID, <mpls-rif or next hop>},
    _{SAI_INSEG_ENTRY_ATTR_POP_TTL_MODE, SAI_INSEG_ENTRY_POP_TTL_MODE_PIPE}_,
    _{SAI_INSEG_ENTRY_ATTR_POP_QOS_MODE, SAI_INSEG_ENTRY_POP_QOS_MODE_PIPE}_,
}
```

# TTL and QoS Processing for intermediate label switch routers

- Transit nodes will uniformly decrement TTL
- Use either incoming label (L-LSP) or incoming label(E-LSP) to apply PHB.

Extending Outsegment notion to also "SWAP" operation, where incoming label is swapped with either same or different label.

For this SAI_OUTSEG_TYPE_SWAP is used.


```
inseg_entry = {
    {SAI_INSEG_ENTRY_ATTR_NUM_OF_POP, 1},
    {SAI_INSEG_ENTRY_ATTR_NEXT_HOP_ID, nexthop},
    {SAI_INSEG_ENTRY_ATTR_PSC_TYPE,
        SAI_INSEG_ENTRY_PSC_TYPE_LLSP or SAI_INSEG_ENTRY_PSC_TYPE_ELSP
    },
}
```

Note than num pop is 1 even if label being swapped with same value.

```
nexthop = {
    {SAI_NEXT_HOP_ATTR_TYPE, SAI_NEXT_HOP_TYPE_MPLS},
    {SAI_NEXT_HOP_ATTR_IP, <next hop ip>},
    {SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID, <interface object>},
    {SAI_NEXT_HOP_ATTR_LABELSTACK, <out going label>},
    _{SAI_NEXT_HOP_ATTR_OUTSEG_TYPE, SAI_OUTSEG_TYPE_SWITCH}_,
}
```

Note that label stack attribute SAI_NEXT_HOP_ATTR_LABELSTACK is a list of length 1

# TTL and QoS Processing for intermediate label switch routers with Binding SID

Intermediate label switch routers swaps top label with label stack.

Exactly same as above except next hop attribute SAI_NEXT_HOP_ATTR_OUTSEG_TYPE value of SAI_OUTSEG_TYPE_PUSH instead of SAI_OUTSEG_TYPE_SWAP.

```
inseg_entry = {
    {SAI_INSEG_ENTRY_ATTR_NUM_OF_POP, 1},
    {SAI_INSEG_ENTRY_ATTR_NEXT_HOP_ID, nexthop},
    {SAI_INSEG_ENTRY_ATTR_PSC_TYPE,
        SAI_INSEG_ENTRY_PSC_TYPE_LLSP or SAI_INSEG_ENTRY_PSC_TYPE_ELSP
    },
}
```
Note than num pop is 1 even if label being swapped with a label stack.

Pipe mode next hop for Binding SID
```
nexthop = {
    {SAI_NEXT_HOP_ATTR_TYPE, SAI_NEXT_HOP_TYPE_MPLS},
    {SAI_NEXT_HOP_ATTR_IP, <next hop ip>},
    {SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID, <interface object>},
    {SAI_NEXT_HOP_ATTR_LABELSTACK, <outgoing label stack>},
    _{SAI_NEXT_HOP_ATTR_OUTSEG_TYPE, SAI_OUTSEG_TYPE_PUSH}_,
    _{SAI_NEXT_HOP_ATTR_OUTSEG_TTL_MODE, SAI_OUTSEG_TTL_MODE_PIPE}_,
    _{SAI_NEXT_HOP_ATTR_OUTSEG_TTL_VALUE, <new TTL value>}_,
    _{SAI_NEXT_HOP_ATTR_OUTSEG_EXP_MODE, SAI_OUTSEG_EXP_MODE_PIPE}_,
    _{SAI_NEXT_HOP_ATTR_OUTSEG_EXP_VALUE, <new EXP value>}_,
}
```

Uniform mode next hop for Binding SID
```
nexthop = {
    {SAI_NEXT_HOP_ATTR_TYPE, SAI_NEXT_HOP_TYPE_MPLS},
    {SAI_NEXT_HOP_ATTR_IP, <next hop ip>},
    {SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID, <interface object>},
    {SAI_NEXT_HOP_ATTR_LABELSTACK, <outgoing label stack>},
    _{SAI_NEXT_HOP_ATTR_OUTSEG_TYPE, SAI_OUTSEG_TYPE_PUSH}_,
    _{SAI_NEXT_HOP_ATTR_OUTSEG_TTL_MODE, SAI_OUTSEG_TTL_MODE_UNIFORM}_,
    _{SAI_NEXT_HOP_ATTR_OUTSEG_EXP_MODE, SAI_OUTSEG_EXP_MODE_UNIFORM}_,
}
```

Note that label stack attribute SAI_NEXT_HOP_ATTR_LABELSTACK is a list of length 1


### SAI_OUTSEG_TYPE_SWAP vs SAI_OUTSEG_TYPE_PUSH

Two Outseg types indicate that application must have two separate next hops even if the label stack is same. This is because SAI_OUTSEG_TYPE_PUSH requires TTL and QoS treatment as per above describe models, where as SAI_OUTSEG_TYPE_SWAP uniformly decrements TTL and QoS is based on if incoming label map entry is either E-LSP or L-LSP.

Consider following topology
```
--- A --- B --- C---
```

- Downstream node C is advertises a prefix 1.1.1.0/24 reachable with label 100 to node B.
- Node B propogates reachability information for prefix 1.1.1.0/24 with label 100 to node A.
- Node A if receives MPLS traffic with label 100,
  - Node A will act as transit LSR and swap top label with 100 and forward to B
  - For this Node A programs next hop having attribute SAI_NEXT_HOP_ATTR_OUTSEG_TYPE of value SAI_OUTSEG_TYPE_SWAP.
- Node A if receives IP traffic with destination prefix 1.1.1.0/24,
  - Node A will act as ingress LSR and push label 100
  - For this Node A programs next hop having attribute SAI_NEXT_HOP_ATTR_OUTSEG_TYPE of value SAI_OUTSEG_TYPE_PUSH

An important point to note that SAI Adapter Host must program two types of next hops even though both next hops have same label stack one for ingress LER operation and one for transit LSR operation. This is done to simplify implementation and enable programming of proper TTL and QoS treatment only for ingress LER operation.
