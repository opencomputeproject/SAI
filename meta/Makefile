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

WARNINGS = \
	-ansi \
	-Wall \
	-Wcast-align \
	-Wcast-qual \
	-Wconversion \
	-Wdisabled-optimization \
	-Werror \
	-Wextra \
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

CFLAGS += -I../inc -I../experimental $(WARNINGS)

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++

DEPS = $(wildcard ../inc/*.h) $(wildcard ../experimental/*.h)
XMLDEPS = $(wildcard xml/*.xml)

OBJ = saimetadata.o saimetadatautils.o saiserialize.o

SYMBOLS = $(OBJ:=.symbols)

all: toolsversions saisanitycheck saimetadatatest saiserializetest saidepgraph.svg $(SYMBOLS)
	./checkheaders.pl ../inc ../inc
	./checkenumlock.sh
	./checkancestry.sh
	./saimetadatatest >/dev/null
	./saiserializetest >/dev/null
	./saisanitycheck

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

saimetadatatest.c saimetadata.c saimetadata.h: xml $(XMLDEPS) parse.pl $(CONSTHEADERS) $(EXTRA)
	perl -I. parse.pl

rpc sai.thrift sai_rpc_server.cpp sai_adapter.py: xml $(XMLDEPS) gensairpc.pl
	perl -Irpc gensairpc.pl

HEADERS = saimetadata.h $(CONSTHEADERS)

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
	nm $^ | ./checksymbols.pl

saidepgraph.gv: saidepgraphgen
	./saidepgraphgen > $@

saidepgraph.svg: saidepgraph.gv
	dot -Tsvg saidepgraph.gv > $@

.PHONY: clean

clean:
	rm -f *.o *~ .*~ *.tmp .*.swp .*.swo *.bak sai*.gv sai*.svg *.o.symbols doxygen*.db
	rm -f saimetadata.h saimetadata.c saimetadatatest.c
	rm -f saisanitycheck saimetadatatest saiserializetest saidepgraphgen
	rm -f sai.thrift sai_rpc_server.cpp sai_adapter.py
	rm -rf xml html dist temp generated
