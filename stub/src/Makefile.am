# Makefile.am -- Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/include -I$(srcdir) \
           -I$(srcdir)/../inc -I$(srcdir)/../../inc \
           -I$(APP_LIB_PATH)/include

if DEBUG
DBGFLAGS = -ggdb -D_DEBUG_
else
DBGFLAGS = -g
endif

CFLAGS = @CFLAGS@ $(CFLAGS_SAI_INTERFACE_COMMON) $(DBGFLAGS)

lib_LTLIBRARIES = libsai.la

libsai_la_SOURCES = \
                       stub_sai_fdb.c \
                       stub_sai_interfacequery.c \
                       stub_sai_neighbor.c \
                       stub_sai_nexthop.c \
                       stub_sai_nexthopgroup.c \
                       stub_sai_port.c \
                       stub_sai_route.c \
                       stub_sai_router.c \
                       stub_sai_switch.c \
                       stub_sai_utils.c \
                       stub_sai_vlan.c \
                       stub_sai_rif.c \
                       stub_sai_host_interface.c \
                       stub_sai_lag.c
					   
libsai_la_LIBADD =

libsai_apiincludedir = $(includedir)/sai
libsai_apiinclude_HEADERS = $(top_srcdir)/../inc/*.h


libsai_api_version=$(shell grep LIBVERSION= $(top_srcdir)/sai_interface.ver | sed 's/LIBVERSION=//')
libsai_la_LDFLAGS = -version-info $(libsai_api_version)
