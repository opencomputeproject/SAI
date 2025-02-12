CXX=g++
ODIR = ./obj
THRIFT = /usr/bin/thrift

PREDIR := ../

METADIR := $(PREDIR)$(PREDIR)meta/

SAI_PREFIX = /usr
SAI_HEADER_DIR ?= $(SAI_PREFIX)/include/sai
SAI_LIBRARY_DIR ?= $(SAI_PREFIX)/lib
SAI_HEADERS = $(SAI_HEADER_DIR)/sai*.h

LDFLAGS = -L$(SAI_LIBRARY_DIR) -Wl,-rpath=$(SAI_LIBRARY_DIR)
CPPFLAGS = -I$(SAI_HEADER_DIR) -I. -std=c++11 -O0 -ggdb

# specify add'l libraries along with libsai
SAIRPC_EXTRA_LIBS?=

ifeq ($(platform),MLNX)
CDEFS = -DMLNXSAI
else
ifeq ($(platform),BFN)
CDEFS = -DBFNSAI
else
ifeq ($(platform),CAVIUM)
CDEFS = -DCAVIUMSAI
else
CDEFS = -DBRCMSAI
endif
endif
endif

ifneq (, $(wildcard /usr/local/bin/ctypesgen))
CTYPESGEN = /usr/local/bin/ctypesgen
else
CTYPESGEN = /usr/local/bin/ctypesgen.py
endif

ifeq ($(platform),vs)
LIBS = -lthrift -lpthread -lsaivs -lsaimeta -lsaimetadata -lzmq
else
LIBS = -lthrift -lpthread -lsai -lsaimetadata
endif


CPP_SOURCES = 	gen-cpp/sai_rpc.cpp \
				gen-cpp/sai_rpc.h \
				gen-cpp/sai_types.cpp \
				gen-cpp/sai_types.h

PY_SOURCES = 	gen-py/__init__.py \
				gen-py/sai/constants.py \
				gen-py/sai/__init__.py \
				gen-py/sai/sai_rpc-remote \
				gen-py/sai/sai_rpc.py \
				gen-py/sai/ttypes.py

SAI_PY_HEADERS = gen-py/sai/sai_headers.py

INSTALL := /usr/bin/install

all: directories meta $(ODIR)/librpcserver.a saiserver clientlib

directories:
	mkdir -p $(ODIR)

meta:
	make -C $(METADIR) saisanitycheck rpc

$(METADIR)sai.thrift:
	make -C $(METADIR) rpc

# rm gen-cpp/gen-py is needed since thrift don't override existing files
# even if sai.thrift file changed :(

$(CPP_SOURCES): $(METADIR)sai.thrift
	rm -rf gen-cpp
	$(THRIFT) -o ./ --gen cpp -r $^

$(PY_SOURCES): $(METADIR)sai.thrift
	rm -rf gen-py
	$(THRIFT) -o ./ --gen py -r $^
	$(INSTALL) -vCD $(METADIR)sai_adapter.py gen-py/sai/sai_adapter.py

# TODO should depend on ../../inc/sai*.h ../../experimental/sai*.h and not /usr/include/sai/sai*.h

$(SAI_PY_HEADERS): $(SAI_HEADERS)
	$(CTYPESGEN) --output-language=py32 -I/usr/include -I$(SAI_HEADER_DIR) -I../../experimental -I../../custom --include /usr/include/linux/limits.h $^ -o $@
	python3 convert_header.py -i $(SAI_PY_HEADERS) -o ./new_header.py
	mv $(SAI_PY_HEADERS) ./sai_headers.py.bk
	mv ./new_header.py $(SAI_PY_HEADERS)

$(METADIR)saimetadata.h:
	make -C $(METADIR) saisanitycheck

$(ODIR)/%.o: gen-cpp/%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(ODIR)/sai_rpc_server.o: $(METADIR)sai_rpc_frontend.cpp $(METADIR)saimetadata.h
	$(CXX) $(CPPFLAGS) -c $(METADIR)sai_rpc_frontend.cpp -o $@ -I$(METADIR) -I./gen-cpp -I../../inc -I../../experimental -I../../custom

$(ODIR)/saiserver.o: src/saiserver.cpp src/switch_sai_rpc_server.h $(CPP_SOURCES)
	$(CXX) $(CPPFLAGS) -c src/saiserver.cpp -o $@ $(CDEFS) -I./gen-cpp -I../../inc -I../../experimental -I../../custom

$(ODIR)/librpcserver.a: $(ODIR)/sai_rpc.o $(ODIR)/sai_types.o $(ODIR)/sai_rpc_server.o
	ar rcs $(ODIR)/librpcserver.a $^

VER=$(shell cat setup.py |grep version | awk -F "'" '{print $$2}')

dist/saithrift-$(VER).tar.gz: $(PY_SOURCES) $(SAI_PY_HEADERS)
	python3 setup.py sdist

clientlib: dist/saithrift-$(VER).tar.gz

saiserver: $(ODIR)/sai_rpc_server.o $(ODIR)/saiserver.o $(ODIR)/librpcserver.a
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS) $(SAIRPC_EXTRA_LIBS)

install-lib: $(ODIR)/librpcserver.a saiserver
	$(INSTALL) -vCD $(ODIR)/librpcserver.a $(DESTDIR)/usr/lib/librpcserver.a
	$(INSTALL) -vCD saiserver $(DESTDIR)/usr/sbin/saiserver
	$(INSTALL) -vCD ./src/switch_sai_rpc_server.h $(DESTDIR)/usr/include/switch_sai_rpc_server.h

$(METADIR)sai_adapter.py: $(METADIR)sai.thrift

install-pylib: clientlib
	python3 setup.py install --prefix $(DESTDIR)/usr/local$(prefix)

install: all install-lib install-pylib

clean:
	make -C $(METADIR) clean
	rm -rf $(ODIR) dist build saiserver MANIFEST gen-cpp gen-py sai_headers.py.bk

.PHONY: clean directories meta clientlib
