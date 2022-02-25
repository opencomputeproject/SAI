/*
 *  Copyright (C) 2017. Mellanox Technologies, Ltd. ALL RIGHTS RESERVED.
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

#ifndef _SAI_KEYS_
#define _SAI_KEYS_

typedef bit<64> sai_object_id_t;
typedef bit<32> sai_ip4_t;

@name("standard_metadata")
struct standard_metadata_t {
}

@name("in_port_metadata")
struct in_port_metadata_t {
		sai_object_id_t ingress_port;
}

@name("out_port_metadata")
struct out_port_metadata_t {
		sai_object_id_t egress_port;
}

@name("in_rif_metadata")
struct in_rif_metadata_t {
		sai_object_id_t ingress_rif;
}

@name("out_rif_metadata")
struct out_rif_metadata_t {
		sai_object_id_t egress_rif;
}

#endif
