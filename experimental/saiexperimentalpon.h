/**
 * Copyright (c) 2026 Microsoft Open Technologies, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License"); you may
 *    not use this file except in compliance with the License. You may obtain
 *    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 *    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *
 *    See the Apache Version 2.0 License for specific language governing
 *    permissions and limitations under the License.
 *
 *    Microsoft would like to thank the following companies for their review and
 *    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
 *    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
 *
 * @file    saiexperimentalpon.h
 *
 * @brief   This module defines the aggregated experimental PON API.
 */

#if !defined (__SAIEXPERIMENTALPON_H_)
#define __SAIEXPERIMENTALPON_H_

#include <saiexperimentalponcommon.h>

#include <saiexperimentalponcontroller.h>
#include <saiexperimentalponoltintf.h>
#include <saiexperimentalponoltplug.h>
#include <saiexperimentalpononu.h>
#include <saiexperimentalpononutemplate.h>
#include <saiexperimentalponserviceconfigprofile.h>

/**
 * @defgroup SAIEXPERIMENTALPON SAI - Experimental: PON specific API definitions
 *
 * @{
 */

/**
 * @brief PON action types for one-shot RPC operations
 */
typedef enum _sai_pon_action_type_t
{
    SAI_PON_ACTION_TYPE_OLT_RESET                   = 0,
    SAI_PON_ACTION_TYPE_OLT_CLEAR                   = 1,
    SAI_PON_ACTION_TYPE_OLT_DISABLE_ONU             = 2,
    SAI_PON_ACTION_TYPE_OLT_ALLOW_ONU_REGISTRATION  = 3,
    SAI_PON_ACTION_TYPE_OLT_PROTECTION_ARM          = 4,
    SAI_PON_ACTION_TYPE_OLT_PROTECTION_SWITCH       = 5,
    SAI_PON_ACTION_TYPE_ONU_RESET                   = 6,
    SAI_PON_ACTION_TYPE_ONU_CLEAR                   = 7,
    SAI_PON_ACTION_TYPE_ONU_CLEAR_ABORTED_UPGRADE   = 8,
    SAI_PON_ACTION_TYPE_CONTROLLER_CLEAR            = 9,
    SAI_PON_ACTION_TYPE_CONTROLLER_SET_STATUS       = 10,
    SAI_PON_ACTION_TYPE_DB_MANAGER_RESTART          = 11,
    SAI_PON_ACTION_TYPE_DB_MANAGER_SERVER_STATS_CLR = 12,
    SAI_PON_ACTION_TYPE_DB_MANAGER_CLIENT_STATS_CLR = 13,
} sai_pon_action_type_t;

/**
 * @brief Attribute IDs for PON action objects
 */
typedef enum _sai_pon_action_attr_t
{
    /**
     * @brief Start of attributes
     */
    SAI_PON_ACTION_ATTR_START,

    /**
     * @brief Target name (OLT or ONU interface name)
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ACTION_ATTR_NAME = SAI_PON_ACTION_ATTR_START,

    /**
     * @brief OLT name for ONU-targeted actions
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ACTION_ATTR_OLT_NAME,

    /**
     * @brief ONU ID for ONU-specific actions
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ACTION_ATTR_ONU_ID,

    /**
     * @brief Device state for clear actions
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ACTION_ATTR_DEVICE_STATE,

    /**
     * @brief Disable flag for OLT-disable-ONU action
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ACTION_ATTR_DISABLE,

    /**
     * @brief Arm flag for OLT-protection-arm action
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ACTION_ATTR_ARM,

    /**
     * @brief Force switch flag for OLT-protection-arm action
     *
     * @type bool
     * @flags CREATE_AND_SET
     * @default false
     */
    SAI_PON_ACTION_ATTR_FORCE_SWITCH,

    /**
     * @brief Admin status for controller-set-status action
     *
     * @type sai_s8_list_t
     * @flags CREATE_AND_SET
     */
    SAI_PON_ACTION_ATTR_ADMIN_STATUS,

    /**
     * @brief End of attributes
     */
    SAI_PON_ACTION_ATTR_END,

    /** Custom range base value */
    SAI_PON_ACTION_ATTR_CUSTOM_RANGE_START = 0x10000000,

    /** End of custom range base */
    SAI_PON_ACTION_ATTR_CUSTOM_RANGE_END

} sai_pon_action_attr_t;

/**
 * @brief Execute a PON action (one-shot RPC)
 *
 * @param[in] switch_id Switch ID
 * @param[in] action_type Type of PON action to execute
 * @param[in] attr_count Number of attributes
 * @param[in] attr_list Array of attributes
 *
 * @return #SAI_STATUS_SUCCESS on success, failure status code on error
 */
typedef sai_status_t (*sai_execute_pon_action_fn)(
        _In_ sai_object_id_t switch_id,
        _In_ sai_pon_action_type_t action_type,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list);

typedef struct _sai_pon_api_t
{
    sai_create_pon_controller_fn                                                                                                      create_pon_controller;
    sai_remove_pon_controller_fn                                                                                                      remove_pon_controller;
    sai_set_pon_controller_attribute_fn                                                                                               set_pon_controller_attribute;
    sai_get_pon_controller_attribute_fn                                                                                               get_pon_controller_attribute;
    sai_create_pon_downstream_qos_map_map_fn                                                                                          create_pon_downstream_qos_map_map;
    sai_remove_pon_downstream_qos_map_map_fn                                                                                          remove_pon_downstream_qos_map_map;
    sai_set_pon_downstream_qos_map_map_attribute_fn                                                                                   set_pon_downstream_qos_map_map_attribute;
    sai_get_pon_downstream_qos_map_map_attribute_fn                                                                                   get_pon_downstream_qos_map_map_attribute;
    sai_get_pon_controller_system_status_olt_attribute_fn                                                                             get_pon_controller_system_status_olt_attribute;
    sai_get_pon_controller_system_status_olt_onu_attribute_fn                                                                         get_pon_controller_system_status_olt_onu_attribute;
    sai_get_pon_controller_state_attribute_fn                                                                                         get_pon_controller_state_attribute;
    sai_get_pon_controller_onu_fw_upgrade_state_attribute_fn                                                                          get_pon_controller_onu_fw_upgrade_state_attribute;

    /* bulk get pon controller */
    sai_bulk_get_pon_controller_system_status_olt_attribute_fn                                                                        get_pon_controller_system_status_olts_attribute;
    sai_bulk_get_pon_controller_system_status_olt_onu_attribute_fn                                                                    get_pon_controller_system_status_olt_onus_attribute;
    sai_bulk_get_pon_controller_state_attribute_fn                                                                                    get_pon_controller_states_attribute;
    sai_bulk_get_pon_controller_onu_fw_upgrade_state_attribute_fn                                                                     get_pon_controller_onu_fw_upgrade_states_attribute;

    sai_create_pon_olt_intf_fn                                                                                                        create_pon_olt_intf;
    sai_remove_pon_olt_intf_fn                                                                                                        remove_pon_olt_intf;
    sai_set_pon_olt_intf_attribute_fn                                                                                                 set_pon_olt_intf_attribute;
    sai_get_pon_olt_intf_attribute_fn                                                                                                 get_pon_olt_intf_attribute;
    sai_create_pon_olt_intf_network_fn                                                                                                create_pon_olt_intf_network;
    sai_remove_pon_olt_intf_network_fn                                                                                                remove_pon_olt_intf_network;
    sai_set_pon_olt_intf_network_attribute_fn                                                                                         set_pon_olt_intf_network_attribute;
    sai_get_pon_olt_intf_network_attribute_fn                                                                                         get_pon_olt_intf_network_attribute;
    sai_create_pon_olt_intf_onu_fn                                                                                                    create_pon_olt_intf_onu;
    sai_remove_pon_olt_intf_onu_fn                                                                                                    remove_pon_olt_intf_onu;
    sai_set_pon_olt_intf_onu_attribute_fn                                                                                             set_pon_olt_intf_onu_attribute;
    sai_get_pon_olt_intf_onu_attribute_fn                                                                                             get_pon_olt_intf_onu_attribute;
    sai_create_pon_olt_intf_onu_olt_service_fn                                                                                        create_pon_olt_intf_onu_olt_service;
    sai_remove_pon_olt_intf_onu_olt_service_fn                                                                                        remove_pon_olt_intf_onu_olt_service;
    sai_set_pon_olt_intf_onu_olt_service_attribute_fn                                                                                 set_pon_olt_intf_onu_olt_service_attribute;
    sai_get_pon_olt_intf_onu_olt_service_attribute_fn                                                                                 get_pon_olt_intf_onu_olt_service_attribute;
    sai_create_pon_olt_intf_onu_service_tcont_fn                                                                                      create_pon_olt_intf_onu_service_tcont;
    sai_remove_pon_olt_intf_onu_service_tcont_fn                                                                                      remove_pon_olt_intf_onu_service_tcont;
    sai_set_pon_olt_intf_onu_service_tcont_attribute_fn                                                                               set_pon_olt_intf_onu_service_tcont_attribute;
    sai_get_pon_olt_intf_onu_service_tcont_attribute_fn                                                                               get_pon_olt_intf_onu_service_tcont_attribute;
    sai_create_pon_olt_intf_onu_service_gemport_fn                                                                                    create_pon_olt_intf_onu_service_gemport;
    sai_remove_pon_olt_intf_onu_service_gemport_fn                                                                                    remove_pon_olt_intf_onu_service_gemport;
    sai_set_pon_olt_intf_onu_service_gemport_attribute_fn                                                                             set_pon_olt_intf_onu_service_gemport_attribute;
    sai_get_pon_olt_intf_onu_service_gemport_attribute_fn                                                                             get_pon_olt_intf_onu_service_gemport_attribute;
    sai_create_pon_olt_protection_group_fn                                                                                            create_pon_olt_protection_group;
    sai_remove_pon_olt_protection_group_fn                                                                                            remove_pon_olt_protection_group;
    sai_set_pon_olt_protection_group_attribute_fn                                                                                     set_pon_olt_protection_group_attribute;
    sai_get_pon_olt_protection_group_attribute_fn                                                                                     get_pon_olt_protection_group_attribute;
    sai_get_pon_olt_intf_onu_operational_state_attribute_fn                                                                           get_pon_olt_intf_onu_operational_state_attribute;
    sai_get_pon_olt_intf_nni_network_learning_table_state_attribute_fn                                                                get_pon_olt_intf_nni_network_learning_table_state_attribute;
    sai_get_pon_firmware_filename_state_attribute_fn                                                                                  get_pon_firmware_filename_state_attribute;
    sai_get_pon_olt_intf_state_attribute_fn                                                                                           get_pon_olt_intf_state_attribute;
    sai_get_pon_olt_intf_network_state_attribute_fn                                                                                   get_pon_olt_intf_network_state_attribute;
    sai_get_pon_olt_intf_onu_state_attribute_fn                                                                                       get_pon_olt_intf_onu_state_attribute;
    sai_get_pon_olt_intf_onu_fw_upgrade_state_attribute_fn                                                                            get_pon_olt_intf_onu_fw_upgrade_state_attribute;
    sai_get_pon_olt_intf_onu_service_tcont_state_attribute_fn                                                                         get_pon_olt_intf_onu_service_tcont_state_attribute;
    sai_get_pon_olt_intf_onu_service_gemport_state_attribute_fn                                                                       get_pon_olt_intf_onu_service_gemport_state_attribute;
    sai_get_pon_olt_protection_group_state_attribute_fn                                                                               get_pon_olt_protection_group_state_attribute;
    sai_get_pon_olt_intf_statistics_binned_attribute_fn                                                                               get_pon_olt_intf_statistics_binned_attribute;
    sai_get_pon_olt_statistics_binned_pon_flooding_attribute_fn                                                                       get_pon_olt_statistics_binned_pon_flooding_attribute;
    sai_get_pon_olt_statistics_binned_pon_flooding_network_attribute_fn                                                               get_pon_olt_statistics_binned_pon_flooding_network_attribute;
    sai_get_pon_olt_statistics_accumulating_attribute_fn                                                                              get_pon_olt_statistics_accumulating_attribute;
    sai_get_pon_olt_statistics_accumulating_env_attribute_fn                                                                          get_pon_olt_statistics_accumulating_env_attribute;
    sai_get_pon_olt_statistics_accumulating_temp_attribute_fn                                                                         get_pon_olt_statistics_accumulating_temp_attribute;
    sai_get_pon_olt_statistics_accumulating_pon_flooding_attribute_fn                                                                 get_pon_olt_statistics_accumulating_pon_flooding_attribute;
    sai_get_pon_olt_statistics_accumulating_pon_flooding_nni_network_attribute_fn                                                     get_pon_olt_statistics_accumulating_pon_flooding_nni_network_attribute;

    /* bulk get pon olt */
    sai_bulk_get_pon_olt_intf_onu_operational_state_attribute_fn                                                                      get_pon_olt_intf_onu_operational_states_attribute;
    sai_bulk_get_pon_olt_intf_nni_network_learning_table_state_attribute_fn                                                           get_pon_olt_intf_nni_network_learning_table_states_attribute;
    sai_bulk_get_pon_firmware_filename_state_attribute_fn                                                                             get_pon_firmware_filename_states_attribute;
    sai_bulk_get_pon_olt_intf_state_attribute_fn                                                                                      get_pon_olt_intf_states_attribute;
    sai_bulk_get_pon_olt_intf_network_state_attribute_fn                                                                              get_pon_olt_intf_network_states_attribute;
    sai_bulk_get_pon_olt_intf_onu_state_attribute_fn                                                                                  get_pon_olt_intf_onu_states_attribute;
    sai_bulk_get_pon_olt_intf_onu_fw_upgrade_state_attribute_fn                                                                       get_pon_olt_intf_onu_fw_upgrade_states_attribute;
    sai_bulk_get_pon_olt_intf_onu_service_tcont_state_attribute_fn                                                                    get_pon_olt_intf_onu_service_tcont_states_attribute;
    sai_bulk_get_pon_olt_intf_onu_service_gemport_state_attribute_fn                                                                  get_pon_olt_intf_onu_service_gemport_states_attribute;
    sai_bulk_get_pon_olt_intf_statistics_binned_attribute_fn                                                                          get_pon_olt_intf_statistics_binneds_attribute;
    sai_bulk_get_pon_olt_statistics_binned_pon_flooding_attribute_fn                                                                  get_pon_olt_statistics_binned_pon_floodings_attribute;
    sai_bulk_get_pon_olt_statistics_binned_pon_flooding_network_attribute_fn                                                          get_pon_olt_statistics_binned_pon_flooding_networks_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_attribute_fn                                                                         get_pon_olt_statistics_accumulatings_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_env_attribute_fn                                                                     get_pon_olt_statistics_accumulating_envs_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_temp_attribute_fn                                                                    get_pon_olt_statistics_accumulating_temps_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_pon_flooding_attribute_fn                                                            get_pon_olt_statistics_accumulating_pon_floodings_attribute;
    sai_bulk_get_pon_olt_statistics_accumulating_pon_flooding_nni_network_attribute_fn                                                get_pon_olt_statistics_accumulating_pon_flooding_nni_networks_attribute;

    sai_create_pon_olt_plug_fw_bank_file_fn                                                                                           create_pon_olt_plug_fw_bank_file;
    sai_remove_pon_olt_plug_fw_bank_file_fn                                                                                           remove_pon_olt_plug_fw_bank_file;
    sai_set_pon_olt_plug_fw_bank_file_attribute_fn                                                                                    set_pon_olt_plug_fw_bank_file_attribute;
    sai_get_pon_olt_plug_fw_bank_file_attribute_fn                                                                                    get_pon_olt_plug_fw_bank_file_attribute;
    sai_create_pon_olt_plug_fn                                                                                                        create_pon_olt_plug;
    sai_remove_pon_olt_plug_fn                                                                                                        remove_pon_olt_plug;
    sai_set_pon_olt_plug_attribute_fn                                                                                                 set_pon_olt_plug_attribute;
    sai_get_pon_olt_plug_attribute_fn                                                                                                 get_pon_olt_plug_attribute;
    sai_get_pon_olt_plug_fw_upgrade_status_attribute_fn                                                                               get_pon_olt_plug_fw_upgrade_status_attribute;
    sai_get_pon_olt_plug_state_attribute_fn                                                                                           get_pon_olt_plug_state_attribute;
    sai_get_pon_olt_plug_fw_bank_version_state_attribute_fn                                                                           get_pon_olt_plug_fw_bank_version_state_attribute;
    sai_get_pon_olt_plug_statistics_binned_env_attribute_fn                                                                           get_pon_olt_plug_statistics_binned_env_attribute;
    sai_get_pon_olt_plug_statistics_binned_nni_attribute_fn                                                                           get_pon_olt_plug_statistics_binned_nni_attribute;
    sai_get_pon_olt_plug_statistics_binned_temp_attribute_fn                                                                          get_pon_olt_plug_statistics_binned_temp_attribute;

    /* bulk get olt plug */
    sai_bulk_get_pon_olt_plug_fw_upgrade_status_attribute_fn                                                                          get_pon_olt_plug_fw_upgrade_statuss_attribute;
    sai_bulk_get_pon_olt_plug_state_attribute_fn                                                                                      get_pon_olt_plug_states_attribute;
    sai_bulk_get_pon_olt_plug_fw_bank_version_state_attribute_fn                                                                      get_pon_olt_plug_fw_bank_version_states_attribute;
    sai_bulk_get_pon_olt_plug_statistics_binned_env_attribute_fn                                                                      get_pon_olt_plug_statistics_binned_envs_attribute;
    sai_bulk_get_pon_olt_plug_statistics_binned_nni_attribute_fn                                                                      get_pon_olt_plug_statistics_binned_nnis_attribute;
    sai_bulk_get_pon_olt_plug_statistics_binned_temp_attribute_fn                                                                     get_pon_olt_plug_statistics_binned_temps_attribute;

    sai_create_pon_onu_fn                                                                                                             create_pon_onu;
    sai_remove_pon_onu_fn                                                                                                             remove_pon_onu;
    sai_set_pon_onu_attribute_fn                                                                                                      set_pon_onu_attribute;
    sai_get_pon_onu_attribute_fn                                                                                                      get_pon_onu_attribute;
    sai_create_pon_onu_olt_service_fn                                                                                                 create_pon_onu_olt_service;
    sai_remove_pon_onu_olt_service_fn                                                                                                 remove_pon_onu_olt_service;
    sai_set_pon_onu_olt_service_attribute_fn                                                                                          set_pon_onu_olt_service_attribute;
    sai_get_pon_onu_olt_service_attribute_fn                                                                                          get_pon_onu_olt_service_attribute;
    sai_create_pon_onu_olt_service_network_fn                                                                                         create_pon_onu_olt_service_network;
    sai_remove_pon_onu_olt_service_network_fn                                                                                         remove_pon_onu_olt_service_network;
    sai_set_pon_onu_olt_service_network_attribute_fn                                                                                  set_pon_onu_olt_service_network_attribute;
    sai_get_pon_onu_olt_service_network_attribute_fn                                                                                  get_pon_onu_olt_service_network_attribute;
    sai_create_pon_onu_service_config_value_fn                                                                                        create_pon_onu_service_config_value;
    sai_remove_pon_onu_service_config_value_fn                                                                                        remove_pon_onu_service_config_value;
    sai_set_pon_onu_service_config_value_attribute_fn                                                                                 set_pon_onu_service_config_value_attribute;
    sai_get_pon_onu_service_config_value_attribute_fn                                                                                 get_pon_onu_service_config_value_attribute;
    sai_create_pon_onu_uni_fn                                                                                                         create_pon_onu_uni;
    sai_remove_pon_onu_uni_fn                                                                                                         remove_pon_onu_uni;
    sai_set_pon_onu_uni_attribute_fn                                                                                                  set_pon_onu_uni_attribute;
    sai_get_pon_onu_uni_attribute_fn                                                                                                  get_pon_onu_uni_attribute;
    sai_create_pon_onu_fw_bank_file_fn                                                                                                create_pon_onu_fw_bank_file;
    sai_remove_pon_onu_fw_bank_file_fn                                                                                                remove_pon_onu_fw_bank_file;
    sai_set_pon_onu_fw_bank_file_attribute_fn                                                                                         set_pon_onu_fw_bank_file_attribute;
    sai_get_pon_onu_fw_bank_file_attribute_fn                                                                                         get_pon_onu_fw_bank_file_attribute;
    sai_get_pon_onu_olt_service_downstream_qos_map_state_attribute_fn                                                                 get_pon_onu_olt_service_downstream_qos_map_state_attribute;
    sai_get_pon_onu_olt_service_downstream_qos_map_map_state_attribute_fn                                                             get_pon_onu_olt_service_downstream_qos_map_map_state_attribute;
    sai_get_pon_onu_uni_learned_addresses_state_attribute_fn                                                                          get_pon_onu_uni_learned_addresses_state_attribute;
    sai_get_pon_onu_state_attribute_fn                                                                                                get_pon_onu_state_attribute;
    sai_get_pon_onu_fw_bank_version_state_attribute_fn                                                                                get_pon_onu_fw_bank_version_state_attribute;
    sai_get_pon_onu_olt_service_state_attribute_fn                                                                                    get_pon_onu_olt_service_state_attribute;
    sai_get_pon_onu_olt_service_network_state_attribute_fn                                                                            get_pon_onu_olt_service_network_state_attribute;
    sai_get_pon_onu_uni_state_attribute_fn                                                                                            get_pon_onu_uni_state_attribute;
    sai_get_pon_onu_statistics_binned_attribute_fn                                                                                    get_pon_onu_statistics_binned_attribute;
    sai_get_pon_onu_statistics_binned_olt_pon_attribute_fn                                                                            get_pon_onu_statistics_binned_olt_pon_attribute;
    sai_get_pon_onu_statistics_binned_olt_pon_omcc_attribute_fn                                                                       get_pon_onu_statistics_binned_olt_pon_omcc_attribute;
    sai_get_pon_onu_statistics_binned_olt_pon_service_attribute_fn                                                                    get_pon_onu_statistics_binned_olt_pon_service_attribute;
    sai_get_pon_onu_statistics_binned_onu_enhanced_tc_pm_attribute_fn                                                                 get_pon_onu_statistics_binned_onu_enhanced_tc_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_attribute_fn                                                     get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_attribute_fn                                               get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_attribute_fn                                                   get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_attribute_fn                                                     get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_pm_attribute_fn                                                                    get_pon_onu_statistics_binned_onu_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_ethernet_pm3_attribute_fn                                                                   get_pon_onu_statistics_binned_onu_ethernet_pm3_attribute;
    sai_get_pon_onu_statistics_binned_onu_fec_pm_attribute_fn                                                                         get_pon_onu_statistics_binned_onu_fec_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_gal_ethernet_pm_attribute_fn                                                                get_pon_onu_statistics_binned_onu_gal_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_attribute_fn                                                        get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_attribute_fn                                                     get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_binned_onu_mac_bridge_port_pm_attribute_fn                                                             get_pon_onu_statistics_binned_onu_mac_bridge_port_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_operational_pm_attribute_fn                                                                 get_pon_onu_statistics_binned_onu_operational_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_pon_attribute_fn                                                                            get_pon_onu_statistics_binned_onu_pon_attribute;
    sai_get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_attribute_fn                                                 get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_attribute_fn                                                     get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_attribute_fn                                                      get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_attribute_fn                                                        get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_olt_pon_attribute_fn                                                                      get_pon_onu_statistics_accumulating_olt_pon_attribute;
    sai_get_pon_onu_statistics_accumulating_olt_pon_omcc_attribute_fn                                                                 get_pon_onu_statistics_accumulating_olt_pon_omcc_attribute;
    sai_get_pon_onu_statistics_accumulating_olt_pon_service_attribute_fn                                                              get_pon_onu_statistics_accumulating_olt_pon_service_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_enhanced_tc_pm_attribute_fn                                                           get_pon_onu_statistics_accumulating_onu_enhanced_tc_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_attribute_fn                                               get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_attribute_fn                                         get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_attribute_fn                                             get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_attribute_fn                                               get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_pm_attribute_fn                                                              get_pon_onu_statistics_accumulating_onu_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ethernet_pm3_attribute_fn                                                             get_pon_onu_statistics_accumulating_onu_ethernet_pm3_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_fec_pm_attribute_fn                                                                   get_pon_onu_statistics_accumulating_onu_fec_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_gal_ethernet_pm_attribute_fn                                                          get_pon_onu_statistics_accumulating_onu_gal_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_attribute_fn                                                  get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_attribute_fn                                               get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_attribute_fn                                                       get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_operational_pm_attribute_fn                                                           get_pon_onu_statistics_accumulating_onu_operational_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_pon_attribute_fn                                                                      get_pon_onu_statistics_accumulating_onu_pon_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_attribute_fn                                           get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_attribute_fn                                               get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_attribute_fn                                                get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_attribute_fn                                                  get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_enhanced_tc_pm_attribute_fn                                                              get_pon_onu_statistics_streaming_onu_enhanced_tc_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_attribute_fn                                                get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_attribute;
    sai_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_attribute_fn                                                  get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_attribute;
    sai_get_pon_onu_statistics_streaming_onu_ethernet_pm_attribute_fn                                                                 get_pon_onu_statistics_streaming_onu_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_fec_pm_attribute_fn                                                                      get_pon_onu_statistics_streaming_onu_fec_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_gal_ethernet_pm_attribute_fn                                                             get_pon_onu_statistics_streaming_onu_gal_ethernet_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_attribute_fn                                                     get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_attribute_fn                                              get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_attribute;
    sai_get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_attribute_fn                                                   get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_attribute;
    sai_get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_attribute_fn                                                     get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_attribute;

    /* bulk get pon onu */
    sai_bulk_get_pon_onu_olt_service_downstream_qos_map_state_attribute_fn                                                            get_pon_onu_olt_service_downstream_qos_map_states_attribute;
    sai_bulk_get_pon_onu_olt_service_downstream_qos_map_map_state_attribute_fn                                                        get_pon_onu_olt_service_downstream_qos_map_map_states_attribute;
    sai_bulk_get_pon_onu_uni_learned_addresses_state_attribute_fn                                                                     get_pon_onu_uni_learned_addresses_states_attribute;
    sai_bulk_get_pon_onu_state_attribute_fn                                                                                           get_pon_onu_states_attribute;
    sai_bulk_get_pon_onu_fw_bank_version_state_attribute_fn                                                                           get_pon_onu_fw_bank_version_states_attribute;
    sai_bulk_get_pon_onu_olt_service_state_attribute_fn                                                                               get_pon_onu_olt_service_states_attribute;
    sai_bulk_get_pon_onu_olt_service_network_state_attribute_fn                                                                       get_pon_onu_olt_service_network_states_attribute;
    sai_bulk_get_pon_onu_uni_state_attribute_fn                                                                                       get_pon_onu_uni_states_attribute;
    sai_bulk_get_pon_onu_statistics_binned_attribute_fn                                                                               get_pon_onu_statistics_binneds_attribute;
    sai_bulk_get_pon_onu_statistics_binned_olt_pon_attribute_fn                                                                       get_pon_onu_statistics_binned_olt_pons_attribute;
    sai_bulk_get_pon_onu_statistics_binned_olt_pon_omcc_attribute_fn                                                                  get_pon_onu_statistics_binned_olt_pon_omccs_attribute;
    sai_bulk_get_pon_onu_statistics_binned_olt_pon_service_attribute_fn                                                               get_pon_onu_statistics_binned_olt_pon_services_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_enhanced_tc_pm_attribute_fn                                                            get_pon_onu_statistics_binned_onu_enhanced_tc_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_attribute_fn                                                get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bit_attribute_fn                                          get_pon_onu_statistics_binned_onu_ethernet_frame_extended_pm_64bits_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstream_attribute_fn                                              get_pon_onu_statistics_binned_onu_ethernet_frame_pm_downstreams_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstream_attribute_fn                                                get_pon_onu_statistics_binned_onu_ethernet_frame_pm_upstreams_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_pm_attribute_fn                                                               get_pon_onu_statistics_binned_onu_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ethernet_pm3_attribute_fn                                                              get_pon_onu_statistics_binned_onu_ethernet_pm3s_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_fec_pm_attribute_fn                                                                    get_pon_onu_statistics_binned_onu_fec_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_gal_ethernet_pm_attribute_fn                                                           get_pon_onu_statistics_binned_onu_gal_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pm_attribute_fn                                                   get_pon_onu_statistics_binned_onu_gem_port_network_ctp_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_data_attribute_fn                                                get_pon_onu_statistics_binned_onu_ip_host_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_mac_bridge_port_pm_attribute_fn                                                        get_pon_onu_statistics_binned_onu_mac_bridge_port_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_operational_pm_attribute_fn                                                            get_pon_onu_statistics_binned_onu_operational_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_pon_attribute_fn                                                                       get_pon_onu_statistics_binned_onu_pons_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_data_attribute_fn                                            get_pon_onu_statistics_binned_onu_rs232_rs485_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_data_attribute_fn                                                get_pon_onu_statistics_binned_onu_tcp_udp_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pm_attribute_fn                                                 get_pon_onu_statistics_binned_onu_xg_pon_downstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pm_attribute_fn                                                   get_pon_onu_statistics_binned_onu_xg_pon_upstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_olt_pon_attribute_fn                                                                 get_pon_onu_statistics_accumulating_olt_pons_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_olt_pon_omcc_attribute_fn                                                            get_pon_onu_statistics_accumulating_olt_pon_omccs_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_olt_pon_service_attribute_fn                                                         get_pon_onu_statistics_accumulating_olt_pon_services_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_enhanced_tc_pm_attribute_fn                                                      get_pon_onu_statistics_accumulating_onu_enhanced_tc_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_attribute_fn                                          get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bit_attribute_fn                                    get_pon_onu_statistics_accumulating_onu_ethernet_frame_extended_pm_64bits_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstream_attribute_fn                                        get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_downstreams_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstream_attribute_fn                                          get_pon_onu_statistics_accumulating_onu_ethernet_frame_pm_upstreams_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_pm_attribute_fn                                                         get_pon_onu_statistics_accumulating_onu_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ethernet_pm3_attribute_fn                                                        get_pon_onu_statistics_accumulating_onu_ethernet_pm3s_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_fec_pm_attribute_fn                                                              get_pon_onu_statistics_accumulating_onu_fec_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_gal_ethernet_pm_attribute_fn                                                     get_pon_onu_statistics_accumulating_onu_gal_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pm_attribute_fn                                             get_pon_onu_statistics_accumulating_onu_gem_port_network_ctp_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_data_attribute_fn                                          get_pon_onu_statistics_accumulating_onu_ip_host_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pm_attribute_fn                                                  get_pon_onu_statistics_accumulating_onu_mac_bridge_port_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_operational_pm_attribute_fn                                                      get_pon_onu_statistics_accumulating_onu_operational_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_pon_attribute_fn                                                                 get_pon_onu_statistics_accumulating_onu_pons_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_data_attribute_fn                                      get_pon_onu_statistics_accumulating_onu_rs232_rs485_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_data_attribute_fn                                          get_pon_onu_statistics_accumulating_onu_tcp_udp_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pm_attribute_fn                                           get_pon_onu_statistics_accumulating_onu_xg_pon_downstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pm_attribute_fn                                             get_pon_onu_statistics_accumulating_onu_xg_pon_upstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_enhanced_tc_pm_attribute_fn                                                         get_pon_onu_statistics_streaming_onu_enhanced_tc_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstream_attribute_fn                                           get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_downstreams_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstream_attribute_fn                                             get_pon_onu_statistics_streaming_onu_ethernet_frame_pm_upstreams_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_ethernet_pm_attribute_fn                                                            get_pon_onu_statistics_streaming_onu_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_fec_pm_attribute_fn                                                                 get_pon_onu_statistics_streaming_onu_fec_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_gal_ethernet_pm_attribute_fn                                                        get_pon_onu_statistics_streaming_onu_gal_ethernet_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pm_attribute_fn                                                get_pon_onu_statistics_streaming_onu_gem_port_network_ctp_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_data_attribute_fn                                         get_pon_onu_statistics_streaming_onu_rs232_rs485_perf_mon_hist_datas_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pm_attribute_fn                                              get_pon_onu_statistics_streaming_onu_xg_pon_downstream_mgmt_pms_attribute;
    sai_bulk_get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pm_attribute_fn                                                get_pon_onu_statistics_streaming_onu_xg_pon_upstream_mgmt_pms_attribute;

    sai_create_pon_onu_template_onu_fn                                                                                                create_pon_onu_template_onu;
    sai_remove_pon_onu_template_onu_fn                                                                                                remove_pon_onu_template_onu;
    sai_set_pon_onu_template_onu_attribute_fn                                                                                         set_pon_onu_template_onu_attribute;
    sai_get_pon_onu_template_onu_attribute_fn                                                                                         get_pon_onu_template_onu_attribute;
    sai_create_pon_onu_template_onu_fw_bank_file_fn                                                                                   create_pon_onu_template_onu_fw_bank_file;
    sai_remove_pon_onu_template_onu_fw_bank_file_fn                                                                                   remove_pon_onu_template_onu_fw_bank_file;
    sai_set_pon_onu_template_onu_fw_bank_file_attribute_fn                                                                            set_pon_onu_template_onu_fw_bank_file_attribute;
    sai_get_pon_onu_template_onu_fw_bank_file_attribute_fn                                                                            get_pon_onu_template_onu_fw_bank_file_attribute;
    sai_create_pon_onu_template_olt_service_fn                                                                                        create_pon_onu_template_olt_service;
    sai_remove_pon_onu_template_olt_service_fn                                                                                        remove_pon_onu_template_olt_service;
    sai_set_pon_onu_template_olt_service_attribute_fn                                                                                 set_pon_onu_template_olt_service_attribute;
    sai_get_pon_onu_template_olt_service_attribute_fn                                                                                 get_pon_onu_template_olt_service_attribute;
    sai_create_pon_onu_template_olt_service_network_fn                                                                                create_pon_onu_template_olt_service_network;
    sai_remove_pon_onu_template_olt_service_network_fn                                                                                remove_pon_onu_template_olt_service_network;
    sai_set_pon_onu_template_olt_service_network_attribute_fn                                                                         set_pon_onu_template_olt_service_network_attribute;
    sai_get_pon_onu_template_olt_service_network_attribute_fn                                                                         get_pon_onu_template_olt_service_network_attribute;
    sai_create_pon_onu_template_service_config_value_fn                                                                               create_pon_onu_template_service_config_value;
    sai_remove_pon_onu_template_service_config_value_fn                                                                               remove_pon_onu_template_service_config_value;
    sai_set_pon_onu_template_service_config_value_attribute_fn                                                                        set_pon_onu_template_service_config_value_attribute;
    sai_get_pon_onu_template_service_config_value_attribute_fn                                                                        get_pon_onu_template_service_config_value_attribute;
    sai_create_pon_onu_template_uni_fn                                                                                                create_pon_onu_template_uni;
    sai_remove_pon_onu_template_uni_fn                                                                                                remove_pon_onu_template_uni;
    sai_set_pon_onu_template_uni_attribute_fn                                                                                         set_pon_onu_template_uni_attribute;
    sai_get_pon_onu_template_uni_attribute_fn                                                                                         get_pon_onu_template_uni_attribute;
    sai_create_pon_service_config_profile_header_fn                                                                                   create_pon_service_config_profile_header;
    sai_remove_pon_service_config_profile_header_fn                                                                                   remove_pon_service_config_profile_header;
    sai_set_pon_service_config_profile_header_attribute_fn                                                                            set_pon_service_config_profile_header_attribute;
    sai_get_pon_service_config_profile_header_attribute_fn                                                                            get_pon_service_config_profile_header_attribute;
    sai_create_pon_service_config_profile_header_compatibility_vendor_model_fn                                                        create_pon_service_config_profile_header_compatibility_vendor_model;
    sai_remove_pon_service_config_profile_header_compatibility_vendor_model_fn                                                        remove_pon_service_config_profile_header_compatibility_vendor_model;
    sai_set_pon_service_config_profile_header_compatibility_vendor_model_attribute_fn                                                 set_pon_service_config_profile_header_compatibility_vendor_model_attribute;
    sai_get_pon_service_config_profile_header_compatibility_vendor_model_attribute_fn                                                 get_pon_service_config_profile_header_compatibility_vendor_model_attribute;
    sai_create_pon_service_config_profile_header_inputs_ext_input_fn                                                                  create_pon_service_config_profile_header_inputs_ext_input;
    sai_remove_pon_service_config_profile_header_inputs_ext_input_fn                                                                  remove_pon_service_config_profile_header_inputs_ext_input;
    sai_set_pon_service_config_profile_header_inputs_ext_input_attribute_fn                                                           set_pon_service_config_profile_header_inputs_ext_input_attribute;
    sai_get_pon_service_config_profile_header_inputs_ext_input_attribute_fn                                                           get_pon_service_config_profile_header_inputs_ext_input_attribute;
    sai_create_pon_service_config_profile_ani_g_fn                                                                                    create_pon_service_config_profile_ani_g;
    sai_remove_pon_service_config_profile_ani_g_fn                                                                                    remove_pon_service_config_profile_ani_g;
    sai_set_pon_service_config_profile_ani_g_attribute_fn                                                                             set_pon_service_config_profile_ani_g_attribute;
    sai_get_pon_service_config_profile_ani_g_attribute_fn                                                                             get_pon_service_config_profile_ani_g_attribute;
    sai_create_pon_service_config_profile_cardholder_fn                                                                               create_pon_service_config_profile_cardholder;
    sai_remove_pon_service_config_profile_cardholder_fn                                                                               remove_pon_service_config_profile_cardholder;
    sai_set_pon_service_config_profile_cardholder_attribute_fn                                                                        set_pon_service_config_profile_cardholder_attribute;
    sai_get_pon_service_config_profile_cardholder_attribute_fn                                                                        get_pon_service_config_profile_cardholder_attribute;
    sai_create_pon_service_config_profile_circuit_pack_fn                                                                             create_pon_service_config_profile_circuit_pack;
    sai_remove_pon_service_config_profile_circuit_pack_fn                                                                             remove_pon_service_config_profile_circuit_pack;
    sai_set_pon_service_config_profile_circuit_pack_attribute_fn                                                                      set_pon_service_config_profile_circuit_pack_attribute;
    sai_get_pon_service_config_profile_circuit_pack_attribute_fn                                                                      get_pon_service_config_profile_circuit_pack_attribute;
    sai_create_pon_service_config_profile_enhanced_fec_pm_hist_data_fn                                                                create_pon_service_config_profile_enhanced_fec_pm_hist_data;
    sai_remove_pon_service_config_profile_enhanced_fec_pm_hist_data_fn                                                                remove_pon_service_config_profile_enhanced_fec_pm_hist_data;
    sai_set_pon_service_config_profile_enhanced_fec_pm_hist_data_attribute_fn                                                         set_pon_service_config_profile_enhanced_fec_pm_hist_data_attribute;
    sai_get_pon_service_config_profile_enhanced_fec_pm_hist_data_attribute_fn                                                         get_pon_service_config_profile_enhanced_fec_pm_hist_data_attribute;
    sai_create_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_fn                                                           create_pon_service_config_profile_enhanced_tc_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_fn                                                           remove_pon_service_config_profile_enhanced_tc_perf_mon_hist_data;
    sai_set_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_attribute_fn                                                    set_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_attribute_fn                                                    get_pon_service_config_profile_enhanced_tc_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_ethernet_frame_extended_pm_fn                                                               create_pon_service_config_profile_ethernet_frame_extended_pm;
    sai_remove_pon_service_config_profile_ethernet_frame_extended_pm_fn                                                               remove_pon_service_config_profile_ethernet_frame_extended_pm;
    sai_set_pon_service_config_profile_ethernet_frame_extended_pm_attribute_fn                                                        set_pon_service_config_profile_ethernet_frame_extended_pm_attribute;
    sai_get_pon_service_config_profile_ethernet_frame_extended_pm_attribute_fn                                                        get_pon_service_config_profile_ethernet_frame_extended_pm_attribute;
    sai_create_pon_service_config_profile_ethernet_frame_extended_pm64_bit_fn                                                         create_pon_service_config_profile_ethernet_frame_extended_pm64_bit;
    sai_remove_pon_service_config_profile_ethernet_frame_extended_pm64_bit_fn                                                         remove_pon_service_config_profile_ethernet_frame_extended_pm64_bit;
    sai_set_pon_service_config_profile_ethernet_frame_extended_pm64_bit_attribute_fn                                                  set_pon_service_config_profile_ethernet_frame_extended_pm64_bit_attribute;
    sai_get_pon_service_config_profile_ethernet_frame_extended_pm64_bit_attribute_fn                                                  get_pon_service_config_profile_ethernet_frame_extended_pm64_bit_attribute;
    sai_create_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_fn                                             create_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream;
    sai_remove_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_fn                                             remove_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream;
    sai_set_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_attribute_fn                                      set_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_attribute;
    sai_get_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_attribute_fn                                      get_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_downstream_attribute;
    sai_create_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_fn                                               create_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream;
    sai_remove_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_fn                                               remove_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream;
    sai_set_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_attribute_fn                                        set_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_attribute;
    sai_get_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_attribute_fn                                        get_pon_service_config_profile_ethernet_frame_perf_mon_hist_data_upstream_attribute;
    sai_create_pon_service_config_profile_ethernet_perf_mon_hist_data_fn                                                              create_pon_service_config_profile_ethernet_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_ethernet_perf_mon_hist_data_fn                                                              remove_pon_service_config_profile_ethernet_perf_mon_hist_data;
    sai_set_pon_service_config_profile_ethernet_perf_mon_hist_data_attribute_fn                                                       set_pon_service_config_profile_ethernet_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_ethernet_perf_mon_hist_data_attribute_fn                                                       get_pon_service_config_profile_ethernet_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_ethernet_perf_mon_hist_data3_fn                                                             create_pon_service_config_profile_ethernet_perf_mon_hist_data3;
    sai_remove_pon_service_config_profile_ethernet_perf_mon_hist_data3_fn                                                             remove_pon_service_config_profile_ethernet_perf_mon_hist_data3;
    sai_set_pon_service_config_profile_ethernet_perf_mon_hist_data3_attribute_fn                                                      set_pon_service_config_profile_ethernet_perf_mon_hist_data3_attribute;
    sai_get_pon_service_config_profile_ethernet_perf_mon_hist_data3_attribute_fn                                                      get_pon_service_config_profile_ethernet_perf_mon_hist_data3_attribute;
    sai_create_pon_service_config_profile_extended_vlan_tagging_operation_config_data_fn                                              create_pon_service_config_profile_extended_vlan_tagging_operation_config_data;
    sai_remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data_fn                                              remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data;
    sai_set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_attribute_fn                                       set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_attribute;
    sai_get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_attribute_fn                                       get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_attribute;
    sai_create_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_fn        create_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation;
    sai_remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_fn        remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation;
    sai_set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_attribute_fn set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_attribute;
    sai_get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_attribute_fn get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_received_frame_vlan_tagging_operation_attribute;
    sai_create_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_fn                         create_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping;
    sai_remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_fn                         remove_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping;
    sai_set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_attribute_fn                  set_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_attribute;
    sai_get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_attribute_fn                  get_pon_service_config_profile_extended_vlan_tagging_operation_config_data_dscp_to_pbit_mapping_attribute;
    sai_create_pon_service_config_profile_fec_perf_mon_hist_data_fn                                                                   create_pon_service_config_profile_fec_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_fec_perf_mon_hist_data_fn                                                                   remove_pon_service_config_profile_fec_perf_mon_hist_data;
    sai_set_pon_service_config_profile_fec_perf_mon_hist_data_attribute_fn                                                            set_pon_service_config_profile_fec_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_fec_perf_mon_hist_data_attribute_fn                                                            get_pon_service_config_profile_fec_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_fn                                                          create_pon_service_config_profile_gal_ethernet_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_fn                                                          remove_pon_service_config_profile_gal_ethernet_perf_mon_hist_data;
    sai_set_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_attribute_fn                                                   set_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_attribute_fn                                                   get_pon_service_config_profile_gal_ethernet_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_gal_ethernet_profile_fn                                                                     create_pon_service_config_profile_gal_ethernet_profile;
    sai_remove_pon_service_config_profile_gal_ethernet_profile_fn                                                                     remove_pon_service_config_profile_gal_ethernet_profile;
    sai_set_pon_service_config_profile_gal_ethernet_profile_attribute_fn                                                              set_pon_service_config_profile_gal_ethernet_profile_attribute;
    sai_get_pon_service_config_profile_gal_ethernet_profile_attribute_fn                                                              get_pon_service_config_profile_gal_ethernet_profile_attribute;
    sai_create_pon_service_config_profile_gem_interworking_tp_fn                                                                      create_pon_service_config_profile_gem_interworking_tp;
    sai_remove_pon_service_config_profile_gem_interworking_tp_fn                                                                      remove_pon_service_config_profile_gem_interworking_tp;
    sai_set_pon_service_config_profile_gem_interworking_tp_attribute_fn                                                               set_pon_service_config_profile_gem_interworking_tp_attribute;
    sai_get_pon_service_config_profile_gem_interworking_tp_attribute_fn                                                               get_pon_service_config_profile_gem_interworking_tp_attribute;
    sai_create_pon_service_config_profile_gem_port_network_ctp_fn                                                                     create_pon_service_config_profile_gem_port_network_ctp;
    sai_remove_pon_service_config_profile_gem_port_network_ctp_fn                                                                     remove_pon_service_config_profile_gem_port_network_ctp;
    sai_set_pon_service_config_profile_gem_port_network_ctp_attribute_fn                                                              set_pon_service_config_profile_gem_port_network_ctp_attribute;
    sai_get_pon_service_config_profile_gem_port_network_ctp_attribute_fn                                                              get_pon_service_config_profile_gem_port_network_ctp_attribute;
    sai_create_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_fn                                                  create_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_fn                                                  remove_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data;
    sai_set_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_attribute_fn                                           set_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_attribute_fn                                           get_pon_service_config_profile_gem_port_network_ctp_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_ieee8021p_mapper_service_profile_fn                                                         create_pon_service_config_profile_ieee8021p_mapper_service_profile;
    sai_remove_pon_service_config_profile_ieee8021p_mapper_service_profile_fn                                                         remove_pon_service_config_profile_ieee8021p_mapper_service_profile;
    sai_set_pon_service_config_profile_ieee8021p_mapper_service_profile_attribute_fn                                                  set_pon_service_config_profile_ieee8021p_mapper_service_profile_attribute;
    sai_get_pon_service_config_profile_ieee8021p_mapper_service_profile_attribute_fn                                                  get_pon_service_config_profile_ieee8021p_mapper_service_profile_attribute;
    sai_create_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_fn                                   create_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping;
    sai_remove_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_fn                                   remove_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping;
    sai_set_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_attribute_fn                            set_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_attribute;
    sai_get_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_attribute_fn                            get_pon_service_config_profile_ieee8021p_mapper_service_profile_dscp_to_p_bit_mapping_attribute;
    sai_create_pon_service_config_profile_ip_host_config_data_fn                                                                      create_pon_service_config_profile_ip_host_config_data;
    sai_remove_pon_service_config_profile_ip_host_config_data_fn                                                                      remove_pon_service_config_profile_ip_host_config_data;
    sai_set_pon_service_config_profile_ip_host_config_data_attribute_fn                                                               set_pon_service_config_profile_ip_host_config_data_attribute;
    sai_get_pon_service_config_profile_ip_host_config_data_attribute_fn                                                               get_pon_service_config_profile_ip_host_config_data_attribute;
    sai_create_pon_service_config_profile_ip_host_perf_mon_hist_data_fn                                                               create_pon_service_config_profile_ip_host_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_ip_host_perf_mon_hist_data_fn                                                               remove_pon_service_config_profile_ip_host_perf_mon_hist_data;
    sai_set_pon_service_config_profile_ip_host_perf_mon_hist_data_attribute_fn                                                        set_pon_service_config_profile_ip_host_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_ip_host_perf_mon_hist_data_attribute_fn                                                        get_pon_service_config_profile_ip_host_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_ipv6_host_config_data_fn                                                                    create_pon_service_config_profile_ipv6_host_config_data;
    sai_remove_pon_service_config_profile_ipv6_host_config_data_fn                                                                    remove_pon_service_config_profile_ipv6_host_config_data;
    sai_set_pon_service_config_profile_ipv6_host_config_data_attribute_fn                                                             set_pon_service_config_profile_ipv6_host_config_data_attribute;
    sai_get_pon_service_config_profile_ipv6_host_config_data_attribute_fn                                                             get_pon_service_config_profile_ipv6_host_config_data_attribute;
    sai_create_pon_service_config_profile_large_string_fn                                                                             create_pon_service_config_profile_large_string;
    sai_remove_pon_service_config_profile_large_string_fn                                                                             remove_pon_service_config_profile_large_string;
    sai_set_pon_service_config_profile_large_string_attribute_fn                                                                      set_pon_service_config_profile_large_string_attribute;
    sai_get_pon_service_config_profile_large_string_attribute_fn                                                                      get_pon_service_config_profile_large_string_attribute;
    sai_create_pon_service_config_profile_mac_bridge_perf_mon_hist_data_fn                                                            create_pon_service_config_profile_mac_bridge_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_mac_bridge_perf_mon_hist_data_fn                                                            remove_pon_service_config_profile_mac_bridge_perf_mon_hist_data;
    sai_set_pon_service_config_profile_mac_bridge_perf_mon_hist_data_attribute_fn                                                     set_pon_service_config_profile_mac_bridge_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_mac_bridge_perf_mon_hist_data_attribute_fn                                                     get_pon_service_config_profile_mac_bridge_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_mac_bridge_port_config_data_fn                                                              create_pon_service_config_profile_mac_bridge_port_config_data;
    sai_remove_pon_service_config_profile_mac_bridge_port_config_data_fn                                                              remove_pon_service_config_profile_mac_bridge_port_config_data;
    sai_set_pon_service_config_profile_mac_bridge_port_config_data_attribute_fn                                                       set_pon_service_config_profile_mac_bridge_port_config_data_attribute;
    sai_get_pon_service_config_profile_mac_bridge_port_config_data_attribute_fn                                                       get_pon_service_config_profile_mac_bridge_port_config_data_attribute;
    sai_create_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_fn                                                       create_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_fn                                                       remove_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data;
    sai_set_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_attribute_fn                                                set_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_attribute_fn                                                get_pon_service_config_profile_mac_bridge_port_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_mac_bridge_service_profile_fn                                                               create_pon_service_config_profile_mac_bridge_service_profile;
    sai_remove_pon_service_config_profile_mac_bridge_service_profile_fn                                                               remove_pon_service_config_profile_mac_bridge_service_profile;
    sai_set_pon_service_config_profile_mac_bridge_service_profile_attribute_fn                                                        set_pon_service_config_profile_mac_bridge_service_profile_attribute;
    sai_get_pon_service_config_profile_mac_bridge_service_profile_attribute_fn                                                        get_pon_service_config_profile_mac_bridge_service_profile_attribute;
    sai_create_pon_service_config_profile_multicast_gem_interworking_tp_fn                                                            create_pon_service_config_profile_multicast_gem_interworking_tp;
    sai_remove_pon_service_config_profile_multicast_gem_interworking_tp_fn                                                            remove_pon_service_config_profile_multicast_gem_interworking_tp;
    sai_set_pon_service_config_profile_multicast_gem_interworking_tp_attribute_fn                                                     set_pon_service_config_profile_multicast_gem_interworking_tp_attribute;
    sai_get_pon_service_config_profile_multicast_gem_interworking_tp_attribute_fn                                                     get_pon_service_config_profile_multicast_gem_interworking_tp_attribute;
    sai_create_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_fn                                     create_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address;
    sai_remove_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_fn                                     remove_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address;
    sai_set_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_attribute_fn                              set_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_attribute;
    sai_get_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_attribute_fn                              get_pon_service_config_profile_multicast_gem_interworking_tp_ipv4_multicast_address_attribute;
    sai_create_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_fn                                     create_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address;
    sai_remove_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_fn                                     remove_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address;
    sai_set_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_attribute_fn                              set_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_attribute;
    sai_get_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_attribute_fn                              get_pon_service_config_profile_multicast_gem_interworking_tp_ipv6_multicast_address_attribute;
    sai_create_pon_service_config_profile_olt_g_fn                                                                                    create_pon_service_config_profile_olt_g;
    sai_remove_pon_service_config_profile_olt_g_fn                                                                                    remove_pon_service_config_profile_olt_g;
    sai_set_pon_service_config_profile_olt_g_attribute_fn                                                                             set_pon_service_config_profile_olt_g_attribute;
    sai_get_pon_service_config_profile_olt_g_attribute_fn                                                                             get_pon_service_config_profile_olt_g_attribute;
    sai_create_pon_service_config_profile_onu2_g_fn                                                                                   create_pon_service_config_profile_onu2_g;
    sai_remove_pon_service_config_profile_onu2_g_fn                                                                                   remove_pon_service_config_profile_onu2_g;
    sai_set_pon_service_config_profile_onu2_g_attribute_fn                                                                            set_pon_service_config_profile_onu2_g_attribute;
    sai_get_pon_service_config_profile_onu2_g_attribute_fn                                                                            get_pon_service_config_profile_onu2_g_attribute;
    sai_create_pon_service_config_profile_onu_g_fn                                                                                    create_pon_service_config_profile_onu_g;
    sai_remove_pon_service_config_profile_onu_g_fn                                                                                    remove_pon_service_config_profile_onu_g;
    sai_set_pon_service_config_profile_onu_g_attribute_fn                                                                             set_pon_service_config_profile_onu_g_attribute;
    sai_get_pon_service_config_profile_onu_g_attribute_fn                                                                             get_pon_service_config_profile_onu_g_attribute;
    sai_create_pon_service_config_profile_onu_operational_perf_mon_hist_data_fn                                                       create_pon_service_config_profile_onu_operational_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_onu_operational_perf_mon_hist_data_fn                                                       remove_pon_service_config_profile_onu_operational_perf_mon_hist_data;
    sai_set_pon_service_config_profile_onu_operational_perf_mon_hist_data_attribute_fn                                                set_pon_service_config_profile_onu_operational_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_onu_operational_perf_mon_hist_data_attribute_fn                                                get_pon_service_config_profile_onu_operational_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_pptp_rs232_rs485_uni_fn                                                                     create_pon_service_config_profile_pptp_rs232_rs485_uni;
    sai_remove_pon_service_config_profile_pptp_rs232_rs485_uni_fn                                                                     remove_pon_service_config_profile_pptp_rs232_rs485_uni;
    sai_set_pon_service_config_profile_pptp_rs232_rs485_uni_attribute_fn                                                              set_pon_service_config_profile_pptp_rs232_rs485_uni_attribute;
    sai_get_pon_service_config_profile_pptp_rs232_rs485_uni_attribute_fn                                                              get_pon_service_config_profile_pptp_rs232_rs485_uni_attribute;
    sai_create_pon_service_config_profile_priority_queue_fn                                                                           create_pon_service_config_profile_priority_queue;
    sai_remove_pon_service_config_profile_priority_queue_fn                                                                           remove_pon_service_config_profile_priority_queue;
    sai_set_pon_service_config_profile_priority_queue_attribute_fn                                                                    set_pon_service_config_profile_priority_queue_attribute;
    sai_get_pon_service_config_profile_priority_queue_attribute_fn                                                                    get_pon_service_config_profile_priority_queue_attribute;
    sai_create_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_fn                                               create_pon_service_config_profile_priority_queue_packet_drop_queue_threshold;
    sai_remove_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_fn                                               remove_pon_service_config_profile_priority_queue_packet_drop_queue_threshold;
    sai_set_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_attribute_fn                                        set_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_attribute;
    sai_get_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_attribute_fn                                        get_pon_service_config_profile_priority_queue_packet_drop_queue_threshold_attribute;
    sai_create_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_fn                                                           create_pon_service_config_profile_rs232_rs485_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_fn                                                           remove_pon_service_config_profile_rs232_rs485_perf_mon_hist_data;
    sai_set_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_attribute_fn                                                    set_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_attribute_fn                                                    get_pon_service_config_profile_rs232_rs485_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_rs232_rs485_port_oper_config_data_fn                                                        create_pon_service_config_profile_rs232_rs485_port_oper_config_data;
    sai_remove_pon_service_config_profile_rs232_rs485_port_oper_config_data_fn                                                        remove_pon_service_config_profile_rs232_rs485_port_oper_config_data;
    sai_set_pon_service_config_profile_rs232_rs485_port_oper_config_data_attribute_fn                                                 set_pon_service_config_profile_rs232_rs485_port_oper_config_data_attribute;
    sai_get_pon_service_config_profile_rs232_rs485_port_oper_config_data_attribute_fn                                                 get_pon_service_config_profile_rs232_rs485_port_oper_config_data_attribute;
    sai_create_pon_service_config_profile_ssh_server_operation_fn                                                                     create_pon_service_config_profile_ssh_server_operation;
    sai_remove_pon_service_config_profile_ssh_server_operation_fn                                                                     remove_pon_service_config_profile_ssh_server_operation;
    sai_set_pon_service_config_profile_ssh_server_operation_attribute_fn                                                              set_pon_service_config_profile_ssh_server_operation_attribute;
    sai_get_pon_service_config_profile_ssh_server_operation_attribute_fn                                                              get_pon_service_config_profile_ssh_server_operation_attribute;
    sai_create_pon_service_config_profile_ssh_server_port_data_fn                                                                     create_pon_service_config_profile_ssh_server_port_data;
    sai_remove_pon_service_config_profile_ssh_server_port_data_fn                                                                     remove_pon_service_config_profile_ssh_server_port_data;
    sai_set_pon_service_config_profile_ssh_server_port_data_attribute_fn                                                              set_pon_service_config_profile_ssh_server_port_data_attribute;
    sai_get_pon_service_config_profile_ssh_server_port_data_attribute_fn                                                              get_pon_service_config_profile_ssh_server_port_data_attribute;
    sai_create_pon_service_config_profile_tcont_fn                                                                                    create_pon_service_config_profile_tcont;
    sai_remove_pon_service_config_profile_tcont_fn                                                                                    remove_pon_service_config_profile_tcont;
    sai_set_pon_service_config_profile_tcont_attribute_fn                                                                             set_pon_service_config_profile_tcont_attribute;
    sai_get_pon_service_config_profile_tcont_attribute_fn                                                                             get_pon_service_config_profile_tcont_attribute;
    sai_create_pon_service_config_profile_tcp_udp_config_data_fn                                                                      create_pon_service_config_profile_tcp_udp_config_data;
    sai_remove_pon_service_config_profile_tcp_udp_config_data_fn                                                                      remove_pon_service_config_profile_tcp_udp_config_data;
    sai_set_pon_service_config_profile_tcp_udp_config_data_attribute_fn                                                               set_pon_service_config_profile_tcp_udp_config_data_attribute;
    sai_get_pon_service_config_profile_tcp_udp_config_data_attribute_fn                                                               get_pon_service_config_profile_tcp_udp_config_data_attribute;
    sai_create_pon_service_config_profile_tcp_udp_perf_mon_hist_data_fn                                                               create_pon_service_config_profile_tcp_udp_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_tcp_udp_perf_mon_hist_data_fn                                                               remove_pon_service_config_profile_tcp_udp_perf_mon_hist_data;
    sai_set_pon_service_config_profile_tcp_udp_perf_mon_hist_data_attribute_fn                                                        set_pon_service_config_profile_tcp_udp_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_tcp_udp_perf_mon_hist_data_attribute_fn                                                        get_pon_service_config_profile_tcp_udp_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_threshold_data64_bit_fn                                                                     create_pon_service_config_profile_threshold_data64_bit;
    sai_remove_pon_service_config_profile_threshold_data64_bit_fn                                                                     remove_pon_service_config_profile_threshold_data64_bit;
    sai_set_pon_service_config_profile_threshold_data64_bit_attribute_fn                                                              set_pon_service_config_profile_threshold_data64_bit_attribute;
    sai_get_pon_service_config_profile_threshold_data64_bit_attribute_fn                                                              get_pon_service_config_profile_threshold_data64_bit_attribute;
    sai_create_pon_service_config_profile_threshold_data_one_fn                                                                       create_pon_service_config_profile_threshold_data_one;
    sai_remove_pon_service_config_profile_threshold_data_one_fn                                                                       remove_pon_service_config_profile_threshold_data_one;
    sai_set_pon_service_config_profile_threshold_data_one_attribute_fn                                                                set_pon_service_config_profile_threshold_data_one_attribute;
    sai_get_pon_service_config_profile_threshold_data_one_attribute_fn                                                                get_pon_service_config_profile_threshold_data_one_attribute;
    sai_create_pon_service_config_profile_threshold_data_two_fn                                                                       create_pon_service_config_profile_threshold_data_two;
    sai_remove_pon_service_config_profile_threshold_data_two_fn                                                                       remove_pon_service_config_profile_threshold_data_two;
    sai_set_pon_service_config_profile_threshold_data_two_attribute_fn                                                                set_pon_service_config_profile_threshold_data_two_attribute;
    sai_get_pon_service_config_profile_threshold_data_two_attribute_fn                                                                get_pon_service_config_profile_threshold_data_two_attribute;
    sai_create_pon_service_config_profile_traffic_descriptor_fn                                                                       create_pon_service_config_profile_traffic_descriptor;
    sai_remove_pon_service_config_profile_traffic_descriptor_fn                                                                       remove_pon_service_config_profile_traffic_descriptor;
    sai_set_pon_service_config_profile_traffic_descriptor_attribute_fn                                                                set_pon_service_config_profile_traffic_descriptor_attribute;
    sai_get_pon_service_config_profile_traffic_descriptor_attribute_fn                                                                get_pon_service_config_profile_traffic_descriptor_attribute;
    sai_create_pon_service_config_profile_traffic_scheduler_fn                                                                        create_pon_service_config_profile_traffic_scheduler;
    sai_remove_pon_service_config_profile_traffic_scheduler_fn                                                                        remove_pon_service_config_profile_traffic_scheduler;
    sai_set_pon_service_config_profile_traffic_scheduler_attribute_fn                                                                 set_pon_service_config_profile_traffic_scheduler_attribute;
    sai_get_pon_service_config_profile_traffic_scheduler_attribute_fn                                                                 get_pon_service_config_profile_traffic_scheduler_attribute;
    sai_create_pon_service_config_profile_virtual_ethernet_interface_pt_fn                                                            create_pon_service_config_profile_virtual_ethernet_interface_pt;
    sai_remove_pon_service_config_profile_virtual_ethernet_interface_pt_fn                                                            remove_pon_service_config_profile_virtual_ethernet_interface_pt;
    sai_set_pon_service_config_profile_virtual_ethernet_interface_pt_attribute_fn                                                     set_pon_service_config_profile_virtual_ethernet_interface_pt_attribute;
    sai_get_pon_service_config_profile_virtual_ethernet_interface_pt_attribute_fn                                                     get_pon_service_config_profile_virtual_ethernet_interface_pt_attribute;
    sai_create_pon_service_config_profile_vlan_tagging_filter_data_fn                                                                 create_pon_service_config_profile_vlan_tagging_filter_data;
    sai_remove_pon_service_config_profile_vlan_tagging_filter_data_fn                                                                 remove_pon_service_config_profile_vlan_tagging_filter_data;
    sai_set_pon_service_config_profile_vlan_tagging_filter_data_attribute_fn                                                          set_pon_service_config_profile_vlan_tagging_filter_data_attribute;
    sai_get_pon_service_config_profile_vlan_tagging_filter_data_attribute_fn                                                          get_pon_service_config_profile_vlan_tagging_filter_data_attribute;
    sai_create_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_fn                                                     create_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter;
    sai_remove_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_fn                                                     remove_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter;
    sai_set_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_attribute_fn                                              set_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_attribute;
    sai_get_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_attribute_fn                                              get_pon_service_config_profile_vlan_tagging_filter_data_vlan_filter_attribute;
    sai_create_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_fn                                                create_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_fn                                                remove_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data;
    sai_set_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_attribute_fn                                         set_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_attribute_fn                                         get_pon_service_config_profile_xg_pon_downstream_mgmt_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_fn                                                             create_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_fn                                                             remove_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data;
    sai_set_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_attribute_fn                                                      set_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_attribute_fn                                                      get_pon_service_config_profile_xg_pon_tc_perf_mon_hist_data_attribute;
    sai_create_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_fn                                                  create_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data;
    sai_remove_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_fn                                                  remove_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data;
    sai_set_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_attribute_fn                                           set_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_attribute;
    sai_get_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_attribute_fn                                           get_pon_service_config_profile_xg_pon_upstream_mgmt_perf_mon_hist_data_attribute;
    sai_create_pon_sla_profile_fn                                                                                                     create_pon_sla_profile;
    sai_remove_pon_sla_profile_fn                                                                                                     remove_pon_sla_profile;
    sai_set_pon_sla_profile_attribute_fn                                                                                              set_pon_sla_profile_attribute;
    sai_get_pon_sla_profile_attribute_fn                                                                                              get_pon_sla_profile_attribute;
    sai_create_pon_sla_profile_controller_fn                                                                                          create_pon_sla_profile_controller;
    sai_remove_pon_sla_profile_controller_fn                                                                                          remove_pon_sla_profile_controller;
    sai_set_pon_sla_profile_controller_attribute_fn                                                                                   set_pon_sla_profile_controller_attribute;
    sai_get_pon_sla_profile_controller_attribute_fn                                                                                   get_pon_sla_profile_controller_attribute;
    sai_execute_pon_action_fn                                                                                                         execute_pon_action;
} sai_pon_api_t;

/**
 * @}
 */

#endif /* __SAIEXPERIMENTALPON_H_ */
