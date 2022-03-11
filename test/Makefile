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

CXX = $(CROSS_COMPILE)g++

LIBS = -lpthread 
LDIR = ./lib

MKDIR_P = mkdir -p 
USER_ODIR = obj
USER_BDIR = bin
OUT_DIRS = $(USER_BDIR) $(USER_ODIR) $(LDIR)
TESTS = $(USER_BDIR)/basic_router 

###########################################################
#GTEST SECTIONS COMMON

GTEST_DIR = ./gtest-1.7.0
GTEST_FLAGS = -isystem $(GTEST_DIR)/include
CXXFLAGS += -g -Wall -Wextra -pthread -I./  -std=c++11
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
				$(GTEST_DIR)/include/gtest/internal/*.h


all : directories $(LDIR)/gtest_main.a $(TESTS) sai_ut

.PHONY: directories clean $(TESTS) sai_ut

###########################################################

directories: 
	$(MKDIR_P) $(OUT_DIRS)

$(USER_BDIR)/basic_router:
	make -C basic_router

sai_ut:
	make -C sai_ut

clean :
	rm -f $(TESTS) $(USER_ODIR)/gtest.a $(USER_ODIR)/gtest_main.a $(USER_ODIR)/*.o
	make -C sai_ut clean
	make -C saithrift clean
	make -C saithriftv2 clean


###########################################################
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

$(USER_ODIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(GTEST_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
	$(GTEST_DIR)/src/gtest-all.cc -o $@

$(USER_ODIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(GTEST_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
	$(GTEST_DIR)/src/gtest_main.cc -o $@

$(LDIR)/gtest.a : $(USER_ODIR)/gtest-all.o
	$(AR) $(ARFLAGS) -o $@ $^ 

$(LDIR)/gtest_main.a : $(USER_ODIR)/gtest-all.o $(USER_ODIR)/gtest_main.o
	$(AR) $(ARFLAGS) -o $@ $^
