#!/bin/bash

echo "copy file for saithriftv2"

if test -n "$(find ../ -name 'python-saithrift_*.deb' -print -quit)"; then  \
    for file in ../python-saithrift_*.deb ; do \
        if test file; then \
            echo copy file: $file; \
            cp $file ${file//python-saithrift_/python-saithriftv2_}; \
        fi; \
    done; \
fi

if test -n "$(find ../ -name 'libsaithrift-dev_*.deb' -print -quit)"; then  \
    for file in ../libsaithrift-dev_*.deb ; do \
        if test file; then \
            echo copy file: $file; \
            cp $file ${file//libsaithrift-dev_/libsaithriftv2-dev_}; \
        fi; \
    done; \
fi

if test -n "$(find ../ -name 'saiserver*.deb' -print -quit)"; then  \
    for file in ../saiserver*.deb ; do \
        if test file; then \
            echo copy file: $file; \
            cp $file ${file//saiserver/saiserverv2}; \
        fi; \
    done; \
fi
