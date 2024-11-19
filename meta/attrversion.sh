#!/bin/bash
#
# Copyright (c) 2021 Microsoft Open Technologies, Inc.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
#    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
#
#    See the Apache Version 2.0 License for specific language governing
#    permissions and limitations under the License.
#
#    Microsoft would like to thank the following companies for their review and
#    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
#    Dell Products, L.P., Facebook, Inc., Marvell International Ltd.
#
# @file    attrversion.sh
#
# @brief   This module defines attr version script
#

OUTPUT="saiattrversion.h"

if ! git rev-parse --git-dir > /dev/null 2>&1; then

echo "WARNING: this is not git repository, will skip generating saiattrversion.h"
cat /dev/null > $OUTPUT
exit
fi

# since api sai_query_api_version was introduced at v1.10.0 and we are enum
# backward compatible from v1.9.0 then it make sense to list all attributes
# version at v1.10.0, since it's min version to query vendor for api version

BASE="v1.10.0"

set -e

TAGS=$(git tag --sort=v:refname | grep -P "^v\d+\.\d+.\d+$" | sed -n -e '/'$BASE'/,$p'; echo HEAD)

(for tag in $TAGS; do git grep -P "^\s+SAI_\w+_ATTR_" $tag ../inc ../experimental | cat; done;
 grep -P "^\s+SAI_\w+_ATTR_" ../inc/sai*h ../experimental/sai*h | perl -npe '$_.="HEAD:"' ) | \
        perl -ne '/^(\S+):..\/(\S+)\/\S+.h:\s+(SAI_\w+_ATTR_\w+)/;
        print "#define SAI_METADATA_ATTR_VERSION_$3 \"$1\" /* $2 */\n" if not defined $h{$3};$h{$3}=1' > $OUTPUT
