#!/bin/bash
#
# Copyright (c) 2014 Microsoft Open Technologies, Inc.
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
# @file    checkenumlock.sh
#
# @brief   This module defines SAI enum values integration check for 2 header directories
#

set -e

rm -rf temp

mkdir temp

git --work-tree=temp/ checkout origin/master inc
git --work-tree=temp/ checkout origin/master experimental

echo "Checking for possible enum values shift (current branch vs origin/master) ..."

./checkheaders.pl -s ../inc/ temp/inc/

rm -rf temp
# clean up the git changes as well
# workaround fix for git --work-tree=temp/ checkout ...
# after checkout from other branch, data will be left in git
git stash
