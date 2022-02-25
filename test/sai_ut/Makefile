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

fdb_SRCS = ./switching/sai_fdb_unit_test.cpp
vlan_SRCS = ./switching/sai_vlan_unit_test.cpp
lag_SRCS = ./switching/sai_lag_unit_test.cpp
stp_SRCS = ./switching/sai_stp_unit_test.cpp

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
fdb_EXEC   = sai_ut_fdb
vlan_EXEC  = sai_ut_vlan
lag_EXEC  = sai_ut_lag
stp_EXEC   = sai_ut_stp

EXEC_ALL = $(BDIR)/$(vr_EXEC) $(BDIR)/$(rif_EXEC) $(BDIR)/$(nh_EXEC) $(BDIR)/$(nhg_EXEC) $(BDIR)/$(nbr_EXEC) $(BDIR)/$(route_EXEC) $(BDIR)/$(fdb_EXEC) $(BDIR)/$(vlan_EXEC) $(BDIR)/$(lag_EXEC) $(BDIR)/$(stp_EXEC)

# what to use for compiling
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar

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
fdb_OBJS = $(fdb_SRCS:%.cpp=%.o) $(LDIR)/gtest_main.a
vlan_OBJS = $(vlan_SRCS:%.cpp=%.o) $(LDIR)/gtest_main.a
lag_OBJS = $(lag_SRCS:%.cpp=%.o) $(LDIR)/gtest_main.a
stp_OBJS = $(stp_SRCS:%.cpp=%.o) $(LDIR)/gtest_main.a

all : $(vr_SRCS) $(rif_SRCS) $(nh_SRCS) $(nhg_SRCS) $(nbr_SRCS) $(route_SRCS) $(fdb_SRCS) $(vlan_SRCS) $(lag_SRCS) $(stp_SRCS) $(EXEC_ALL)
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

$(BDIR)/$(fdb_EXEC): $(fdb_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(fdb_OBJS) -o $@ $(LDFLAGS)

$(BDIR)/$(vlan_EXEC): $(vlan_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(vlan_OBJS) -o $@ $(LDFLAGS)

$(BDIR)/$(lag_EXEC): $(lag_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(lag_OBJS) -o $@ $(LDFLAGS)

$(BDIR)/$(stp_EXEC): $(stp_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(stp_OBJS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDEFLAGS) -o $@ -c $<
 
clean :
	rm -f $(EXEC_ALL) *.o routing/*.o switching/*.o


