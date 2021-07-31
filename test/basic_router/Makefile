#	 Copyright (c) 2015 Microsoft Open Technologies, Inc.
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

##########################################################
# SAI headers

#Broadcom, Mellonox, Barefoot

CXX = $(CROSS_COMPILE)g++

LIBS = -lpthread -lsai
SAI_IDIR = /usr/include/sai

ODIR = ../obj
LDIR = ../lib
BDIR = ../bin
IDIR = .
all: $(BDIR)/basic_router

GTEST_DIR = ../gtest-1.7.0
CXXFLAGS += -g -Wall -Wextra -pthread -I./  -std=c++11
GTEST_FLAGS = -isystem $(GTEST_DIR)/include
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
	$(GTEST_DIR)/include/gtest/internal/*.h

_DEPS = sai.h saiacl.h sailag.h sainexthop.h  sairouter.h saistp.h\
	saifdb.h saimirror.h saiport.h sairouterintf.h saiswitch.h \
	saineighbor.h saiqos.h saisamplepacket.h saitypes.h saihostintf.h\
	sainexthopgroup.h sairoute.h saistatus.h saivlan.h\
DEPS = $(patsubst %, $(SAI_IDIR)/%, $(_DEPS))

#basic_router
_BRDEPS = log.h ip.h mac.h neighbor_mgr.h route_mgr.h basic_router.h\
	fdb_mgr.h nexthop_mgr.h nexthopgrp_mgr.h
BRDEPS = $(patsubst %,$(IDIR)/%,$(_BRDEPS))

_BROBJ = ip.o log.o mac.o fdb_mgr.o nexthop_mgr.o nexthopgrp_mgr.o\
	neighbor_mgr.o route_mgr.o
BROBJ = $(patsubst %,$(ODIR)/%,$(_BROBJ))


$(ODIR)/%.o : $(IDIR)/%.cpp 
	$(CXX) -c $^ -o $@ $(CXXFLAGS) -I$(SAI_IDIR) 

$(ODIR)/basic_router.o : $(IDIR)/basic_router.cpp \
	$(GTEST_HEADERS) $(DEPS) 
	$(CXX) $(GTEST_FLAGS) $(CXXFLAGS) -I $(SAI_IDIR) -I $(GTEST_DIR)/include/gtest/ \
	-I $(GTEST_DIR)/include/gtest/internal/ -c $(IDIR)/basic_router.cpp -o $@ 

$(BDIR)/basic_router: $(ODIR)/basic_router.o $(LDIR)/gtest_main.a $(BROBJ)
	$(CXX) $(CXXFLAGS)  $^ -o $@ $(LIBS) 

