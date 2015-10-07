#  Makefile for SAI unit-tests
#  add gtest_dir and platform-specific directories
#  important - link to your platform's SAI library
#  add new unit-tests to SRCS
#  run make {all|clean} 
#  run the resultant binary from EXEC, along with SAI library
#  

# Pointer to the root of Google Test
# download from https://code.google.com/p/googletest/downloads/list
GTEST_DIR = ../gtest-1.7.0

# provide location to SAI include files
SAI_INCLUDE_FLAGS = -I./ -I../../inc

# location for gtest lib target and SAI ut bins
LDIR = ../lib
BDIR = ../bin

# add unit-test sources here 
l3_util_SRCS = ./routing/sai_l3_unit_test_utils.cpp
vr_SRCS = $(l3_util_SRCS) ./routing/sai_l3_vr_unit_test.cpp
rif_SRCS = $(l3_util_SRCS) ./routing/sai_l3_rif_unit_test.cpp
nh_SRCS = $(l3_util_SRCS) ./routing/sai_l3_nexthop_unit_test.cpp
nhg_SRCS = $(l3_util_SRCS) ./routing/sai_l3_nexthopgroup_unit_test.cpp
nbr_SRCS = $(l3_util_SRCS) ./routing/sai_l3_neighbor_unit_test.cpp
route_SRCS = $(l3_util_SRCS) ./routing/sai_l3_route_unit_test.cpp

acl_util_SRCS = ./acl/sai_acl_unit_test_utils.cpp 
counter_SRCS = $(acl_util_SRCS) ./acl/sai_acl_counter_unit_test.cpp
rule_SRCS = $(acl_util_SRCS) $(l3_util_SRCS) ./acl/sai_acl_rule_unit_test.cpp
table_SRCS = $(acl_util_SRCS) ./acl/sai_acl_table_unit_test.cpp

### platform specific Linker/LD Flags
# add pointers to SAI library
# by adding -l<sai> and -L<location-to-libsai.so> directives
# add other linker dependencies too. alternately, dlopen/dlsym
# in unit-test source. For e.g:
#PLATFORM_LINK_LDFLAGS = -lsai -L../lib -Wl,-rpath-link,../lib
PLATFORM_LINK_LDFLAGS =
###########

# binary for all unit-tests
vr_EXEC    = sai_ut_vr
rif_EXEC   = sai_ut_rif
nh_EXEC    = sai_ut_nh
nhg_EXEC   = sai_ut_nhg
nbr_EXEC   = sai_ut_nbr
route_EXEC = sai_ut_route
counter_EXEC = sai_ut_counter
rule_EXEC 	 = sai_ut_rule
table_EXEC 	 = sai_ut_table

EXEC_ALL = $(BDIR)/$(vr_EXEC) $(BDIR)/$(rif_EXEC) $(BDIR)/$(nh_EXEC) $(BDIR)/$(nhg_EXEC) $(BDIR)/$(nbr_EXEC) $(BDIR)/$(route_EXEC) \
		   $(BDIR)/$(counter_EXEC) $(BDIR)/$(rule_EXEC) $(BDIR)/$(table_EXEC)

# what to use for compiling
CXX=g++
AR=ar

# include dir for unit-tests
INCLUDEFLAGS = ${SAI_INCLUDE_FLAGS} -I$(GTEST_DIR)/include 

# libraries to be included for unit-tests
LDFLAGS = ${PLATFORM_LINK_LDFLAGS} -lpthread

# rule for objs
vr_OBJS = $(vr_SRCS:%.cpp=%.o) $(LDIR)/gtest_main.a
rif_OBJS = $(rif_SRCS:%.cpp=%.o) $(LDIR)/gtest_main.a
nh_OBJS = $(nh_SRCS:%.cpp=%.o) $(LDIR)/gtest_main.a
nhg_OBJS = $(nhg_SRCS:%.cpp=%.o) $(LDIR)/gtest_main.a
nbr_OBJS = $(nbr_SRCS:%.cpp=%.o) $(LDIR)/gtest_main.a
route_OBJS = $(route_SRCS:%.cpp=%.o) $(LDIR)/gtest_main.a

counter_OBJS = $(counter_SRCS:%.cpp=%.o) $(LDIR)/gtest_main.a
rule_OBJS = $(rule_SRCS:%.cpp=%.o) $(LDIR)/gtest_main.a
table_OBJS = $(table_SRCS:%.cpp=%.o) $(LDIR)/gtest_main.a

all : $(vr_SRCS) $(rif_SRCS) $(nh_SRCS) $(nhg_SRCS) $(nbr_SRCS) $(route_SRCS) $(counter_SRCS) $(rule_SRCS) $(table_SRCS) $(EXEC_ALL)

# rule for execs
$(BDIR)/$(vr_EXEC): $(vr_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(vr_OBJS) -o $@ $(LDFLAGS)

$(BDIR)/$(rif_EXEC): $(rif_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(rif_OBJS) -o $@ $(LDFLAGS)

$(BDIR)/$(nh_EXEC): $(nh_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(nh_OBJS) -o $@ $(LDFLAGS)

$(BDIR)/$(nhg_EXEC): $(nhg_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(nhg_OBJS) -o $@ $(LDFLAGS)

$(BDIR)/$(nbr_EXEC): $(nbr_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(nbr_OBJS) -o $@ $(LDFLAGS)

$(BDIR)/$(route_EXEC): $(route_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(route_OBJS) -o $@ $(LDFLAGS)

$(BDIR)/$(counter_EXEC): $(counter_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(counter_OBJS) -o $@ $(LDFLAGS)

$(BDIR)/$(rule_EXEC): $(rule_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(rule_OBJS) -o $@ $(LDFLAGS)

$(BDIR)/$(table_EXEC): $(table_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(table_OBJS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDEFLAGS) -o $@ -c $<
 
clean :
	rm -f $(EXEC_ALL) *.o routing/*.o


