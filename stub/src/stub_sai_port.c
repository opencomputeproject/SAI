/*
 *  Copyright (C) 2014. Mellanox Technologies, Ltd. ALL RIGHTS RESERVED.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 */

#include "sai.h"
#include "stub_sai.h"
#include "assert.h"

#undef  __MODULE__
#define __MODULE__ SAI_PORT

sai_status_t stub_port_fdb_violation_set(_In_ const sai_object_key_t      *key,
                                         _In_ const sai_attribute_value_t *value,
                                         void                             *arg);
sai_status_t stub_port_max_learned_addr_set(_In_ const sai_object_key_t      *key,
                                            _In_ const sai_attribute_value_t *value,
                                            void                             *arg);
sai_status_t stub_port_storm_control_set(_In_ const sai_object_key_t      *key,
                                         _In_ const sai_attribute_value_t *value,
                                         void                             *arg);
sai_status_t stub_port_update_dscp_set(_In_ const sai_object_key_t      *key,
                                       _In_ const sai_attribute_value_t *value,
                                       void                             *arg);
sai_status_t stub_port_state_set(_In_ const sai_object_key_t      *key,
                                 _In_ const sai_attribute_value_t *value,
                                 void                             *arg);
sai_status_t stub_port_media_type_set(_In_ const sai_object_key_t      *key,
                                      _In_ const sai_attribute_value_t *value,
                                      void                             *arg);
sai_status_t stub_port_default_vlan_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg);
sai_status_t stub_port_default_vlan_prio_set(_In_ const sai_object_key_t      *key,
                                             _In_ const sai_attribute_value_t *value,
                                             void                             *arg);
sai_status_t stub_port_ingress_filter_set(_In_ const sai_object_key_t      *key,
                                          _In_ const sai_attribute_value_t *value,
                                          void                             *arg);
sai_status_t stub_port_drop_tags_set(_In_ const sai_object_key_t      *key,
                                     _In_ const sai_attribute_value_t *value,
                                     void                             *arg);
sai_status_t stub_port_internal_loopback_set(_In_ const sai_object_key_t      *key,
                                             _In_ const sai_attribute_value_t *value,
                                             void                             *arg);
sai_status_t stub_port_fdb_learning_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg);
sai_status_t stub_port_mtu_set(_In_ const sai_object_key_t      *key,
                               _In_ const sai_attribute_value_t *value,
                               void                             *arg);
sai_status_t stub_port_speed_set(_In_ const sai_object_key_t      *key,
                                 _In_ const sai_attribute_value_t *value,
                                 void                             *arg);
sai_status_t stub_port_autoneg_set(_In_ const sai_object_key_t      *key,
                                   _In_ const sai_attribute_value_t *value,
                                   void                             *arg);
sai_status_t stub_port_type_get(_In_ const sai_object_key_t   *key,
                                _Inout_ sai_attribute_value_t *value,
                                _In_ uint32_t                  attr_index,
                                _Inout_ vendor_cache_t        *cache,
                                void                          *arg);
sai_status_t stub_port_state_get(_In_ const sai_object_key_t   *key,
                                 _Inout_ sai_attribute_value_t *value,
                                 _In_ uint32_t                  attr_index,
                                 _Inout_ vendor_cache_t        *cache,
                                 void                          *arg);
sai_status_t stub_port_max_learned_addr_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg);
sai_status_t stub_port_fdb_violation_get(_In_ const sai_object_key_t   *key,
                                         _Inout_ sai_attribute_value_t *value,
                                         _In_ uint32_t                  attr_index,
                                         _Inout_ vendor_cache_t        *cache,
                                         void                          *arg);
sai_status_t stub_port_media_type_get(_In_ const sai_object_key_t   *key,
                                      _Inout_ sai_attribute_value_t *value,
                                      _In_ uint32_t                  attr_index,
                                      _Inout_ vendor_cache_t        *cache,
                                      void                          *arg);
sai_status_t stub_port_storm_control_get(_In_ const sai_object_key_t   *key,
                                         _Inout_ sai_attribute_value_t *value,
                                         _In_ uint32_t                  attr_index,
                                         _Inout_ vendor_cache_t        *cache,
                                         void                          *arg);
sai_status_t stub_port_update_dscp_get(_In_ const sai_object_key_t   *key,
                                       _Inout_ sai_attribute_value_t *value,
                                       _In_ uint32_t                  attr_index,
                                       _Inout_ vendor_cache_t        *cache,
                                       void                          *arg);
sai_status_t stub_port_hw_lanes_get(_In_ const sai_object_key_t   *key,
                                    _Inout_ sai_attribute_value_t *value,
                                    _In_ uint32_t                  attr_index,
                                    _Inout_ vendor_cache_t        *cache,
                                    void                          *arg);
sai_status_t stub_port_supported_breakout_get(_In_ const sai_object_key_t   *key,
                                              _Inout_ sai_attribute_value_t *value,
                                              _In_ uint32_t                  attr_index,
                                              _Inout_ vendor_cache_t        *cache,
                                              void                          *arg);
sai_status_t stub_port_current_breakout_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg);
sai_status_t stub_port_speed_get(_In_ const sai_object_key_t   *key,
                                 _Inout_ sai_attribute_value_t *value,
                                 _In_ uint32_t                  attr_index,
                                 _Inout_ vendor_cache_t        *cache,
                                 void                          *arg);
sai_status_t stub_port_autoneg_get(_In_ const sai_object_key_t   *key,
                                   _Inout_ sai_attribute_value_t *value,
                                   _In_ uint32_t                  attr_index,
                                   _Inout_ vendor_cache_t        *cache,
                                   void                          *arg);
sai_status_t stub_port_default_vlan_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg);
sai_status_t stub_port_default_vlan_prio_get(_In_ const sai_object_key_t   *key,
                                             _Inout_ sai_attribute_value_t *value,
                                             _In_ uint32_t                  attr_index,
                                             _Inout_ vendor_cache_t        *cache,
                                             void                          *arg);
sai_status_t stub_port_ingress_filter_get(_In_ const sai_object_key_t   *key,
                                          _Inout_ sai_attribute_value_t *value,
                                          _In_ uint32_t                  attr_index,
                                          _Inout_ vendor_cache_t        *cache,
                                          void                          *arg);
sai_status_t stub_port_drop_tags_get(_In_ const sai_object_key_t   *key,
                                     _Inout_ sai_attribute_value_t *value,
                                     _In_ uint32_t                  attr_index,
                                     _Inout_ vendor_cache_t        *cache,
                                     void                          *arg);
sai_status_t stub_port_internal_loopback_get(_In_ const sai_object_key_t   *key,
                                             _Inout_ sai_attribute_value_t *value,
                                             _In_ uint32_t                  attr_index,
                                             _Inout_ vendor_cache_t        *cache,
                                             void                          *arg);
sai_status_t stub_port_fdb_learning_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg);
sai_status_t stub_port_mtu_get(_In_ const sai_object_key_t   *key,
                               _Inout_ sai_attribute_value_t *value,
                               _In_ uint32_t                  attr_index,
                               _Inout_ vendor_cache_t        *cache,
                               void                          *arg);

static const sai_attribute_entry_t        port_attribs[] = {
    { SAI_PORT_ATTR_TYPE, false, false, false, true,
      "Port type", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_OPER_STATUS, false, false, false, true,
      "Port operational status", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_HW_LANE_LIST, false, false, false, true,
      "Port HW lane list", SAI_ATTR_VAL_TYPE_U32LIST },
    { SAI_PORT_ATTR_SUPPORTED_BREAKOUT_MODE, false, false, false, true,
      "Port supported breakout modes", SAI_ATTR_VAL_TYPE_S32LIST },
    { SAI_PORT_ATTR_CURRENT_BREAKOUT_MODE, false, false, false, true,
      "Port current breakout mode", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_SPEED, false, false, true, true,
      "Port speed", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_PORT_ATTR_AUTO_NEG_MODE, false, false, true, true,
     "Port autoneg mode", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_ADMIN_STATE, false, false, true, true,
      "Port admin state", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_MEDIA_TYPE, false, false, true, true,
      "Port media type", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_PORT_VLAN_ID, false, false, true, true,
      "Port default vlan", SAI_ATTR_VAL_TYPE_U16 },
    { SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY, false, false, true, true,
      "Port default vlan priority", SAI_ATTR_VAL_TYPE_U8 },
    { SAI_PORT_ATTR_INGRESS_FILTERING, false, false, true, true,
      "Port ingress filtering", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_DROP_UNTAGGED, false, false, true, true,
      "Port drop untageed", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_DROP_TAGGED, false, false, true, true,
      "Port drop tageed", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_INTERNAL_LOOPBACK, false, false, true, true,
      "Port internal loopback", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_FDB_LEARNING, false, false, true, true,
      "Port fdb learning", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_UPDATE_DSCP, false, false, true, true,
      "Port update DSCP", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_MTU, false, false, true, true,
      "Port mtu", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID, false, false, true, true,
      "Port flood storm control", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID, false, false, true, true,
      "Port broadcast storm control", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID, false, false, true, true,
      "Port multicast storm control", SAI_ATTR_VAL_TYPE_BOOL },
    { SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL, false, false, true, true,
      "Port global flow control", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_MAX_LEARNED_ADDRESSES, false, false, true, true,
      "Port max learned addresses", SAI_ATTR_VAL_TYPE_U32 },
    { SAI_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION, false, false, true, true,
      "Port fdb learning limit violation", SAI_ATTR_VAL_TYPE_S32 },
    { SAI_PORT_ATTR_INGRESS_MIRROR_SESSION, false, false, true, true,
      "Port ingress mirror session", SAI_ATTR_VAL_TYPE_OBJLIST },
    { SAI_PORT_ATTR_EGRESS_MIRROR_SESSION, false, false, true, true,
      "Port egress mirror session", SAI_ATTR_VAL_TYPE_OBJLIST },
    { SAI_PORT_ATTR_INGRESS_SAMPLEPACKET_ENABLE, false, false, true, true,
      "Port ingress samplepacket enable", SAI_ATTR_VAL_TYPE_OID },
    { SAI_PORT_ATTR_EGRESS_SAMPLEPACKET_ENABLE, false, false, true, true,
      "Port egress samplepacket enable", SAI_ATTR_VAL_TYPE_OID },
    { END_FUNCTIONALITY_ATTRIBS_ID, false, false, false, false,
      "", SAI_ATTR_VAL_TYPE_UNDETERMINED }
};
static const sai_vendor_attribute_entry_t port_vendor_attribs[] = {
    { SAI_PORT_ATTR_TYPE,
      { false, false, false, true },
      { false, false, false, true },
      stub_port_type_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_OPER_STATUS,
      { false, false, false, true },
      { false, false, false, true },
      stub_port_state_get, (void*)SAI_PORT_ATTR_OPER_STATUS,
      NULL, NULL },
    { SAI_PORT_ATTR_HW_LANE_LIST,
      { false, false, false, true },
      { false, false, false, true },
      stub_port_hw_lanes_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_SUPPORTED_BREAKOUT_MODE,
      { false, false, false, true },
      { false, false, false, true },
      stub_port_supported_breakout_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_CURRENT_BREAKOUT_MODE,
      { false, false, false, true },
      { false, false, false, true },
      stub_port_current_breakout_get, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_SPEED,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_speed_get, NULL,
      stub_port_speed_set, NULL },
    { SAI_PORT_ATTR_AUTO_NEG_MODE,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_autoneg_get, NULL,
      stub_port_autoneg_set, NULL },
    { SAI_PORT_ATTR_ADMIN_STATE,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_state_get, (void*)SAI_PORT_ATTR_ADMIN_STATE,
      stub_port_state_set, NULL },
    { SAI_PORT_ATTR_MEDIA_TYPE,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_media_type_get, NULL,
      stub_port_media_type_set, NULL },
    { SAI_PORT_ATTR_PORT_VLAN_ID,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_default_vlan_get, NULL,
      stub_port_default_vlan_set, NULL },
    { SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_default_vlan_prio_get, NULL,
      stub_port_default_vlan_prio_set, NULL },
    { SAI_PORT_ATTR_INGRESS_FILTERING,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_ingress_filter_get, NULL,
      stub_port_ingress_filter_set, NULL },
    { SAI_PORT_ATTR_DROP_UNTAGGED,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_drop_tags_get, (void*)SAI_PORT_ATTR_DROP_UNTAGGED,
      stub_port_drop_tags_set, (void*)SAI_PORT_ATTR_DROP_UNTAGGED },
    { SAI_PORT_ATTR_DROP_TAGGED,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_drop_tags_get, (void*)SAI_PORT_ATTR_DROP_TAGGED,
      stub_port_drop_tags_set, (void*)SAI_PORT_ATTR_DROP_TAGGED },
    { SAI_PORT_ATTR_INTERNAL_LOOPBACK,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_internal_loopback_get, NULL,
      stub_port_internal_loopback_set, NULL },
    { SAI_PORT_ATTR_FDB_LEARNING,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_fdb_learning_get, NULL,
      stub_port_fdb_learning_set, NULL },
    { SAI_PORT_ATTR_UPDATE_DSCP,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_update_dscp_get, NULL,
      stub_port_update_dscp_set, NULL },
    { SAI_PORT_ATTR_MTU,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_mtu_get, NULL,
      stub_port_mtu_set, NULL },
    { SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_storm_control_get, (void*)SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID,
      stub_port_storm_control_set, (void*)SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID },
    { SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_storm_control_get, (void*)SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID,
      stub_port_storm_control_set, (void*)SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID },
    { SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_storm_control_get, (void*)SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID,
      stub_port_storm_control_set, (void*)SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID },
    { SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_MAX_LEARNED_ADDRESSES,
      { false, false, true, true },
      { false, false, true, true },
      stub_port_max_learned_addr_get, NULL,
      stub_port_max_learned_addr_set, NULL },
    { SAI_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION,
      { false, false, false, false },
      { false, false, true, true },
      stub_port_fdb_violation_get, NULL,
      stub_port_fdb_violation_set, NULL },
    { SAI_PORT_ATTR_INGRESS_MIRROR_SESSION,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_EGRESS_MIRROR_SESSION,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_INGRESS_SAMPLEPACKET_ENABLE,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL },
    { SAI_PORT_ATTR_EGRESS_SAMPLEPACKET_ENABLE,
      { false, false, false, false },
      { false, false, true, true },
      NULL, NULL,
      NULL, NULL }
};

/* Admin Mode [bool] */
sai_status_t stub_port_state_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value, void *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Update DSCP of outgoing packets [bool] */
sai_status_t stub_port_update_dscp_set(_In_ const sai_object_key_t      *key,
                                       _In_ const sai_attribute_value_t *value,
                                       void                             *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* [bool] (default to FALSE) */
sai_status_t stub_port_storm_control_set(_In_ const sai_object_key_t      *key,
                                         _In_ const sai_attribute_value_t *value,
                                         void                             *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Action for packets with unknown source mac address
 * when FDB learning limit is reached.
 * [sai_packet_action_t] (default to SAI_PACKET_ACTION_DROP) */
sai_status_t stub_port_fdb_violation_set(_In_ const sai_object_key_t      *key,
                                         _In_ const sai_attribute_value_t *value,
                                         void                             *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Maximum number of learned MAC addresses [uint32_t] */
sai_status_t stub_port_max_learned_addr_set(_In_ const sai_object_key_t      *key,
                                            _In_ const sai_attribute_value_t *value,
                                            void                             *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Media Type [sai_port_media_type_t] */
sai_status_t stub_port_media_type_set(_In_ const sai_object_key_t      *key,
                                      _In_ const sai_attribute_value_t *value,
                                      void                             *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default VLAN [sai_vlan_id_t]
 *   Untagged ingress frames are tagged with default VLAN
 */
sai_status_t stub_port_default_vlan_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default VLAN Priority [uint8_t]
 *  (default to 0) */
sai_status_t stub_port_default_vlan_prio_set(_In_ const sai_object_key_t      *key,
                                             _In_ const sai_attribute_value_t *value,
                                             void                             *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Ingress Filtering (Drop Frames with Unknown VLANs) [bool] */
sai_status_t stub_port_ingress_filter_set(_In_ const sai_object_key_t      *key,
                                          _In_ const sai_attribute_value_t *value,
                                          void                             *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Dropping of untagged frames on ingress [bool] */
/* Dropping of tagged frames on ingress [bool] */
sai_status_t stub_port_drop_tags_set(_In_ const sai_object_key_t      *key,
                                     _In_ const sai_attribute_value_t *value,
                                     void                             *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    assert((SAI_PORT_ATTR_DROP_UNTAGGED == (int64_t)arg) || (SAI_PORT_ATTR_DROP_TAGGED == (int64_t)arg));

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Internal loopback control [sai_port_loopback_mode_t] */
sai_status_t stub_port_internal_loopback_set(_In_ const sai_object_key_t      *key,
                                             _In_ const sai_attribute_value_t *value,
                                             void                             *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    switch (value->s32) {
    case SAI_PORT_INTERNAL_LOOPBACK_NONE:
        break;

    case SAI_PORT_INTERNAL_LOOPBACK_PHY:
        break;

    case SAI_PORT_INTERNAL_LOOPBACK_MAC:
        break;

    default:
        STUB_LOG_ERR("Invalid port internal loopback value %d\n", value->s32);
        return SAI_STATUS_INVALID_ATTR_VALUE_0;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* FDB Learning mode [sai_port_fdb_learning_mode_t] */
sai_status_t stub_port_fdb_learning_set(_In_ const sai_object_key_t      *key,
                                        _In_ const sai_attribute_value_t *value,
                                        void                             *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    switch (value->s32) {
    case SAI_PORT_LEARN_MODE_DISABLE:
        break;

    case SAI_PORT_LEARN_MODE_HW:
        break;

    case SAI_PORT_LEARN_MODE_DROP:
    case SAI_PORT_LEARN_MODE_CPU_TRAP:
    case SAI_PORT_LEARN_MODE_CPU_LOG:
        break;

    default:
        STUB_LOG_ERR("Invalid port fdb learning mode %d\n", value->s32);
        return SAI_STATUS_INVALID_ATTR_VALUE_0;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* MTU [uint32_t] */
sai_status_t stub_port_mtu_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value, void *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Speed in Mbps [uint32_t] */
sai_status_t stub_port_speed_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value, void *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Autoneg mode [bool] */
sai_status_t stub_port_autoneg_set(_In_ const sai_object_key_t *key, _In_ const sai_attribute_value_t *value, void *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Port type [sai_port_type_t] */
sai_status_t stub_port_type_get(_In_ const sai_object_key_t   *key,
                                _Inout_ sai_attribute_value_t *value,
                                _In_ uint32_t                  attr_index,
                                _Inout_ vendor_cache_t        *cache,
                                void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->s32 = SAI_PORT_TYPE_LOGICAL;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Maximum number of learned MAC addresses [uint32_t] */
sai_status_t stub_port_max_learned_addr_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->u32 = 100000;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Action for packets with unknown source mac address
 * when FDB learning limit is reached.
 * [sai_packet_action_t] (default to SAI_PACKET_ACTION_DROP) */
sai_status_t stub_port_fdb_violation_get(_In_ const sai_object_key_t   *key,
                                         _Inout_ sai_attribute_value_t *value,
                                         _In_ uint32_t                  attr_index,
                                         _Inout_ vendor_cache_t        *cache,
                                         void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->s32 = SAI_PACKET_ACTION_DROP;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Media Type [sai_port_media_type_t] */
sai_status_t stub_port_media_type_get(_In_ const sai_object_key_t   *key,
                                      _Inout_ sai_attribute_value_t *value,
                                      _In_ uint32_t                  attr_index,
                                      _Inout_ vendor_cache_t        *cache,
                                      void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->s32 = SAI_PORT_MEDIA_TYPE_QSFP_COPPER;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Update DSCP of outgoing packets [bool] */
sai_status_t stub_port_update_dscp_get(_In_ const sai_object_key_t   *key,
                                       _Inout_ sai_attribute_value_t *value,
                                       _In_ uint32_t                  attr_index,
                                       _Inout_ vendor_cache_t        *cache,
                                       void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->booldata = false;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* [bool] (default to FALSE) */
sai_status_t stub_port_storm_control_get(_In_ const sai_object_key_t   *key,
                                         _Inout_ sai_attribute_value_t *value,
                                         _In_ uint32_t                  attr_index,
                                         _Inout_ vendor_cache_t        *cache,
                                         void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->booldata = false;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Operational Status [sai_port_oper_status_t] */
/* Admin Mode [bool] */
sai_status_t stub_port_state_get(_In_ const sai_object_key_t   *key,
                                 _Inout_ sai_attribute_value_t *value,
                                 _In_ uint32_t                  attr_index,
                                 _Inout_ vendor_cache_t        *cache,
                                 void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    assert((SAI_PORT_ATTR_OPER_STATUS == (int64_t)arg) || (SAI_PORT_ATTR_ADMIN_STATE == (int64_t)arg));

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    if (SAI_PORT_ATTR_OPER_STATUS == (int64_t)arg) {
        value->s32 = SAI_PORT_OPER_STATUS_UP;
    } else {
        value->booldata = true;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Hardware Lane list [sai_u32_list_t] */
sai_status_t stub_port_hw_lanes_get(_In_ const sai_object_key_t   *key,
                                    _Inout_ sai_attribute_value_t *value,
                                    _In_ uint32_t                  attr_index,
                                    _Inout_ vendor_cache_t        *cache,
                                    void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;
    uint32_t     hw_lane;


    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    hw_lane = port_id;

    status = stub_fill_u32list(&hw_lane, 1, &value->u32list);

    STUB_LOG_EXIT();
    return status;
}

/* Breakout mode(s) supported [sai_s32_list_t] */
sai_status_t stub_port_supported_breakout_get(_In_ const sai_object_key_t   *key,
                                              _Inout_ sai_attribute_value_t *value,
                                              _In_ uint32_t                  attr_index,
                                              _Inout_ vendor_cache_t        *cache,
                                              void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Current breakout mode [sai_port_breakout_mode_type_t] */
sai_status_t stub_port_current_breakout_get(_In_ const sai_object_key_t   *key,
                                            _Inout_ sai_attribute_value_t *value,
                                            _In_ uint32_t                  attr_index,
                                            _Inout_ vendor_cache_t        *cache,
                                            void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->s32 = SAI_PORT_BREAKOUT_MODE_4_LANE;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Speed in Mbps [uint32_t] */
sai_status_t stub_port_speed_get(_In_ const sai_object_key_t   *key,
                                 _Inout_ sai_attribute_value_t *value,
                                 _In_ uint32_t                  attr_index,
                                 _Inout_ vendor_cache_t        *cache,
                                 void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->u32 = 40000;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Autoneg mode [bool] */
sai_status_t stub_port_autoneg_get(_In_ const sai_object_key_t   *key,
                                   _Inout_ sai_attribute_value_t *value,
                                   _In_ uint32_t                  attr_index,
                                   _Inout_ vendor_cache_t        *cache,
                                   void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->booldata = false;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default VLAN [sai_vlan_id_t]
 *   Untagged ingress frames are tagged with default VLAN
 */
sai_status_t stub_port_default_vlan_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->u16 = 1;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Default VLAN Priority [uint8_t]
 *  (default to 0) */
sai_status_t stub_port_default_vlan_prio_get(_In_ const sai_object_key_t   *key,
                                             _Inout_ sai_attribute_value_t *value,
                                             _In_ uint32_t                  attr_index,
                                             _Inout_ vendor_cache_t        *cache,
                                             void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->u8 = 0;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Ingress Filtering (Drop Frames with Unknown VLANs) [bool] */
sai_status_t stub_port_ingress_filter_get(_In_ const sai_object_key_t   *key,
                                          _Inout_ sai_attribute_value_t *value,
                                          _In_ uint32_t                  attr_index,
                                          _Inout_ vendor_cache_t        *cache,
                                          void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->booldata = false;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Dropping of untagged frames on ingress [bool] */
/* Dropping of tagged frames on ingress [bool] */
sai_status_t stub_port_drop_tags_get(_In_ const sai_object_key_t   *key,
                                     _Inout_ sai_attribute_value_t *value,
                                     _In_ uint32_t                  attr_index,
                                     _Inout_ vendor_cache_t        *cache,
                                     void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    assert((SAI_PORT_ATTR_DROP_UNTAGGED == (int64_t)arg) || (SAI_PORT_ATTR_DROP_TAGGED == (int64_t)arg));

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->booldata = false;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* Internal loopback control [sai_port_internal_loopback_mode_t] */
sai_status_t stub_port_internal_loopback_get(_In_ const sai_object_key_t   *key,
                                             _Inout_ sai_attribute_value_t *value,
                                             _In_ uint32_t                  attr_index,
                                             _Inout_ vendor_cache_t        *cache,
                                             void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->s32 = SAI_PORT_INTERNAL_LOOPBACK_NONE;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* FDB Learning mode [sai_port_fdb_learning_mode_t] */
sai_status_t stub_port_fdb_learning_get(_In_ const sai_object_key_t   *key,
                                        _Inout_ sai_attribute_value_t *value,
                                        _In_ uint32_t                  attr_index,
                                        _Inout_ vendor_cache_t        *cache,
                                        void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->s32 = SAI_PORT_LEARN_MODE_HW;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

/* MTU [uint32_t] */
sai_status_t stub_port_mtu_get(_In_ const sai_object_key_t   *key,
                               _Inout_ sai_attribute_value_t *value,
                               _In_ uint32_t                  attr_index,
                               _Inout_ vendor_cache_t        *cache,
                               void                          *arg)
{
    sai_status_t status;
    uint32_t     port_id;

    STUB_LOG_ENTER();

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(key->object_id, SAI_OBJECT_TYPE_PORT, &port_id))) {
        return status;
    }

    value->u32 = 1514;

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

static void port_key_to_str(_In_ sai_object_id_t port_id, _Out_ char *key_str)
{
    uint32_t port;

    if (SAI_STATUS_SUCCESS != stub_object_to_type(port_id, SAI_OBJECT_TYPE_PORT, &port)) {
        snprintf(key_str, MAX_KEY_STR_LEN, "invalid port");
    } else {
        snprintf(key_str, MAX_KEY_STR_LEN, "port %x", port);
    }
}

/*
 * Routine Description:
 *   Set port attribute value.
 *
 * Arguments:
 *    [in] port_id - port id
 *    [in] attr - attribute
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_set_port_attribute(_In_ sai_object_id_t port_id, _In_ const sai_attribute_t *attr)
{
    const sai_object_key_t key = { .object_id = port_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    port_key_to_str(port_id, key_str);
    return sai_set_attribute(&key, key_str, port_attribs, port_vendor_attribs, attr);
}


/*
 * Routine Description:
 *   Get port attribute value.
 *
 * Arguments:
 *    [in] port_id - port id
 *    [in] attr_count - number of attributes
 *    [inout] attr_list - array of attributes
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_get_port_attribute(_In_ sai_object_id_t     port_id,
                                     _In_ uint32_t            attr_count,
                                     _Inout_ sai_attribute_t *attr_list)
{
    const sai_object_key_t key = { .object_id = port_id };
    char                   key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    port_key_to_str(port_id, key_str);
    return sai_get_attributes(&key, key_str, port_attribs, port_vendor_attribs, attr_count, attr_list);
}

/*
 * Routine Description:
 *   Get port statistics counters.
 *
 * Arguments:
 *    [in] port_id - port id
 *    [in] counter_ids - specifies the array of counter ids
 *    [in] number_of_counters - number of counters in the array
 *    [out] counters - array of resulting counter values.
 *
 * Return Values:
 *    SAI_STATUS_SUCCESS on success
 *    Failure status code on error
 */
sai_status_t stub_get_port_stats(_In_ sai_object_id_t                port_id,
                                 _In_ const sai_port_stat_counter_t *counter_ids,
                                 _In_ uint32_t                       number_of_counters,
                                 _Out_ uint64_t                     *counters)
{
    sai_status_t status;
    uint32_t     ii, port_data;
    char         key_str[MAX_KEY_STR_LEN];

    STUB_LOG_ENTER();

    port_key_to_str(port_id, key_str);
    STUB_LOG_NTC("Get port stats %s\n", key_str);

    if (NULL == counter_ids) {
        STUB_LOG_ERR("NULL counter ids array param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (NULL == counters) {
        STUB_LOG_ERR("NULL counters array param\n");
        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (SAI_STATUS_SUCCESS != (status = stub_object_to_type(port_id, SAI_OBJECT_TYPE_PORT, &port_data))) {
        return status;
    }

    for (ii = 0; ii < number_of_counters; ii++) {
        switch (counter_ids[ii]) {
        case SAI_PORT_STAT_IF_IN_OCTETS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_IN_UCAST_PKTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_IN_NON_UCAST_PKTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_IN_DISCARDS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_IN_ERRORS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_IN_UNKNOWN_PROTOS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_IN_BROADCAST_PKTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_IN_MULTICAST_PKTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_OUT_OCTETS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_OUT_UCAST_PKTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_OUT_NON_UCAST_PKTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_OUT_DISCARDS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_OUT_ERRORS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_OUT_BROADCAST_PKTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_OUT_MULTICAST_PKTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_DROP_EVENTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_MULTICAST_PKTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_BROADCAST_PKTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_UNDERSIZE_PKTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_FRAGMENTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS_64_OCTETS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS_65_TO_127_OCTETS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS_128_TO_255_OCTETS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS_256_TO_511_OCTETS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS_512_TO_1023_OCTETS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS_1024_TO_1518_OCTETS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_OVERSIZE_PKTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_JABBERS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_OCTETS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_PKTS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_COLLISIONS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_ETHER_STATS_CRC_ALIGN_ERRORS:
            counters[ii] = 0;
            break;

        case SAI_PORT_STAT_IF_IN_VLAN_DISCARDS:
        case SAI_PORT_STAT_IF_OUT_QLEN:
        case SAI_PORT_STAT_ETHER_RX_OVERSIZE_PKTS:
        case SAI_PORT_STAT_ETHER_TX_OVERSIZE_PKTS:
        case SAI_PORT_STAT_ETHER_STATS_TX_NO_ERRORS:
        case SAI_PORT_STAT_ETHER_STATS_RX_NO_ERRORS:
        case SAI_PORT_STAT_IP_IN_RECEIVES:
        case SAI_PORT_STAT_IP_IN_OCTETS:
        case SAI_PORT_STAT_IP_IN_UCAST_PKTS:
        case SAI_PORT_STAT_IP_IN_NON_UCAST_PKTS:
        case SAI_PORT_STAT_IP_IN_DISCARDS:
        case SAI_PORT_STAT_IP_OUT_OCTETS:
        case SAI_PORT_STAT_IP_OUT_UCAST_PKTS:
        case SAI_PORT_STAT_IP_OUT_NON_UCAST_PKTS:
        case SAI_PORT_STAT_IP_OUT_DISCARDS:
        case SAI_PORT_STAT_IPV6_IN_RECEIVES:
        case SAI_PORT_STAT_IPV6_IN_OCTETS:
        case SAI_PORT_STAT_IPV6_IN_UCAST_PKTS:
        case SAI_PORT_STAT_IPV6_IN_NON_UCAST_PKTS:
        case SAI_PORT_STAT_IPV6_IN_MCAST_PKTS:
        case SAI_PORT_STAT_IPV6_IN_DISCARDS:
        case SAI_PORT_STAT_IPV6_OUT_OCTETS:
        case SAI_PORT_STAT_IPV6_OUT_UCAST_PKTS:
        case SAI_PORT_STAT_IPV6_OUT_NON_UCAST_PKTS:
        case SAI_PORT_STAT_IPV6_OUT_MCAST_PKTS:
        case SAI_PORT_STAT_IPV6_OUT_DISCARDS:
            counters[ii] = 0;
            break;

        default:
            STUB_LOG_ERR("Invalid port counter %d\n", counter_ids[ii]);
            return SAI_STATUS_INVALID_PARAMETER;
        }
    }

    STUB_LOG_EXIT();
    return SAI_STATUS_SUCCESS;
}

const sai_port_api_t port_api = {
    stub_set_port_attribute,
    stub_get_port_attribute,
    stub_get_port_stats,
    NULL,
    NULL
};
