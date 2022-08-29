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

DEPS = $(wildcard ../inc/*.h) $(wildcard ../experimental/*.h)
XMLDEPS = $(wildcard xml/*.xml)

OBJ = saimetadata.o saimetadatautils.o saiserialize.o

SYMBOLS = $(OBJ:=.symbols)

all: saisanitycheck saimetadatatest saiserializetest saidepgraph.svg $(SYMBOLS)
	./checkheaders.pl ../inc ../inc
	./saimetadatatest >/dev/null
	./saiserializetest >/dev/null
	./saisanitycheck

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
	perl -Irpc gensairpc.pl $(GEN_SAIRPC_OPTS)

HEADERS = saimetadata.h $(CONSTHEADERS)

%.o: %.c $(HEADERS)
	gcc -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(HEADERS)
	gcc -c -o $@ $< $(CFLAGS)

saisanitycheck: saisanitycheck.o $(OBJ)
	gcc -o $@ $^

saimetadatatest: saimetadatatest.o $(OBJ)
	gcc -o $@ $^

saiserializetest: saiserializetest.o $(OBJ)
	gcc -o $@ $^

saidepgraphgen: saidepgraphgen.o $(OBJ)
	g++ -o $@ $^

%.o.symbols: %.o
	nm $^ | ./checksymbols.pl

saidepgraph.gv: saidepgraphgen
	./saidepgraphgen > $@

saidepgraph.svg: saidepgraph.gv
	dot -Tsvg saidepgraph.gv > $@

.PHONY: clean

clean:
	rm -f *.o *~ .*~ *.tmp .*.swp .*.swo *.bak sai*.gv sai*.svg *.o.symbols
	rm -f saimetadata.h saimetadata.c saimetadatatest.c
	rm -f saisanitycheck saimetadatatest saiserializetest saidepgraphgen
	rm -f sai.thrift sai_rpc_server.cpp sai_adapter.py
	rm -rf xml html dist temp generated
