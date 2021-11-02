/* Copyright 2021-present Intel Corporation.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <arpa/inet.h>

#include <iostream>
#include <cstring>
#include "sai_rpc.h"

extern "C" {
#include "sai.h"
#include "saitypes.h"
#include "saimetadatatypes.h"
#include "saimetadatautils.h"
#include "saimetadata.h"
}

using namespace ::sai;

unsigned int sai_thrift_mac_t_parse(const std::string s, void *data) {
  unsigned int i, j = 0;
  unsigned char *m = static_cast<unsigned char *>(data);
  memset(m, 0, 6);
  for (i = 0; i < s.size(); i++) {
    char let = s.c_str()[i];
    if (let >= '0' && let <= '9') {
      m[j / 2] = (m[j / 2] << 4) + (let - '0');
      j++;
    } else if (let >= 'a' && let <= 'f') {
      m[j / 2] = (m[j / 2] << 4) + (let - 'a' + 10);
      j++;
    } else if (let >= 'A' && let <= 'F') {
      m[j / 2] = (m[j / 2] << 4) + (let - 'A' + 10);
      j++;
    }
  }
  return (j == 12);
}

void sai_thrift_ip4_t_parse(const std::string s, unsigned int *m) {
  unsigned char r = 0;
  unsigned int i;
  *m = 0;
  for (i = 0; i < s.size(); i++) {
    char let = s.c_str()[i];
    if (let >= '0' && let <= '9') {
      r = (r * 10) + (let - '0');
    } else {
      *m = (*m << 8) | r;
      r = 0;
    }
  }
  *m = (*m << 8) | (r & 0xFF);
  *m = htonl(*m);
  return;
}

void sai_thrift_ip6_t_parse(const std::string s, unsigned char *v6_ip) {
  const char *v6_str = s.c_str();
  inet_pton(AF_INET6, v6_str, v6_ip);
  return;
}

void sai_thrift_ip_address_t_parse(
    const sai_thrift_ip_address_t &thrift_ip_address,
    sai_ip_address_t *ip_address) {
  ip_address->addr_family = (sai_ip_addr_family_t)thrift_ip_address.addr_family;
  if ((sai_ip_addr_family_t)thrift_ip_address.addr_family ==
      SAI_IP_ADDR_FAMILY_IPV4) {
    sai_thrift_ip4_t_parse(thrift_ip_address.addr.ip4, &ip_address->addr.ip4);
  } else {
    sai_thrift_ip6_t_parse(thrift_ip_address.addr.ip6, ip_address->addr.ip6);
  }
}

void sai_thrift_ip_prefix_t_parse(
    const sai_thrift_ip_prefix_t &thrift_ip_prefix,
    sai_ip_prefix_t *ip_prefix) {
  ip_prefix->addr_family = (sai_ip_addr_family_t)thrift_ip_prefix.addr_family;
  if ((sai_ip_addr_family_t)thrift_ip_prefix.addr_family ==
      SAI_IP_ADDR_FAMILY_IPV4) {
    sai_thrift_ip4_t_parse(thrift_ip_prefix.addr.ip4, &ip_prefix->addr.ip4);
    sai_thrift_ip4_t_parse(thrift_ip_prefix.mask.ip4, &ip_prefix->mask.ip4);
  } else {
    sai_thrift_ip6_t_parse(thrift_ip_prefix.addr.ip6, ip_prefix->addr.ip6);
    sai_thrift_ip6_t_parse(thrift_ip_prefix.mask.ip6, ip_prefix->mask.ip6);
  }
}

void convert_attr_thrift_to_sai(const sai_object_type_t sai_ot,
                                const sai_thrift_attribute_t &thrift_attr,
                                sai_attribute_t *sai_attr) {
  const auto attr_md = sai_metadata_get_attr_metadata(sai_ot, thrift_attr.id);
  sai_attr->id = thrift_attr.id;

  if (attr_md == NULL) {
    return;
  }

  switch (attr_md->attrvaluetype) {
    case SAI_ATTR_VALUE_TYPE_BOOL:
      sai_attr->value.booldata = thrift_attr.value.booldata;
      break;
    case SAI_ATTR_VALUE_TYPE_CHARDATA:
      // 32 is chardata size in saitypes.h
      std::memcpy(
          sai_attr->value.chardata, thrift_attr.value.chardata.c_str(), 32);
      break;
    case SAI_ATTR_VALUE_TYPE_UINT8:
      sai_attr->value.u8 = thrift_attr.value.u8;
      break;
    case SAI_ATTR_VALUE_TYPE_INT8:
      sai_attr->value.s8 = thrift_attr.value.s8;
      break;
    case SAI_ATTR_VALUE_TYPE_UINT16:
      sai_attr->value.u16 = thrift_attr.value.u16;
      break;
    case SAI_ATTR_VALUE_TYPE_INT16:
      sai_attr->value.s16 = thrift_attr.value.s16;
      break;
    case SAI_ATTR_VALUE_TYPE_UINT32:
      sai_attr->value.u32 = thrift_attr.value.u32;
      break;
    case SAI_ATTR_VALUE_TYPE_INT32:
      sai_attr->value.s32 = thrift_attr.value.s32;
      break;
    case SAI_ATTR_VALUE_TYPE_UINT64:
      sai_attr->value.u64 = thrift_attr.value.u64;
      break;
    case SAI_ATTR_VALUE_TYPE_INT64:
      sai_attr->value.s64 = thrift_attr.value.s64;
      break;
    case SAI_ATTR_VALUE_TYPE_POINTER:
      // not supported
      break;
    case SAI_ATTR_VALUE_TYPE_MAC:
      sai_thrift_mac_t_parse(thrift_attr.value.mac, &sai_attr->value.mac);
      break;
    case SAI_ATTR_VALUE_TYPE_IPV4:
      sai_thrift_ip4_t_parse(thrift_attr.value.ip4, &sai_attr->value.ip4);
      break;
    case SAI_ATTR_VALUE_TYPE_IPV6:
      sai_thrift_ip6_t_parse(thrift_attr.value.ip6, sai_attr->value.ip6);
      break;
    case SAI_ATTR_VALUE_TYPE_IP_ADDRESS:
      sai_thrift_ip_address_t_parse(thrift_attr.value.ipaddr,
                                    &sai_attr->value.ipaddr);
      break;
    case SAI_ATTR_VALUE_TYPE_IP_PREFIX:
      sai_thrift_ip_prefix_t_parse(thrift_attr.value.ipprefix,
                                   &sai_attr->value.ipprefix);
      break;
    case SAI_ATTR_VALUE_TYPE_OBJECT_ID:
      sai_attr->value.oid = thrift_attr.value.oid;
      break;
    case SAI_ATTR_VALUE_TYPE_OBJECT_LIST: {
      sai_attr->value.objlist.list = (sai_object_id_t *)malloc(
          sizeof(sai_object_id_t) * thrift_attr.value.objlist.count);
      int i = 0;
      for (auto obj : thrift_attr.value.objlist.idlist)
        sai_attr->value.objlist.list[i++] = obj;
      sai_attr->value.objlist.count = thrift_attr.value.objlist.count;
    } break;
    case SAI_ATTR_VALUE_TYPE_UINT8_LIST: {
      sai_attr->value.u8list.list =
          (uint8_t *)malloc(sizeof(uint8_t) * thrift_attr.value.u8list.count);
      int i = 0;
      for (auto u8 : thrift_attr.value.u8list.uint8list)
        sai_attr->value.u8list.list[i++] = u8;
      sai_attr->value.u8list.count = thrift_attr.value.u8list.count;
    } break;
    case SAI_ATTR_VALUE_TYPE_INT8_LIST: {
      sai_attr->value.s8list.list =
          (int8_t *)malloc(sizeof(int8_t) * thrift_attr.value.s8list.count);
      int i = 0;
      for (auto s8 : thrift_attr.value.s8list.int8list)
        sai_attr->value.s8list.list[i++] = s8;
      sai_attr->value.s8list.count = thrift_attr.value.s8list.count;
    } break;
    case SAI_ATTR_VALUE_TYPE_UINT16_LIST: {
      sai_attr->value.u16list.list = (uint16_t *)malloc(
          sizeof(uint16_t) * thrift_attr.value.u16list.count);
      int i = 0;
      for (auto u16 : thrift_attr.value.u16list.uint16list)
        sai_attr->value.u16list.list[i++] = u16;
      sai_attr->value.u16list.count = thrift_attr.value.u16list.count;
    } break;
    case SAI_ATTR_VALUE_TYPE_INT16_LIST: {
      sai_attr->value.s16list.list =
          (int16_t *)malloc(sizeof(int16_t) * thrift_attr.value.s16list.count);
      int i = 0;
      for (auto s16 : thrift_attr.value.s16list.int16list)
        sai_attr->value.s16list.list[i++] = s16;
      sai_attr->value.s16list.count = thrift_attr.value.s16list.count;
    } break;
    case SAI_ATTR_VALUE_TYPE_UINT32_LIST: {
      sai_attr->value.u32list.list = (uint32_t *)malloc(
          sizeof(uint32_t) * thrift_attr.value.u32list.count);
      int i = 0;
      for (auto u32 : thrift_attr.value.u32list.uint32list)
        sai_attr->value.u32list.list[i++] = u32;
      sai_attr->value.u32list.count = thrift_attr.value.u32list.count;
    } break;
    case SAI_ATTR_VALUE_TYPE_INT32_LIST: {
      sai_attr->value.s32list.list =
          (int32_t *)malloc(sizeof(int32_t) * thrift_attr.value.s32list.count);
      int i = 0;
      for (auto s32 : thrift_attr.value.s32list.int32list)
        sai_attr->value.s32list.list[i++] = s32;
      sai_attr->value.s32list.count = thrift_attr.value.s32list.count;
    } break;
    case SAI_ATTR_VALUE_TYPE_UINT32_RANGE:
      sai_attr->value.u32range.min = thrift_attr.value.u32range.min;
      sai_attr->value.u32range.max = thrift_attr.value.u32range.max;
      break;
    case SAI_ATTR_VALUE_TYPE_INT32_RANGE:
      sai_attr->value.s32range.min = thrift_attr.value.s32range.min;
      sai_attr->value.s32range.max = thrift_attr.value.s32range.max;
      break;

    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_BOOL:
      sai_attr->value.aclfield.enable = thrift_attr.value.aclfield.enable;
      sai_attr->value.aclfield.data.booldata =
          thrift_attr.value.aclfield.data.booldata;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8:
      sai_attr->value.aclfield.enable = thrift_attr.value.aclfield.enable;
      sai_attr->value.aclfield.data.u8 = thrift_attr.value.aclfield.data.u8;
      sai_attr->value.aclfield.mask.u8 = thrift_attr.value.aclfield.mask.u8;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT8:
      sai_attr->value.aclfield.enable = thrift_attr.value.aclfield.enable;
      sai_attr->value.aclfield.data.s8 = thrift_attr.value.aclfield.data.s8;
      sai_attr->value.aclfield.mask.s8 = thrift_attr.value.aclfield.mask.s8;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16:
      sai_attr->value.aclfield.enable = thrift_attr.value.aclfield.enable;
      sai_attr->value.aclfield.data.u16 = thrift_attr.value.aclfield.data.u16;
      sai_attr->value.aclfield.mask.u16 = thrift_attr.value.aclfield.mask.u16;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT16:
      sai_attr->value.aclfield.enable = thrift_attr.value.aclfield.enable;
      sai_attr->value.aclfield.data.s16 = thrift_attr.value.aclfield.data.s16;
      sai_attr->value.aclfield.mask.s16 = thrift_attr.value.aclfield.mask.s16;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT32:
      sai_attr->value.aclfield.enable = thrift_attr.value.aclfield.enable;
      sai_attr->value.aclfield.data.u32 = thrift_attr.value.aclfield.data.u32;
      sai_attr->value.aclfield.mask.u32 = thrift_attr.value.aclfield.mask.u32;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32:
      sai_attr->value.aclfield.enable = thrift_attr.value.aclfield.enable;
      sai_attr->value.aclfield.data.s32 = thrift_attr.value.aclfield.data.s32;
      sai_attr->value.aclfield.mask.s32 = thrift_attr.value.aclfield.mask.s32;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC:
      sai_attr->value.aclfield.enable = thrift_attr.value.aclfield.enable;
      sai_thrift_mac_t_parse(thrift_attr.value.aclfield.data.mac,
                             &sai_attr->value.aclfield.data.mac);
      sai_thrift_mac_t_parse(thrift_attr.value.aclfield.mask.mac,
                             &sai_attr->value.aclfield.mask.mac);
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4:
      sai_attr->value.aclfield.enable = thrift_attr.value.aclfield.enable;
      sai_thrift_ip4_t_parse(thrift_attr.value.aclfield.data.ip4,
                             &sai_attr->value.aclfield.data.ip4);
      sai_thrift_ip4_t_parse(thrift_attr.value.aclfield.mask.ip4,
                             &sai_attr->value.aclfield.mask.ip4);
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6:
      sai_attr->value.aclfield.enable = thrift_attr.value.aclfield.enable;
      sai_thrift_ip6_t_parse(thrift_attr.value.aclfield.data.ip6,
                             sai_attr->value.aclfield.data.ip6);
      sai_thrift_ip6_t_parse(thrift_attr.value.aclfield.mask.ip6,
                             sai_attr->value.aclfield.mask.ip6);
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID:
      sai_attr->value.aclfield.enable = thrift_attr.value.aclfield.enable;
      sai_attr->value.aclfield.data.oid = thrift_attr.value.aclfield.data.oid;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST: {
      int i = 0;
      sai_attr->value.aclfield.enable = thrift_attr.value.aclfield.enable;
      sai_attr->value.aclfield.data.objlist.list = (sai_object_id_t *)malloc(
          sizeof(sai_object_id_t) *
          thrift_attr.value.aclfield.data.objlist.count);
      for (auto obj : thrift_attr.value.aclfield.data.objlist.idlist)
        sai_attr->value.aclfield.data.objlist.list[i++] = obj;
      sai_attr->value.aclfield.data.objlist.count =
          thrift_attr.value.aclfield.data.objlist.count;
    } break;
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST: {
      int i = 0;
      sai_attr->value.aclfield.enable = thrift_attr.value.aclfield.enable;
      sai_attr->value.aclfield.data.u8list.list = (uint8_t *)malloc(
          sizeof(uint8_t) * thrift_attr.value.aclfield.data.u8list.count);
      for (auto obj : thrift_attr.value.aclfield.data.u8list.uint8list)
        sai_attr->value.aclfield.data.u8list.list[i++] = obj;
      sai_attr->value.aclfield.data.u8list.count =
          thrift_attr.value.aclfield.data.u8list.count;
      i = 0;
      sai_attr->value.aclfield.mask.u8list.list = (uint8_t *)malloc(
          sizeof(uint8_t) * thrift_attr.value.aclfield.mask.u8list.count);
      for (auto obj : thrift_attr.value.aclfield.mask.u8list.uint8list)
        sai_attr->value.aclfield.mask.u8list.list[i++] = obj;
      sai_attr->value.aclfield.mask.u8list.count =
          thrift_attr.value.aclfield.mask.u8list.count;
    } break;
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_BOOL:
      sai_attr->value.aclaction.enable = thrift_attr.value.aclaction.enable;
      sai_attr->value.aclaction.parameter.booldata =
          thrift_attr.value.aclaction.parameter.booldata;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT8:
      sai_attr->value.aclaction.enable = thrift_attr.value.aclaction.enable;
      sai_attr->value.aclaction.parameter.u8 =
          thrift_attr.value.aclaction.parameter.u8;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT8:
      sai_attr->value.aclaction.enable = thrift_attr.value.aclaction.enable;
      sai_attr->value.aclaction.parameter.s8 =
          thrift_attr.value.aclaction.parameter.s8;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16:
      sai_attr->value.aclaction.enable = thrift_attr.value.aclaction.enable;
      sai_attr->value.aclaction.parameter.u16 =
          thrift_attr.value.aclaction.parameter.u16;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT16:
      sai_attr->value.aclaction.enable = thrift_attr.value.aclaction.enable;
      sai_attr->value.aclaction.parameter.s16 =
          thrift_attr.value.aclaction.parameter.s16;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT32:
      sai_attr->value.aclaction.enable = thrift_attr.value.aclaction.enable;
      sai_attr->value.aclaction.parameter.u32 =
          thrift_attr.value.aclaction.parameter.u32;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32:
      sai_attr->value.aclaction.enable = thrift_attr.value.aclaction.enable;
      sai_attr->value.aclaction.parameter.s32 =
          thrift_attr.value.aclaction.parameter.s32;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_MAC:
      sai_attr->value.aclaction.enable = thrift_attr.value.aclaction.enable;
      sai_thrift_mac_t_parse(thrift_attr.value.aclaction.parameter.mac,
                             &sai_attr->value.aclaction.parameter.mac);
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV4:
      sai_attr->value.aclaction.enable = thrift_attr.value.aclaction.enable;
      sai_thrift_ip4_t_parse(thrift_attr.value.aclaction.parameter.ip4,
                             &sai_attr->value.aclaction.parameter.ip4);
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV6:
      sai_attr->value.aclaction.enable = thrift_attr.value.aclaction.enable;
      sai_thrift_ip6_t_parse(thrift_attr.value.aclaction.parameter.ip6,
                             sai_attr->value.aclaction.parameter.ip6);
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IP_ADDRESS:
      sai_attr->value.aclaction.enable = thrift_attr.value.aclaction.enable;
      sai_thrift_ip_address_t_parse(
          thrift_attr.value.aclaction.parameter.ipaddr,
          &sai_attr->value.aclaction.parameter.ipaddr);
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID:
      sai_attr->value.aclaction.enable = thrift_attr.value.aclaction.enable;
      sai_attr->value.aclaction.parameter.oid =
          thrift_attr.value.aclaction.parameter.oid;
      break;
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST: {
      int i = 0;
      sai_attr->value.aclaction.enable = thrift_attr.value.aclaction.enable;
      sai_attr->value.aclaction.parameter.objlist.list =
          (sai_object_id_t *)malloc(
              sizeof(sai_object_id_t) *
              thrift_attr.value.aclaction.parameter.objlist.count);
      for (auto obj : thrift_attr.value.aclaction.parameter.objlist.idlist)
        sai_attr->value.aclaction.parameter.objlist.list[i++] = obj;
      sai_attr->value.aclaction.parameter.objlist.count =
          thrift_attr.value.aclaction.parameter.objlist.count;
    } break;

    case SAI_ATTR_VALUE_TYPE_ACL_CAPABILITY: {
      sai_attr->value.aclcapability.is_action_list_mandatory =
          thrift_attr.value.aclcapability.is_action_list_mandatory;
      sai_attr->value.aclcapability.action_list.list = (int32_t *)malloc(
          sizeof(int32_t) * thrift_attr.value.aclcapability.action_list.count);
      int i = 0;
      for (auto s32 : thrift_attr.value.aclcapability.action_list.int32list)
        sai_attr->value.aclcapability.action_list.list[i++] = s32;
      sai_attr->value.aclcapability.action_list.count =
          thrift_attr.value.aclcapability.action_list.count;
    } break;
    case SAI_ATTR_VALUE_TYPE_ACL_RESOURCE_LIST: {
      sai_attr->value.aclresource.list = (sai_acl_resource_t *)malloc(
          sizeof(sai_acl_resource_t) * thrift_attr.value.aclresource.count);
      int i = 0;
      for (auto resource : thrift_attr.value.aclresource.resourcelist) {
        sai_attr->value.aclresource.list[i].stage =
            static_cast<sai_acl_stage_t>(resource.stage);
        sai_attr->value.aclresource.list[i].bind_point =
            static_cast<sai_acl_bind_point_type_t>(resource.bind_point);
        sai_attr->value.aclresource.list[i++].avail_num = resource.avail_num;
      }
      sai_attr->value.aclresource.count = thrift_attr.value.aclresource.count;
    } break;
    case SAI_ATTR_VALUE_TYPE_IP_ADDRESS_LIST: {
      sai_attr->value.ipaddrlist.list = (sai_ip_address_t *)malloc(
          sizeof(sai_ip_address_t) * thrift_attr.value.ipaddrlist.count);
      int i = 0;
      for (auto address : thrift_attr.value.ipaddrlist.addresslist) {
        sai_thrift_ip_address_t_parse(address,
                                      &sai_attr->value.ipaddrlist.list[i++]);
      }
      sai_attr->value.ipaddrlist.count = thrift_attr.value.ipaddrlist.count;
    } break;
    case SAI_ATTR_VALUE_TYPE_MAP_LIST:
    case SAI_ATTR_VALUE_TYPE_VLAN_LIST:
    case SAI_ATTR_VALUE_TYPE_QOS_MAP_LIST: {
      sai_attr->value.qosmap.list = (sai_qos_map_t *)malloc(
          sizeof(sai_qos_map_t) * thrift_attr.value.qosmap.count);
      int i = 0;
      for (auto qosmap : thrift_attr.value.qosmap.maplist) {
        // key
        sai_attr->value.qosmap.list[i].key.tc = qosmap.key.tc;
        sai_attr->value.qosmap.list[i].key.dscp = qosmap.key.dscp;
        sai_attr->value.qosmap.list[i].key.dot1p = qosmap.key.dot1p;
        sai_attr->value.qosmap.list[i].key.prio = qosmap.key.prio;
        sai_attr->value.qosmap.list[i].key.pg = qosmap.key.pg;
        sai_attr->value.qosmap.list[i].key.queue_index = qosmap.key.queue_index;
        sai_attr->value.qosmap.list[i].key.color =
            static_cast<sai_packet_color_t>(qosmap.key.color);
        sai_attr->value.qosmap.list[i].key.mpls_exp = qosmap.key.mpls_exp;
        // value
        sai_attr->value.qosmap.list[i].value.tc = qosmap.value.tc;
        sai_attr->value.qosmap.list[i].value.dscp = qosmap.value.dscp;
        sai_attr->value.qosmap.list[i].value.dot1p = qosmap.value.dot1p;
        sai_attr->value.qosmap.list[i].value.prio = qosmap.value.prio;
        sai_attr->value.qosmap.list[i].value.pg = qosmap.value.pg;
        sai_attr->value.qosmap.list[i].value.queue_index =
            qosmap.value.queue_index;
        sai_attr->value.qosmap.list[i].value.color =
            static_cast<sai_packet_color_t>(qosmap.value.color);
        sai_attr->value.qosmap.list[i].value.mpls_exp = qosmap.value.mpls_exp;
        i++;
      }
      sai_attr->value.qosmap.count = thrift_attr.value.qosmap.count;
    } break;
    case SAI_ATTR_VALUE_TYPE_TLV_LIST:
    case SAI_ATTR_VALUE_TYPE_SEGMENT_LIST:
    case SAI_ATTR_VALUE_TYPE_PORT_EYE_VALUES_LIST:
    case SAI_ATTR_VALUE_TYPE_TIMESPEC:
    case SAI_ATTR_VALUE_TYPE_NAT_ENTRY_DATA:
      // not supported
      break;
    default:
      break;
  }
}

std::string sai_ip4_t_to_thrift(const sai_ip4_t ip4) {
  char str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(ip4), str, INET_ADDRSTRLEN);
  return str;
}

std::string sai_ip6_t_to_thrift(const sai_ip6_t ip6) {
  char str[INET6_ADDRSTRLEN];
  inet_ntop(AF_INET6, ip6, str, INET6_ADDRSTRLEN);
  return str;
}

void sai_ip_address_t_to_thrift(sai_thrift_ip_address_t &thrift_ip,
                                const sai_ip_address_t ip) {
  if (ip.addr_family == SAI_IP_ADDR_FAMILY_IPV4) {
    thrift_ip.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    thrift_ip.addr.ip4 = sai_ip4_t_to_thrift(ip.addr.ip4);
  } else if (ip.addr_family == SAI_IP_ADDR_FAMILY_IPV6) {
    thrift_ip.addr_family = SAI_IP_ADDR_FAMILY_IPV6;
    thrift_ip.addr.ip6 = sai_ip6_t_to_thrift(ip.addr.ip6);
  }
}

void sai_ip_prefix_t_to_thrift(sai_thrift_ip_prefix_t &thrift_ip,
                               const sai_ip_prefix_t ip) {
  if (ip.addr_family == SAI_IP_ADDR_FAMILY_IPV4) {
    thrift_ip.addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    thrift_ip.addr.ip4 = sai_ip4_t_to_thrift(ip.addr.ip4);
    thrift_ip.mask.ip4 = sai_ip4_t_to_thrift(ip.mask.ip4);
  } else if (ip.addr_family == SAI_IP_ADDR_FAMILY_IPV6) {
    thrift_ip.addr_family = SAI_IP_ADDR_FAMILY_IPV6;
    thrift_ip.addr.ip6 = sai_ip6_t_to_thrift(ip.addr.ip6);
    thrift_ip.mask.ip6 = sai_ip6_t_to_thrift(ip.mask.ip6);
  }
}

void sai_thrift_nat_type_t_parse(const sai_thrift_nat_type_t &thrift_nat_type,
                                 sai_nat_type_t *nat_type) {
  *nat_type = (sai_nat_type_t)thrift_nat_type;
}

void convert_attr_sai_to_thrift(const sai_object_type_t sai_ot,
                                const sai_attribute_t &sai_attr,
                                sai_thrift_attribute_t &thrift_attr) {
  const auto attr_md = sai_metadata_get_attr_metadata(sai_ot, sai_attr.id);
  thrift_attr.id = sai_attr.id;

  if (attr_md == NULL) {
    return;
  }

  switch (attr_md->attrvaluetype) {
    case SAI_ATTR_VALUE_TYPE_BOOL:
      thrift_attr.value.booldata = sai_attr.value.booldata;
      break;
    case SAI_ATTR_VALUE_TYPE_CHARDATA:
      thrift_attr.value.chardata = sai_attr.value.chardata;
      break;
    case SAI_ATTR_VALUE_TYPE_UINT8:
      thrift_attr.value.u8 = sai_attr.value.u8;
      break;
    case SAI_ATTR_VALUE_TYPE_INT8:
      thrift_attr.value.s8 = sai_attr.value.s8;
      break;
    case SAI_ATTR_VALUE_TYPE_UINT16:
      thrift_attr.value.u16 = sai_attr.value.u16;
      break;
    case SAI_ATTR_VALUE_TYPE_INT16:
      thrift_attr.value.s16 = sai_attr.value.s16;
      break;
    case SAI_ATTR_VALUE_TYPE_UINT32:
      thrift_attr.value.u32 = sai_attr.value.u32;
      break;
    case SAI_ATTR_VALUE_TYPE_INT32:
      thrift_attr.value.s32 = sai_attr.value.s32;
      break;
    case SAI_ATTR_VALUE_TYPE_UINT64:
      thrift_attr.value.u64 = sai_attr.value.u64;
      break;
    case SAI_ATTR_VALUE_TYPE_INT64:
      thrift_attr.value.s64 = sai_attr.value.s64;
      break;
    case SAI_ATTR_VALUE_TYPE_POINTER:
      // not supported
      break;
    case SAI_ATTR_VALUE_TYPE_MAC: {
      char mac_str[18];
      snprintf(mac_str,
               sizeof(mac_str),
               "%02x:%02x:%02x:%02x:%02x:%02x",
               sai_attr.value.mac[0],
               sai_attr.value.mac[1],
               sai_attr.value.mac[2],
               sai_attr.value.mac[3],
               sai_attr.value.mac[4],
               sai_attr.value.mac[5]);
      thrift_attr.value.mac = mac_str;
    } break;
    case SAI_ATTR_VALUE_TYPE_IPV4:
      thrift_attr.value.ip4 = sai_ip4_t_to_thrift(sai_attr.value.ip4);
      break;
    case SAI_ATTR_VALUE_TYPE_IPV6:
      thrift_attr.value.ip6 = sai_ip6_t_to_thrift(sai_attr.value.ip6);
      break;
    case SAI_ATTR_VALUE_TYPE_IP_ADDRESS:
      sai_ip_address_t_to_thrift(thrift_attr.value.ipaddr,
                                 sai_attr.value.ipaddr);
      break;
    case SAI_ATTR_VALUE_TYPE_IP_PREFIX:
      sai_ip_prefix_t_to_thrift(thrift_attr.value.ipprefix,
                                sai_attr.value.ipprefix);
      break;
    case SAI_ATTR_VALUE_TYPE_OBJECT_ID:
      thrift_attr.value.oid = sai_attr.value.oid;
      break;
    case SAI_ATTR_VALUE_TYPE_OBJECT_LIST: {
      for (unsigned int i = 0; i < sai_attr.value.objlist.count; i++) {
        thrift_attr.value.objlist.idlist.push_back(
            sai_attr.value.objlist.list[i]);
      }
      thrift_attr.value.objlist.count = sai_attr.value.objlist.count;
      free(sai_attr.value.objlist.list);
    } break;
    case SAI_ATTR_VALUE_TYPE_UINT8_LIST: {
      for (unsigned int i = 0; i < sai_attr.value.u8list.count; i++)
        thrift_attr.value.u8list.uint8list.push_back(
            sai_attr.value.u8list.list[i]);
      thrift_attr.value.u8list.count = sai_attr.value.u8list.count;
      free(sai_attr.value.u8list.list);
    } break;
    case SAI_ATTR_VALUE_TYPE_INT8_LIST: {
      for (unsigned int i = 0; i < sai_attr.value.s8list.count; i++)
        thrift_attr.value.s8list.int8list.push_back(
            sai_attr.value.s8list.list[i]);
      thrift_attr.value.s8list.count = sai_attr.value.s8list.count;
      free(sai_attr.value.s8list.list);
    } break;
    case SAI_ATTR_VALUE_TYPE_UINT16_LIST: {
      for (unsigned int i = 0; i < sai_attr.value.u16list.count; i++)
        thrift_attr.value.u16list.uint16list.push_back(
            sai_attr.value.u16list.list[i]);
      thrift_attr.value.u16list.count = sai_attr.value.u16list.count;
      free(sai_attr.value.u16list.list);
    } break;
    case SAI_ATTR_VALUE_TYPE_INT16_LIST: {
      for (unsigned int i = 0; i < sai_attr.value.s16list.count; i++)
        thrift_attr.value.s16list.int16list.push_back(
            sai_attr.value.s16list.list[i]);
      thrift_attr.value.s16list.count = sai_attr.value.s16list.count;
      free(sai_attr.value.s16list.list);
    } break;
    case SAI_ATTR_VALUE_TYPE_UINT32_LIST: {
      for (unsigned int i = 0; i < sai_attr.value.u32list.count; i++)
        thrift_attr.value.u32list.uint32list.push_back(
            sai_attr.value.u32list.list[i]);
      thrift_attr.value.u32list.count = sai_attr.value.u32list.count;
      free(sai_attr.value.u32list.list);
    } break;
    case SAI_ATTR_VALUE_TYPE_INT32_LIST: {
      for (unsigned int i = 0; i < sai_attr.value.s32list.count; i++)
        thrift_attr.value.s32list.int32list.push_back(
            sai_attr.value.s32list.list[i]);
      thrift_attr.value.s32list.count = sai_attr.value.s32list.count;
      free(sai_attr.value.s32list.list);
    } break;
    case SAI_ATTR_VALUE_TYPE_UINT32_RANGE:
      thrift_attr.value.u32range.min = sai_attr.value.u32range.min;
      thrift_attr.value.u32range.max = sai_attr.value.u32range.max;
      break;
    case SAI_ATTR_VALUE_TYPE_INT32_RANGE:
      thrift_attr.value.s32range.min = sai_attr.value.s32range.min;
      thrift_attr.value.s32range.max = sai_attr.value.s32range.max;
      break;

    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_BOOL:
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8:
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT8:
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT16:
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT16:
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT32:
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_INT32:
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_MAC:
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV4:
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_IPV6:
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_ID:
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_OBJECT_LIST:
    case SAI_ATTR_VALUE_TYPE_ACL_FIELD_DATA_UINT8_LIST:
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_BOOL:
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT8:
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT8:
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT16:
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT16:
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_UINT32:
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_INT32:
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_MAC:
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV4:
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IPV6:
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_IP_ADDRESS:
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_ID:
    case SAI_ATTR_VALUE_TYPE_ACL_ACTION_DATA_OBJECT_LIST:
      //TODO
      break;

    case SAI_ATTR_VALUE_TYPE_ACL_CAPABILITY: {
      for (unsigned int i = 0;
           i < sai_attr.value.aclcapability.action_list.count;
           i++) {
        thrift_attr.value.aclcapability.action_list.int32list.push_back(
            sai_attr.value.aclcapability.action_list.list[i]);
      }
      thrift_attr.value.aclcapability.action_list.count =
          sai_attr.value.aclcapability.action_list.count;
      free(sai_attr.value.aclcapability.action_list.list);
    } break;
    case SAI_ATTR_VALUE_TYPE_ACL_RESOURCE_LIST: {
      for (unsigned int i = 0; i < sai_attr.value.aclresource.count; i++) {
        sai_thrift_acl_resource_t resource = {};
        resource.stage = sai_attr.value.aclresource.list[i].stage;
        resource.bind_point = sai_attr.value.aclresource.list[i].bind_point;
        resource.avail_num = sai_attr.value.aclresource.list[i].avail_num;
        thrift_attr.value.aclresource.resourcelist.push_back(resource);
      }
      thrift_attr.value.aclresource.count = sai_attr.value.aclresource.count;
      free(sai_attr.value.aclresource.list);
    } break;
    case SAI_ATTR_VALUE_TYPE_IP_ADDRESS_LIST: {
      for (unsigned int i = 0; i < sai_attr.value.ipaddrlist.count; i++) {
        sai_thrift_ip_address_t thrift_ip;
        sai_ip_address_t_to_thrift(thrift_ip,
                                   sai_attr.value.ipaddrlist.list[i]);
        thrift_attr.value.ipaddrlist.addresslist.push_back(thrift_ip);
      }
      thrift_attr.value.ipaddrlist.count = sai_attr.value.ipaddrlist.count;
      free(sai_attr.value.ipaddrlist.list);
    } break;
    case SAI_ATTR_VALUE_TYPE_MAP_LIST:
    case SAI_ATTR_VALUE_TYPE_VLAN_LIST:
    case SAI_ATTR_VALUE_TYPE_QOS_MAP_LIST: {
      for (unsigned int i = 0; i < sai_attr.value.qosmap.count; i++) {
        sai_thrift_qos_map_t thrift_qos_map;
        // key
        thrift_qos_map.key.tc = sai_attr.value.qosmap.list[i].key.tc;
        thrift_qos_map.key.dscp = sai_attr.value.qosmap.list[i].key.dscp;
        thrift_qos_map.key.dot1p = sai_attr.value.qosmap.list[i].key.dot1p;
        thrift_qos_map.key.prio = sai_attr.value.qosmap.list[i].key.prio;
        thrift_qos_map.key.pg = sai_attr.value.qosmap.list[i].key.pg;
        thrift_qos_map.key.queue_index =
            sai_attr.value.qosmap.list[i].key.queue_index;
        thrift_qos_map.key.color =
            static_cast<int32_t>(sai_attr.value.qosmap.list[i].key.color);
        thrift_qos_map.key.mpls_exp =
            sai_attr.value.qosmap.list[i].key.mpls_exp;
        // value
        thrift_qos_map.value.tc = sai_attr.value.qosmap.list[i].value.tc;
        thrift_qos_map.value.dscp = sai_attr.value.qosmap.list[i].value.dscp;
        thrift_qos_map.value.dot1p = sai_attr.value.qosmap.list[i].value.dot1p;
        thrift_qos_map.value.prio = sai_attr.value.qosmap.list[i].value.prio;
        thrift_qos_map.value.pg = sai_attr.value.qosmap.list[i].value.pg;
        thrift_qos_map.value.queue_index =
            sai_attr.value.qosmap.list[i].value.queue_index;
        thrift_qos_map.value.color =
            static_cast<int32_t>(sai_attr.value.qosmap.list[i].value.color);
        thrift_qos_map.value.mpls_exp =
            sai_attr.value.qosmap.list[i].value.mpls_exp;
        thrift_attr.value.qosmap.maplist.push_back(thrift_qos_map);
      }
      thrift_attr.value.qosmap.count = sai_attr.value.qosmap.count;
      free(sai_attr.value.qosmap.list);
    } break;
    case SAI_ATTR_VALUE_TYPE_TLV_LIST:
    case SAI_ATTR_VALUE_TYPE_SEGMENT_LIST:
    case SAI_ATTR_VALUE_TYPE_PORT_EYE_VALUES_LIST:
    case SAI_ATTR_VALUE_TYPE_TIMESPEC:
    case SAI_ATTR_VALUE_TYPE_NAT_ENTRY_DATA:
      // not supported
      break;
    default:
      break;
  }
}

// very hacky but this means we never have to modify the generated file
#include "sai_rpc_server.cpp"

class sai_rpcHandlerFrontend : virtual public sai_rpcHandler {
  int64_t sai_thrift_object_type_get_availability(
      const sai_thrift_object_type_t object_type,
      const sai_thrift_attr_id_t attr_id,
      const int32_t attr_type) {
    sai_attribute_t attr = {};
    attr.id = attr_id;
    attr.value.s32 = attr_type;
    uint64_t count = 0;

    sai_object_type_get_availability(
        switch_id, (sai_object_type_t)object_type, 1, &attr, &count);
    return count;
  }

  void sai_thrift_query_attribute_enum_values_capability(
      std::vector<int32_t> &thrift_enum_caps,
      const sai_thrift_object_type_t object_type,
      const sai_thrift_attr_id_t attr_id,
      const int32_t caps_count) {
    sai_status_t status = SAI_STATUS_SUCCESS;
    sai_s32_list_t enum_values_capability;
    int32_t *caps_list = NULL;

    if (!caps_count) {
      return;
    }
    caps_list = (int32_t *)malloc(sizeof(int32_t) * caps_count);
    if (!caps_list) {
      return;
    }
    enum_values_capability.list = caps_list;
    enum_values_capability.count = caps_count;
    status = sai_query_attribute_enum_values_capability(
        (sai_object_id_t)switch_id,
        (sai_object_type_t)object_type,
        (sai_attr_id_t)attr_id,
        &enum_values_capability);
    if (SAI_STATUS_SUCCESS != status) {
      free(caps_list);
      return;
    }

    for (uint32_t i = 0; i < enum_values_capability.count; ++i) {
      thrift_enum_caps.push_back(enum_values_capability.list[i]);
    }
    free(caps_list);
  }
};

static pthread_mutex_t cookie_mutex;
static pthread_cond_t cookie_cv;
static void *cookie;

static void *sai_thrift_rpc_server_thread(void *arg) {
  int port = *(int *)arg;
  std::shared_ptr<sai_rpcHandlerFrontend> handler(new sai_rpcHandlerFrontend());
  std::shared_ptr<TProcessor> processor(new sai_rpcProcessor(handler));
  std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  std::shared_ptr<TTransportFactory> transportFactory(
      new TBufferedTransportFactory());
  std::shared_ptr<TProtocolFactory> protocolFactory(
      new TBinaryProtocolFactory());

  TSimpleServer server(
      processor, serverTransport, transportFactory, protocolFactory);
  pthread_mutex_lock(&cookie_mutex);
  cookie = (void *)processor.get();
  pthread_cond_signal(&cookie_cv);
  pthread_mutex_unlock(&cookie_mutex);
  server.serve();
  return 0;
}

static pthread_t sai_thrift_rpc_thread;

extern "C" {
int start_p4_sai_thrift_rpc_server(char *port) {
  static int *param = (int *)malloc(sizeof(int));
  *param = atoi(port);
  std::cerr << "Starting SAI RPC server on port " << port << std::endl;

  cookie = NULL;
  int status = pthread_create(
      &sai_thrift_rpc_thread, NULL, sai_thrift_rpc_server_thread, param);
  if (status) return status;
  pthread_mutex_lock(&cookie_mutex);
  while (!cookie) {
    pthread_cond_wait(&cookie_cv, &cookie_mutex);
  }
  pthread_mutex_unlock(&cookie_mutex);
  pthread_mutex_destroy(&cookie_mutex);
  pthread_cond_destroy(&cookie_cv);
  return status;
}

int stop_p4_sai_thrift_rpc_server(void) {
  int status = pthread_cancel(sai_thrift_rpc_thread);
  if (status == 0) {
    int s = pthread_join(sai_thrift_rpc_thread, NULL);
    if (s) return s;
  }
  return status;
}
}
