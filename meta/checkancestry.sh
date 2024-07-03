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
# @file    checkancestry.sh
#
# @brief   This module defines ancestry script
#


# To list git ancestry all comitts (even if there is a tree not single line)
# this can be usefull to build histroy of enums from root (enum lock) to the
# current origin/master and current commit - and it will be possible to fix
# mistakes.

# examples below are to show how to get correct git history tree
# git log --graph --oneline --ancestry-path c388490^..0b90765 | cat
# git rev-list --ancestry-path  c388490^..0b90765

# If we will have our base commit, we will assume that each previous commit
# followed metadata check, and then we can use naive approach for parsing enum
# values instead of doing gcc compile whch can take long time. With this
# approach we should be able to build entire history from base commit throug
# all commits up to the current PR. This will sure that there will be no
# abnormalities if some enums will be removed and then added again with
# different value. This will also help to track the issue if two PRs will pass
# validation but after they will be merged they could potentially cause enum
# value issue and this approach will catch that.
#
# Working throug 25 commits takes about 0.4 seconds + parsing so it seems like
# not a hudge time to make sure all commits are safe and even if we get at some
# point that this will be "too slow", having all history, we can sometimes
# produce "known" history with enum values and keep that file as a reference
# and load it at begin, and start checking commits from one of the future
# commits, basicially reducing processing time to zero.

# Just for sanity we can also keep headers check to 1 commit back and alse
# maybe we can add one gcc check current to history,

set -e

if ! git rev-parse --git-dir > /dev/null 2>&1; then

    echo "WARNING: this is not git repository, will skip ancestry check"
    exit
fi

# 1. get all necessary data to temp directory for future processing
# 2. pass all interesting commits to processor to build history

function clean_temp_dir()
{
    rm -rf temp
}

function create_temp_dir()
{
    mkdir temp
}

function checkout_inc_directories()
{
    echo "git checkout work tree commits:" $LIST

    for commit in $LIST
    do
        #echo working on commit $commit

        mkdir temp/commit-$commit
        mkdir temp/commit-$commit/inc

        git --work-tree=temp/commit-$commit checkout $commit inc 2>/dev/null

    done
}

function create_commit_list()
{
    local begin=$1
    local end=$2

    echo "ancestry graph"

    git --no-pager log --graph --oneline --ancestry-path  origin/master^..HEAD

    echo "git rev list from $begin to $end"

    LIST=$(git rev-list --ancestry-path ${begin}^..${end} | xargs -n 1 git rev-parse --short | tac)
}

function check_enum_history()
{
    perl ancestry.pl -H "ancestry.825c835.history" $LIST
}

#
# MAIN
#

# BEGIN_COMMIT is the commit from we check each commit history for backward compatibility

# since checking ancestry history is taking longer time each commit, we will
# use history file to load all the history from previous processed commits, in
# this way we just load entire history to perl directly and save time on
# processing all those previous commits, this process can be repeated later on if
# the processing time will increase too much

BEGIN_COMMIT=3132018 # from this commit we are backward compatible
BEGIN_COMMIT=825c835 # to this commit we have history file
END_COMMIT=HEAD

clean_temp_dir
create_temp_dir
create_commit_list $BEGIN_COMMIT $END_COMMIT
checkout_inc_directories
check_enum_history
clean_temp_dir
