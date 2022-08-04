#        Copyright (c) 2015 Microsoft Open Technologies, Inc.
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
#    FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
#
#    See the Apache Version 2.0 License for specific language governing
#    permissions and limitations under the License.
#
#    Microsoft would like to thank the following companies for their review and
#    assistance with these files: Intel Corporation, Mellanox Technologies Ltd,
#    Dell Products, L.P., Facebook, Inc
#

# There are two versions of saithrift, in order not to break the origin tests, use ENVIRONMENT to build them respectively.
# By default, it will make the saiserver and the related artifacts under folder test/saithrift.
# For example, build saithrift v2
# - export  SAITHRIFTV2=y
# - make saithrift-build

ifdef SAITHRIFTV2
SAITHRIFT_PATH=test/saithriftv2
else
SAITHRIFT_PATH=test/saithrift
endif

# Passed to genrpc.pl via "make saithrift-build":
GEN_SAIRPC_OPTS?=

# Passed to meta/Makefile via "make saithrift-build, can specify add'l libraries along with libsai
SAIRPC_EXTRA_LIBS?=

.PHONY: test doc clean

doc:
	doxygen Doxyfile

test:
	make -C test

saithrift-build:
	SAIRPC_EXTRA_LIBS="$(SAIRPC_EXTRA_LIBS)" GEN_SAIRPC_OPTS=$(GEN_SAIRPC_OPTS) make -C $(SAITHRIFT_PATH)

saithrift-install: saithrift-build
	make -C $(SAITHRIFT_PATH) install

clean:
	make -C test clean    
