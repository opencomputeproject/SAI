#!/bin/bash


# to list git ancestry all comitts (even if there is a tree not single line)
# this can bu usefull to build histroy of enums from root (enum lock) to the current
# origin/master and current commit - and it will be possible to fix mistakes

#git log --graph --oneline --ancestry-path c388490^..0b90765 | cat
#git rev-list --ancestry-path  c388490^..0b90765


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
# working throug 25 commits takes about 0.4 seconds + parsing
# so it seems like not a hudge time to make sure all commits are safe
# and even if we get at some point that this will be "too slow", having all
# history, we can sometimes produce "known" history with enum values and keep
# that file as a reference and load it at begin, and start checking commits from
# one of the future commits, basicially reducing processing time to zero

# just for sanity we can also keep headers check to 1 commit back and
# alse maybe we can add one gcc check current to history

set -e

mkdir -p temp/inc

git rev-list --ancestry-path  65f04ab^..origin/master | head -n 2 | tac | while read commit;
do
    rm -f temp/inc/*

    echo working on commit $commit

    git --work-tree=temp/ checkout $commit inc

    time ./naive.pl
done


