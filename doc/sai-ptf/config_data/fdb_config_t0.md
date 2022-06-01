# Sample T0 Configurations and data for FDB
- [Sample T0 Configurations and data for FDB](#sample-t0-configurations-and-data-for-fdb)
  - [Overriew](#overriew)
  - [FDB Configuration](#fdb-configuration)
    - [MAC Table](#mac-table)
  - [APIs for FDB configuration](#apis-for-fdb-configuration)
    - [Create FDB Entries](#create-fdb-entries)
    - [Get FDB entties](#get-fdb-entties)
    - [Clear FDB entries in a VLAN](#clear-fdb-entries-in-a-vlan)
## Overriew
This document describes the sample configuration data, sample test data/packet, and APIs to make the configuration that is used around FDB testing.
**Note: This configuration focused on T0 topology.**

## FDB Configuration
### MAC Table
The MAC Table for VLAN L2 forwarding as below
|Name|MAC|PORT|VLAN|HostIf|
|-|-|-|-|-|
|mac0|mac0-00:00:00:00:00:11|Port0||Ethernet0|
|mac1-8  |00:11:11:11:11:11 - 00:88:88:88:88:88|Port1-8|1000|Ethernet4-Ethernet32|
|mac9-16 |00:99:99:99:99:99 - 01:66:66:66:66:66|Port9-16|2000|Ethernet36-Ethernet64|
|mac17-mac31 |01:77:77:77:77:77 - 03:11:11:11:11:11|Port17-31||Ethernet68-Ethernet124|


## APIs for FDB configuration
APIs relate to FDB and FDB attributes.

**P.S. There are just some sample APIs, for more attributes please refer to https://github.com/richardyu-ms/SAI/blob/support_ptf_sai_build/inc/saifdb.h**

### Create FDB Entries

- Add static FDB entry in the FDB table
   
  In the sample code below, we can see how to create an FDB entry.

  This FDB entry is a static entry, it will ``forward`` packet when the packet is with a mac1 on port1.
   ```python

   sai_thrift_fdb_entry_t(switch_id=self.switch_id, mac_address=mac1)
   sai_thrift_create_fdb_entry(
                self.client,
                fdb_entry,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=Port1,
                packet_action=SAI_PACKET_ACTION_FORWARD)
   ```

- Add static FDB entry with **VLAN ID** in the FDB table
   
  In the sample code below, we can see how to create an FDB entry.

  This FDB entry is a static entry, it will ``forward`` packet when the packet is with a mac1 on port1.
   ```python

   sai_thrift_fdb_entry_t(switch_id=self.switch_id, mac_address=mac1, bv_id=self.vlan_oid)
   sai_thrift_create_fdb_entry(
                self.client,
                fdb_entry,
                type=SAI_FDB_ENTRY_TYPE_STATIC,
                bridge_port_id=Port1,
                packet_action=SAI_PACKET_ACTION_FORWARD)
   ```

### Get FDB entties
  ```python
   attr = sai_thrift_get_switch_attribute(
          self.client, available_fdb_entry=True)
   max_fdb_entry = attr["available_fdb_entry"]
  ```


### Clear FDB entries in a VLAN
  Clear the learned FDB entries with the VLAN id object id
  ```python
  sai_thrift_flush_fdb_entries(
                    self.client,
                    bv_id=self.vlan_oid,
                    entry_type=SAI_FDB_ENTRY_TYPE_DYNAMIC)
  ```