#!/bin/bash
#
# Copyright (c) 2023 Microsoft Open Technologies, Inc.
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
# @file    size.sh
#
# @brief   This module generates saimetadatasize.h with struct/union sizes
#

set -e

if ! git rev-parse --git-dir > /dev/null 2>&1; then

    echo "WARNING: this is not git repository, will skip generating saimetadatasize.h and skip struct size check"
    touch saimetadatasize.h
    exit
fi

TEMP_DIR="tmp"

COMMIT=origin/master # should be corresponding branch HEAD

rm -rf $TEMP_DIR

mkdir $TEMP_DIR

echo "git checkout dir inc and experimental on commit: $COMMIT"

git --work-tree=$TEMP_DIR/ checkout $COMMIT inc experimental 2>/dev/null

perl size.pl $TEMP_DIR

rm -rf $TEMP_DIR
