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
# @file    Makefile
#
# @brief   This module defines SAI Metadata Makefile
#

# Passed to genrpc.pl:
GEN_SAIRPC_OPTS?=

WARNINGS = \
	-ansi \
	-Wall \
	-Wcast-align \
	-Wcast-qual \
	-Wconversion \
	-Wdisabled-optimization \
	-Werror \
	-Wextra \
	-Wfloat-equal \
	-Wformat=2 \
	-Wformat-nonliteral \
	-Wformat-security \
	-Wformat-y2k \
	-Wimport \
	-Winit-self \
	-Winline \
	-Winvalid-pch \
	-Wmissing-field-initializers \
	-Wmissing-format-attribute \
	-Wmissing-include-dirs \
	-Wmissing-noreturn \
	-Wno-aggregate-return \
	-Wno-padded \
	-Wno-switch-enum \
	-Wno-unused-parameter \
	-Wpacked \
	-Wpointer-arith \
	-Wredundant-decls \
	-Wshadow \
	-Wstack-protector \
	-Wstrict-aliasing=3 \
	-Wswitch \
	-Wswitch-default \
	-Wunreachable-code \
	-Wunused \
	-Wvariadic-macros \
	-Wwrite-strings

BINS = doxygen perl dot

$(foreach bin,$(BINS),$(if $(shell which $(bin)),,$(error "Missing $(bin) in PATH")))

CFLAGS += -I../inc -I../experimental -I../custom -fPIC $(WARNINGS)

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld

DEPS = $(wildcard ../inc/*.h) $(wildcard ../experimental/*.h) $(wildcard ../custom/*.h)
XMLDEPS = $(wildcard xml/*.xml)

OBJ = saimetadata.o saimetadatautils.o saiserialize.o

SYMBOLS = $(OBJ:=.symbols)

all: toolsversions saisanitycheck saimetadatatest saiserializetest saidepgraph.svg $(SYMBOLS)
	./checksymbols.pl *.o.symbols
	./checkheaders.pl ../inc ../inc
	./aspellcheck.pl
	./checkenumlock.sh
	./checkancestry.sh
	./checkstructs.sh
	./saimetadatatest >/dev/null
	./saiserializetest >/dev/null
	./saisanitycheck

apitest: saimetadatatest.c
	$(CC) -o apitest saimetadatatest.c -DAPI_IMPLEMENTED_TEST -lsai $(CFLAGS) $(OBJ)
	./apitest

toolsversions:
	@make -v | grep -i make
	@$(CC) --version | grep gcc
	@perl --version | grep version
	@aspell --version
	@dot -V
	@doxygen -v | perl -npe 'print "doxygen: "'
	@nm --version | grep nm

CONSTHEADERS = saimetadatatypes.h saimetadatalogger.h saimetadatautils.h saiserialize.h

DOXYGEN_VERSION_CHECK = $(shell printf "$$(doxygen -v)\n1.8.16" | sort -V | head -n1)
ifeq (${DOXYGEN_VERSION_CHECK},1.8.16)
	DOXYFILE = Doxyfile
else
	DOXYFILE = Doxyfile.compat
endif

xml: $(DEPS) Doxyfile Doxyfile.compat $(CONSTHEADERS)
	doxygen ${DOXYFILE} 2>&1 | perl -npe '$$e=1 if /warning/i; END{exit $$e}'
	touch xml

EXTRA = acronyms.txt aspell.en.pws *.pm *.cap

saiattrversion.h: $(DEPS) attrversion.sh
	./attrversion.sh

saimetadatasize.h: $(DEPS)
	./size.sh

saimetadatatest.c saimetadata.c saimetadata.h: xml $(XMLDEPS) parse.pl $(CONSTHEADERS) $(EXTRA) saiattrversion.h
	perl -I. parse.pl

RPC_MODULES=$(shell find rpc -type f -name "*.pm")

sai.proto: xml $(XMLDEPS) gensaiprotorpc.pl templates/*.tt $(RPC_MODULES)
	perl -Irpc gensaiprotorpc.pl $(GEN_SAIRPC_OPTS)

protorpc: sai.proto

sai.thrift sai_rpc_server.cpp sai_adapter.py: xml $(XMLDEPS) gensairpc.pl templates/*.tt $(RPC_MODULES)
	perl -Irpc gensairpc.pl $(GEN_SAIRPC_OPTS)

rpc: sai.thrift sai_rpc_server.cpp sai_adapter.py

HEADERS = saimetadata.h saimetadatasize.h $(CONSTHEADERS)

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

saisanitycheck: saisanitycheck.o $(OBJ)
	$(CC) -o $@ $^

saimetadatatest: saimetadatatest.o $(OBJ)
	$(CC) -o $@ $^

saiserializetest: saiserializetest.o $(OBJ)
	$(CC) -o $@ $^

saidepgraphgen: saidepgraphgen.o $(OBJ)
	$(CXX) -o $@ $^

%.o.symbols: %.o
	nm $^ > $@

saidepgraph.gv: saidepgraphgen
	./saidepgraphgen > $@

saidepgraph.svg: saidepgraph.gv
	dot -Tsvg saidepgraph.gv > $@

libsaimetadata.so: $(OBJ)
	$(CXX) -fPIC -shared -Wl,-Bsymbolic-functions -Wl,-z,relro -Wl,-z,now $^ -o $@

libsai.so: libsai.o
	$(CXX) -fPIC -shared -Wl,-Bsymbolic-functions -Wl,-z,relro -Wl,-z,now $^ -o $@

RPC_SRC=$(wildcard generated/gen-cpp/*.cpp)
RPC_OBJ=$(RPC_SRC:.cpp=.o)

generated/gen-cpp/%.o: generated/gen-cpp/%.cpp
	$(CXX) $(CFLAGS) -std=c++11 -Wno-return-type -c $< -o $@

generated/gen-cpp/sai_rpc_server.skeleton: sai.thrift $(RPC_OBJ)
	$(CXX) $(RPC_OBJ) -o generated/gen-cpp/sai_rpc_server.skeleton -lthrift

sai_rpc_server.skeleton: generated/gen-cpp/sai_rpc_server.skeleton

sai_rpc_frontend: rpc protorpc sai_rpc_frontend.cpp sai_rpc_frontend.main.cpp sai_rpc_server.cpp libsaimetadata.so libsai.so
	$(CXX) $(CFLAGS) -std=c++11 \
		generated/gen-cpp/sai_rpc.o generated/gen-cpp/sai_types.o generated/gen-cpp/sai_constants.o \
		sai_rpc_frontend.main.cpp sai_rpc_frontend.cpp \
		libsaimetadata.so libsai.so -lthrift -lpthread -I generated/gen-cpp -o sai_rpc_frontend
.PHONY: clean rpc protorpc


clean:
	rm -f *.o *~ .*~ *.tmp .*.swp .*.swo *.bak sai*.gv sai*.svg *.o.symbols doxygen*.db *.so
	rm -f saimetadata.h saimetadatasize.h saimetadata.c saimetadatatest.c saiswig.i saiattrversion.h
	rm -f saisanitycheck saimetadatatest saiserializetest saidepgraphgen sai_rpc_frontend
	rm -f sai.thrift sai_rpc_server.cpp sai_adapter.py sai.proto
	rm -f *.gcda *.gcno *.gcov
	rm -rf xml html dist temp generated generated-protobuf
