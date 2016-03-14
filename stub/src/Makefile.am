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
					   stub_sai_host_interface.c
					   
libsai_la_LIBADD =

libsai_apiincludedir = $(includedir)/sai
libsai_apiinclude_HEADERS = \
	$(top_srcdir)/../inc/sainexthop.h \
	$(top_srcdir)/../inc/sainexthopgroup.h \
	$(top_srcdir)/../inc/sai.h \
	$(top_srcdir)/../inc/sairouter.h \
	$(top_srcdir)/../inc/saivlan.h \
	$(top_srcdir)/../inc/saineighbor.h \
	$(top_srcdir)/../inc/saifdb.h \
	$(top_srcdir)/../inc/saiport.h \
	$(top_srcdir)/../inc/saiacl.h \
	$(top_srcdir)/../inc/saiswitch.h \
	$(top_srcdir)/../inc/saistatus.h \
	$(top_srcdir)/../inc/sairoute.h \
	$(top_srcdir)/../inc/saitypes.h \
	$(top_srcdir)/../inc/sairouterintf.h \
	$(top_srcdir)/../inc/saihostintf.h \
	$(top_srcdir)/../inc/sailag.h \
    $(top_srcdir)/../inc/saimirror.h \
    $(top_srcdir)/../inc/saisamplepacket.h \
	$(top_srcdir)/../inc/saistp.h \
	$(top_srcdir)/../inc/saipolicer.h \
	$(top_srcdir)/../inc/saiwred.h \
	$(top_srcdir)/../inc/saiqosmaps.h \
	$(top_srcdir)/../inc/saiqueue.h \
	$(top_srcdir)/../inc/saischeduler.h \
	$(top_srcdir)/../inc/saischedulergroup.h \
	$(top_srcdir)/../inc/saibuffer.h


libsai_api_version=$(shell grep LIBVERSION= $(top_srcdir)/sai_interface.ver | sed 's/LIBVERSION=//')
libsai_la_LDFLAGS = -version-info $(libsai_api_version)
