# [SAI] ERSPAN Mirror Session Enhancements
-------------------------------------------------------------------------------
 Title       | ERSPAN Mirror session Encap types
-------------|-----------------------------------------------------------------
 Authors     | Pooja Panara, Ravindranath C K (Marvell)
 Status      | In review
 Type        | Standards track
 Created     | 2025-01-30
 SAI-Version | 1.16
-------------------------------------------------------------------------------

## 1.0  Introduction

SAI `mirror_session` supports **Local SPAN**, **Remote SPAN**, **Enhanced Remote SPAN** and **Sflow**.  

- **Local SPAN**: Sends mirrored traffic to a destination port within the same switch.  
- **Remote SPAN**: Sends mirrored packets to a destination over an L2 network.  
- **Enhanced Remote SPAN**: Sends mirrored packets to be transmitted across an L3 network. Mirrored packet will be sent over GRE tunnel.
- **Sflow**: Sends mirrored packets over Sflow tunnel.

## 2.0  Problem Statement
Enhancing mirrored packets with additional information improves network administration and troubleshooting.  Embedding information is possible in Enhanced remote span(ERSPAN). There are  3 types of ERSPAN defined in
[ERSPAN Draft - IETF](https://datatracker.ietf.org/doc/html/draft-foschiano-erspan-03). 


- **ERSPAN-I**:

    - Already supported by SAI with encap_type = SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL.

    - Mirror packet format:

    ```mermaid
    ---
    title: Mirror Packet Format with ERSPAN-I
    ---
    block-beta
    A["L2 Ethernet Header(14B)"] B["IP Header(20B)"] C["GRE Header(4B)"] D["Original Mirrored Packet"]
    ```
    - GRE header format with ERSPAN-I 
    ```mermaid
    ---
    title: GRE Packet Format with ERSPAN-I
    ---
    packet-beta
    0: "C=0"
    1: "0"
    2: "K=0"
    3: "S=0"
    4-12: "Reserved = 0"
    13-15: "Version = 0"
    16-31: "Protocol type = 0x88be"
    ```
    - The first 16 bits of the GRE header are always set to 0 or remain unused. These bits can be repurposed to store specific information as needed. 


- **ERSPAN-II**:

    - Not supported by SAI.

    - ERSPAN-II defines 8 byte header to encode information about mirrored packet along with GRE header.

    - Mirror packet format:

    ```mermaid
    ---
    title: Mirror Packet Format with ERSPAN-II
    ---
    block-beta
    A["L2 Ethernet Header(14B)"] B["IP Header(20B)"] C["GRE Header(8B)"] D["ERSPAN-II Header(8B)"] E["Original Mirrored Packet"]
    ```

    - ESPAN-II uses below GRE header followed by ERSPAN-II header:
    ```mermaid
    ---
    title: GRE Packet Format with ERSPAN-II
    ---
    packet-beta
    0: "C=0"
    1: "0"
    2: "K=0"
    3: "S=1"
    4-12: "Reserved = 0"
    13-15: "Version = 0"
    16-31: "Protocol Type = 0x88be"
    32-63: "Sequence Number (increments per packet per session)"
    ```

    ```mermaid
    ---
    title: ERSPAN-II Header format
    ---

    packet-beta
    0-3: "Version"
    4-15: "Vlan"
    16-18: "COS"
    19-20: "En"
    21: "T"
    22-31: "Session-id"
    32-43: "Reserved"
    44-63: "Index"
    ```

- **ERSPAN-III**:

    - Not supported by SAI.

    - ERSPAN-III defines 12 byte header to encode more information about mirrored packet along with GRE header. 
    - Mirror packet format:
    ```mermaid
    ---
    title: Mirror Packet Format with ERSPAN-III
    ---
    block-beta
    A["L2 Ethernet Header(14B)"] B["IP Header(20B)"] C["GRE Header(4B)"] D["ERSPAN-III Header(12B)"] E["Original Mirrored Packet"]
    ```

    - ESPAN-III uses below GRE header followed by ERSPAN-III header

    ```mermaid
    ---
    title: GRE Packet Format with ERSPAN-III
    ---
    packet-beta
    0: "C=0"
    1: "0"
    2: "k=0"
    3: "S=0"
    4-12: "Reserved = 0"
    13-15: "Version = 0"
    16-31: "Protocol Type = 0x22eb"
    ```
    ```mermaid
    ---
    title: ERSPAN-III Header format
    ---

    packet-beta
    0-3: "Version"
    4-15: "Vlan"
    16-18: "COS"
    19-20: "BSO"
    21: "T"
    22-31: "Session-id"
    32-63: "Timestamp"
    64-79: "STG"
    80: "P"
    81-86: "FT"
    87-92: "Hw ID"
    93: "D"
    94-95: "GRA"

    ```

## 3.0 Proposed SAI Enhancement
1) New enum fields have been added in sai_erspan_encapsulation_type_t to support GRE with ERSPAN Type II and Type III headers. 
   ```c
    /**
     * @brief L3 GRE Tunnel Encapsulation | L2 Ethernet header | IP header | GRE header | ERSPAN (8B) | Original mirrored packet
     */
    SAI_ERSPAN_ENCAPSULATION_TYPE_II,

    /**
     * @brief L3 GRE Tunnel Encapsulation | L2 Ethernet header | IP header | GRE header | ERSPAN (12B) + optional headers | Original mirrored packet
     */.
    SAI_ERSPAN_ENCAPSULATION_TYPE_III,
   ```
2) Attribute to configure session_id of ERSPAN-II and ERSPAN-III header:
   ```c
   /**
     * @brief Unique identifier for each ERSPAN mirror session (10 bits).
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     * @validonly SAI_MIRROR_SESSION_ATTR_ERSPAN_ENCAPSULATION_TYPE == SAI_ERSPAN_ENCAPSULATION_TYPE_II or SAI_MIRROR_SESSION_ATTR_ERSPAN_ENCAPSULATION_TYPE == SAI_ERSPAN_ENCAPSULATION_TYPE_III
     */
    SAI_MIRROR_SESSION_ATTR_ERSPAN_SESSION_ID,
   ```
3) Attribute to update first 16 bits of GRE header
   ```c
   /**
     * @brief The first 16 bits of the GRE header.
     *
     * @type sai_uint16_t
     * @flags CREATE_AND_SET
     * @isvlan false
     * @default 0
     * @validonly SAI_MIRROR_SESSION_ATTR_TYPE == SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE
     */
    SAI_MIRROR_SESSION_ATTR_GRE_HEADER_FIRST_16BIT,
   ```

## 4.0 API Example

###  Create mirror session with ERSPAN-II header

```c
...(Existing Attribute)
sai_attr_list[attr_count].id = SAI_MIRROR_SESSION_ATTR_ERSPAN_ENCAPSULATION_TYPE;
sai_attr_list[attr_count++].value.u32 = SAI_ERSPAN_ENCAPSULATION_TYPE_II;
sai_attr_list[attr_count].id = SAI_MIRROR_SESSION_ATTR_ERSPAN_SESSION_ID;
sai_attr_list[attr_count++].value.u16 = 10;

sai_create_mirror_session_fn(
   &ms_erspan_II_oid,
   switch_id,
   attr_count,
   sai_attr_list);
```
###  Create mirror session with ERSPAN-III header

```c
...(Existing Attribute)
sai_attr_list[attr_count].id = SAI_MIRROR_SESSION_ATTR_ERSPAN_ENCAPSULATION_TYPE;
sai_attr_list[attr_count++].value.u32 = SAI_ERSPAN_ENCAPSULATION_TYPE_III;
sai_attr_list[attr_count].id = SAI_MIRROR_SESSION_ATTR_ERSPAN_SESSION_ID;
sai_attr_list[attr_count++].value.u16 = 20;

sai_create_mirror_session_fn(
   &ms_erspan_III_oid,
   switch_id,
   attr_count,
   sai_attr_list);
```
###  Create mirror session with modified GRE header

```c
...(Existing Attribute)
sai_attr_list[attr_count].id = SAI_MIRROR_SESSION_ATTR_ERSPAN_ENCAPSULATION_TYPE;
sai_attr_list[attr_count++].value.u32 = SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL;
sai_attr_list[attr_count].id = SAI_MIRROR_SESSION_ATTR_GRE_HEADER_FIRST_16BIT;
sai_attr_list[attr_count++].value.u16 = 100;

sai_create_mirror_session_fn(
   &ms_gre_oid,
   switch_id,
   attr_count,
   sai_attr_list);
```
