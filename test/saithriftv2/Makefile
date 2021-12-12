CXX=g++
ODIR = ./obj
THRIFT = /usr/bin/thrift

PREDIR := ../

METADIR := $(PREDIR)$(PREDIR)meta/

SAI_PREFIX = /usr
SAI_HEADER_DIR ?= $(SAI_PREFIX)/include/sai
SAI_LIBRARY_DIR ?= $(SAI_PREFIX)/lib
SAI_HEADERS = $(SAI_HEADER_DIR)/sai*.h

LIBS = -lthrift -lpthread
LDFLAGS = -L$(SAI_LIBRARY_DIR) -Wl,-rpath=$(SAI_LIBRARY_DIR)
CPPFLAGS = -I$(SAI_HEADER_DIR) -I. -std=c++11
ifeq ($(DEBUG),1)
CPPFLAGS += -O0 -ggdb
endif

ifeq ($(platform),MLNX)
CDEFS = -DMLNXSAI
else
ifeq ($(platform),BFT)
CDEFS = -DBFTSAI
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
LIBS += -lsaivs -lsaimeta -lsaimetadata -lzmq
else
LIBS += -lsai
endif


CPP_SOURCES = 	gen-cpp/sai_constants.cpp \
				gen-cpp/sai_constants.h \
				gen-cpp/sai_rpc.cpp \
				gen-cpp/sai_rpc.h \
				gen-cpp/sai_types.cpp \
				gen-cpp/sai_types.h

PY_SOURCES = 	gen-py/__init__.py \
				gen-py/sai/constants.py \
				gen-py/sai/__init__.py \
				gen-py/sai/switch_sai_rpc.py \
				gen-py/sai/switch_sai_rpc-remote \
				gen-py/sai/ttypes.py

SAI_PY_HEADERS = gen-py/sai/sai_headers.py

MKDIR_P = mkdir -p
INSTALL := /usr/bin/install

all: clean directories meta $(ODIR)/librpcserver.a clientlib saiserver

directories:
	$(MKDIR_P) $(ODIR) 

meta:
	make -C  ../../meta clean
	make -C  ../../meta
	make -C  ../../meta rpc

$(CPP_SOURCES): $(METADIR)sai.thrift
	$(THRIFT) -o ./ --gen cpp -r $^

$(PY_SOURCES):  $(METADIR)sai.thrift
	$(THRIFT) -o ./ --gen py -r $^

$(SAI_PY_HEADERS): $(SAI_HEADERS) $(PY_SOURCES)
	$(CTYPESGEN) --output-language=py32 -I/usr/include -I$(SAI_HEADER_DIR) --include /usr/include/linux/limits.h $^ -o $@

$(ODIR)/%.o: gen-cpp/%.cpp meta
	$(CXX) $(CPPFLAGS) -c $< -o $@  -I../../meta

$(ODIR)/sai_rpc_server.o: ../../meta/sai_rpc_frontend.cpp
	$(CXX) $(CPPFLAGS) -c $^ -o $@ $(CPPFLAGS) -I./gen-cpp -I../../meta -I../../inc -I../../experimental

$(ODIR)/saiserver.o: src/saiserver.cpp $(CPP_SOURCES) directories
	$(CXX) $(CPPFLAGS) -c src/saiserver.cpp  -o $@ $(CPPFLAGS) $(CDEFS) -I./gen-cpp -I../../inc

$(ODIR)/librpcserver.a: $(ODIR)/sai_rpc.o $(ODIR)/sai_types.o $(ODIR)/sai_constants.o $(ODIR)/sai_rpc_server.o
	ar rcs $(ODIR)/librpcserver.a $^

clientlib: $(PY_SOURCES) $(SAI_PY_HEADERS)
	python3 setup.py sdist

saiserver: $(ODIR)/saiserver.o $(ODIR)/librpcserver.a
	$(CXX) $(LDFLAGS) $(ODIR)/sai_rpc_server.o $(ODIR)/saiserver.o -o $@ \
		   $(ODIR)/librpcserver.a $(LIBS)

install-lib: $(ODIR)/librpcserver.a
	$(INSTALL) -D $(ODIR)/librpcserver.a $(DESTDIR)/usr/lib/librpcserver.a
	$(INSTALL) -D saiserver $(DESTDIR)/usr/sbin/saiserver
	$(INSTALL) -D ./src/switch_sai_rpc_server.h $(DESTDIR)/usr/include/switch_sai_rpc_server.h

install-pylib: clientlib
	cp $(METADIR)sai_adapter.py gen-py/sai/
	python3 setup.py install --prefix $(DESTDIR)/usr/local$(prefix)

install: install-lib install-pylib

clean:
	make -C ../../meta clean
	rm -rf $(ODIR) dist build saiserver MANIFEST gen-cpp gen-py
