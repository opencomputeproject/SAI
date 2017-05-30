#  Makefile for SAI unit-tests
#  add gtest_dir and platform-specific directories
#  important - link to your platform's SAI library
#  add new unit-tests to SRCS
#  run make {all|clean} 
#  run the resultant binary from EXEC, along with SAI library
#  

# Pointer to the root of Google Test
# download from https://code.google.com/p/googletest/downloads/list
GTEST_DIR = ./gtest-1.7.0

# provide location to SAI include files
SAI_INCLUDE_FLAGS = -I../inc 

# add unit-test sources here 
SRCS = sai_port_unit_test.cpp

### platform specific Linker/LD Flags
# add pointers to SAI library
# by adding -l<sai> and -L<location-to-libsai.so> directives
# add other linker dependencies too. alternately, dlopen/dlsym
# in unit-test source. 
PLATFORM_LINK_LDFLAGS = 
###########

# single binary for all unit-tests
EXEC = test_sai

# what to use for compiling
CXX=g++
AR=ar

# include dir for unit-tests
INCLUDEFLAGS = ${SAI_INCLUDE_FLAGS} -I$(GTEST_DIR)/include 

# libraries to be included for unit-tests
LDFLAGS = ${PLATFORM_LINK_LDFLAGS}  -lpthread

# rule for objs
OBJS = $(SRCS:%.cpp=%.o) gtest_main.a

all : $(SRCS) $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDEFLAGS) -o $@ -c $<
 
clean :
	rm -f $(EXEC) gtest.a gtest_main.a *.o

###### google-test specific Builds gtest.a and gtest_main.a.  #####

# Flags passed to the preprocessor.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# gtest Flags passed to the C++ compiler.
CXXFLAGS += -g3 -Wall -Werror -pthread

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

#### google-test specific - end ####

