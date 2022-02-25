/*
Copyright 2013-present Barefoot Networks, Inc.
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

#include "sai_bm_c_api.h"

// maps from SAI types to switchapi types

char *sai_status_to_string(_In_ const sai_status_t status) {
  switch (status) {
    case SAI_STATUS_INVALID_PARAMETER:
      return "invalid parameter";
    case SAI_STATUS_NO_MEMORY:
      return "no memory";
    case SAI_STATUS_FAILURE:
      return "unknown failure";
    default:
      return "unknown failure";
  }
}

char *sai_object_type_to_string(_In_ sai_object_type_t object_type) {
  if (object_type > SAI_OBJECT_TYPE_MAX) {
    return "invalid object";
  }

  switch (object_type) {
    case SAI_OBJECT_TYPE_NULL:
      return "null object";
    case SAI_OBJECT_TYPE_PORT:
      return "port object";
    case SAI_OBJECT_TYPE_LAG:
      return "lag object";
    case SAI_OBJECT_TYPE_VIRTUAL_ROUTER:
      return "virtual router object";
    case SAI_OBJECT_TYPE_NEXT_HOP:
      return "nexthop object";
    case SAI_OBJECT_TYPE_NEXT_HOP_GROUP:
      return "nexthop group object";
    case SAI_OBJECT_TYPE_ROUTER_INTERFACE:
      return "router interface object";
    case SAI_OBJECT_TYPE_ACL_TABLE:
      return "acl table object";
    case SAI_OBJECT_TYPE_ACL_ENTRY:
      return "acl entry object";
    case SAI_OBJECT_TYPE_ACL_COUNTER:
      return "acl counter object";
    case SAI_OBJECT_TYPE_HOSTIF:
      return "host interface object";
    case SAI_OBJECT_TYPE_MIRROR_SESSION:
      return "mirror object";
    case SAI_OBJECT_TYPE_SAMPLEPACKET:
      return "sample packet object";
    case SAI_OBJECT_TYPE_STP:
      return "stp instance object";
    case SAI_OBJECT_TYPE_HOSTIF_TRAP_GROUP:
      return "trap group object";
    case SAI_OBJECT_TYPE_ACL_TABLE_GROUP:
      return "acl table group object";
    case SAI_OBJECT_TYPE_POLICER:
      return "policer object";
    case SAI_OBJECT_TYPE_WRED:
      return "wred object";
    case SAI_OBJECT_TYPE_QOS_MAP:
      return "qos maps object";
    case SAI_OBJECT_TYPE_QUEUE:
      return "queue object";
    case SAI_OBJECT_TYPE_SCHEDULER:
      return "scheduler object";
    case SAI_OBJECT_TYPE_SCHEDULER_GROUP:
      return "scheduler group object";
    default:
      return "invalid object";
  }
}

sai_status_t sai_ipv4_prefix_length(_In_ sai_ip4_t ip4,
                                    _Out_ uint32_t *prefix_length) {
  int x = 0;
  *prefix_length = 0;
  while (ip4) {
    x = ip4 & 0x1;
    if (x) (*prefix_length)++;
    ip4 = ip4 >> 1;
  }
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_ipv6_prefix_length(_In_ const sai_ip6_t ip6,
                                    _Out_ uint32_t *prefix_length) {
  int i = 0, x = 0;
  sai_ip6_t ip6_temp;
  memcpy(ip6_temp, ip6, 16);
  *prefix_length = 0;
  for (i = 0; i < 16; i++) {
    if (ip6_temp[i] == 0xFF) {
      *prefix_length += 8;
    } else {
      while (ip6_temp[i]) {
        x = ip6_temp[i] & 0x1;
        if (x) (*prefix_length)++;
        ip6_temp[i] = ip6_temp[i] >> 1;
      }
    }
  }
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_ip_prefix_to_switch_ip_addr(
    const _In_ sai_ip_prefix_t *sai_ip_addr, _Out_ switch_ip_addr_t *ip_addr) {
  if (sai_ip_addr->addr_family == SAI_IP_ADDR_FAMILY_IPV4) {
    ip_addr->type = SWITCH_API_IP_ADDR_V4;
    ip_addr->ip.v4addr = ntohl(sai_ip_addr->addr.ip4);
    sai_ipv4_prefix_length(ntohl(sai_ip_addr->mask.ip4), &ip_addr->prefix_len);
  } else if (sai_ip_addr->addr_family == SAI_IP_ADDR_FAMILY_IPV6) {
    ip_addr->type = SWITCH_API_IP_ADDR_V6;
    memcpy(ip_addr->ip.v6addr, sai_ip_addr->addr.ip6, 16);
    sai_ipv6_prefix_length(sai_ip_addr->mask.ip6, &ip_addr->prefix_len);
  }
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_ip_addr_to_switch_ip_addr(
    const _In_ sai_ip_address_t *sai_ip_addr, _Out_ switch_ip_addr_t *ip_addr) {
  if (sai_ip_addr->addr_family == SAI_IP_ADDR_FAMILY_IPV4) {
    ip_addr->type = SWITCH_API_IP_ADDR_V4;
    ip_addr->ip.v4addr = ntohl(sai_ip_addr->addr.ip4);
    ip_addr->prefix_len = 32;
  } else if (sai_ip_addr->addr_family == SAI_IP_ADDR_FAMILY_IPV6) {
    ip_addr->type = SWITCH_API_IP_ADDR_V6;
    memcpy(ip_addr->ip.v6addr, sai_ip_addr->addr.ip6, 16);
    ip_addr->prefix_len = 128;
  }
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_ipv4_to_string(_In_ sai_ip4_t ip4,
                                _In_ uint32_t max_length,
                                _Out_ char *entry_string,
                                _Out_ int *entry_length) {
  inet_ntop(AF_INET, &ip4, entry_string, max_length);
  *entry_length = (int)strlen(entry_string);
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_ipv6_to_string(_In_ sai_ip6_t ip6,
                                _In_ uint32_t max_length,
                                _Out_ char *entry_string,
                                _Out_ int *entry_length) {
  inet_ntop(AF_INET6, &ip6, entry_string, max_length);
  *entry_length = (int)strlen(entry_string);
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_ipaddress_to_string(_In_ sai_ip_address_t ip_addr,
                                     _In_ uint32_t max_length,
                                     _Out_ char *entry_string,
                                     _Out_ int *entry_length) {
  if (ip_addr.addr_family == SAI_IP_ADDR_FAMILY_IPV4) {
    sai_ipv4_to_string(
        ip_addr.addr.ip4, max_length, entry_string, entry_length);
  } else if (ip_addr.addr_family == SAI_IP_ADDR_FAMILY_IPV6) {
    sai_ipv6_to_string(
        ip_addr.addr.ip6, max_length, entry_string, entry_length);
  } else {
    snprintf(entry_string,
             max_length,
             "Invalid addr family %d",
             ip_addr.addr_family);
    return SAI_STATUS_INVALID_PARAMETER;
  }
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_ipprefix_to_string(_In_ sai_ip_prefix_t ip_prefix,
                                    _In_ uint32_t max_length,
                                    _Out_ char *entry_string,
                                    _Out_ int *entry_length) {
  int len = 0;
  uint32_t pos = 0;

  if (ip_prefix.addr_family == SAI_IP_ADDR_FAMILY_IPV4) {
    sai_ipv4_to_string(ip_prefix.addr.ip4, max_length, entry_string, &len);
    pos += len;
    if (pos > max_length) {
      *entry_length = max_length;
      return SAI_STATUS_SUCCESS;
    }
    pos += snprintf(entry_string + pos, max_length - pos, "/");
    if (pos > max_length) {
      *entry_length = max_length;
      return SAI_STATUS_SUCCESS;
    }
    sai_ipv4_to_string(
        ip_prefix.mask.ip4, max_length - pos, entry_string + pos, &len);
    pos += len;
    if (pos > max_length) {
      *entry_length = max_length;
      return SAI_STATUS_SUCCESS;
    }
  } else if (ip_prefix.addr_family == SAI_IP_ADDR_FAMILY_IPV6) {
    sai_ipv6_to_string(ip_prefix.addr.ip6, max_length, entry_string, &len);
    pos += len;
    if (pos > max_length) {
      *entry_length = max_length;
      return SAI_STATUS_SUCCESS;
    }
    pos += snprintf(entry_string + pos, max_length - pos, "/");
    if (pos > max_length) {
      *entry_length = max_length;
      return SAI_STATUS_SUCCESS;
    }
    sai_ipv6_to_string(
        ip_prefix.mask.ip6, max_length - pos, entry_string + pos, &len);
    pos += len;
    if (pos > max_length) {
      *entry_length = max_length;
      return SAI_STATUS_SUCCESS;
    }
  } else {
    snprintf(entry_string,
             max_length,
             "Invalid addr family %d",
             ip_prefix.addr_family);
    return SAI_STATUS_INVALID_PARAMETER;
  }

  *entry_length = pos;
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_port_speed_to_switch_port_speed(
    uint32_t sai_port_speed, _Out_ switch_port_speed_t *switch_port_speed) {
  // speeds are in mbps
  switch (sai_port_speed) {
    case 1000:
      *switch_port_speed = SWITCH_API_PORT_SPEED_1G;
      break;
    case 10000:
      *switch_port_speed = SWITCH_API_PORT_SPEED_10G;
      break;
    case 25000:
      *switch_port_speed = SWITCH_API_PORT_SPEED_25G;
      break;
    case 40000:
      *switch_port_speed = SWITCH_API_PORT_SPEED_40G;
      break;
    case 50000:
      *switch_port_speed = SWITCH_API_PORT_SPEED_50G;
      break;
    case 100000:
      *switch_port_speed = SWITCH_API_PORT_SPEED_100G;
      break;
    default:
      return SAI_STATUS_INVALID_PARAMETER;
  }

  return SAI_STATUS_SUCCESS;
}

switch_acl_action_t sai_packet_action_to_switch_packet_action(
    _In_ sai_packet_action_t action) {
  switch (action) {
    case SAI_PACKET_ACTION_DROP:
      return SWITCH_ACL_ACTION_DROP;
    case SAI_PACKET_ACTION_FORWARD:
      return SWITCH_ACL_ACTION_PERMIT;
    case SAI_PACKET_ACTION_TRAP:
      return SWITCH_ACL_ACTION_REDIRECT_TO_CPU;
    case SAI_PACKET_ACTION_LOG:
      return SWITCH_ACL_ACTION_LOG;
    default:
      return SWITCH_ACL_ACTION_NOP;
  }
}

// maps from switchapi types to SAI types

sai_status_t sai_switch_ip_addr_to_sai_ip_addr(
    _Out_ sai_ip_address_t *sai_ip_addr, const _In_ switch_ip_addr_t *ip_addr) {
  if (ip_addr->type == SWITCH_API_IP_ADDR_V4) {
    sai_ip_addr->addr_family = SAI_IP_ADDR_FAMILY_IPV4;
    sai_ip_addr->addr.ip4 = htonl(ip_addr->ip.v4addr);
  } else if (ip_addr->type == SWITCH_API_IP_ADDR_V6) {
    sai_ip_addr->addr_family = SAI_IP_ADDR_FAMILY_IPV6;
    memcpy(sai_ip_addr->addr.ip6, ip_addr->ip.v6addr, 16);
  }
  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_switch_port_enabled_to_sai_oper_status(
    _In_ _Out_ sai_attribute_t *attr) {
  switch ((int)attr->value.booldata) {
    case 1:
      attr->value.u8 = SAI_PORT_OPER_STATUS_UP;
      break;
    case 0:
      attr->value.u8 = SAI_PORT_OPER_STATUS_DOWN;
      break;
  }

  return SAI_STATUS_SUCCESS;
}

sai_status_t sai_switch_status_to_sai_status(_In_ const switch_status_t
                                                 status) {
  switch (status) {
    case SWITCH_STATUS_SUCCESS:
      return SAI_STATUS_SUCCESS;
    case SWITCH_STATUS_FAILURE:
      return SWITCH_STATUS_FAILURE;
    case SWITCH_STATUS_INVALID_PARAMETER:
      return SAI_STATUS_INVALID_PARAMETER;
    case SWITCH_STATUS_NO_MEMORY:
      return SAI_STATUS_NO_MEMORY;
    default:
      return SAI_STATUS_FAILURE;
  }
}