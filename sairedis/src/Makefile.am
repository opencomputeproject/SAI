# Makefile.am -- Process this file with automake to produce Makefile.in

AM_CPPFLAGS = 
AM_CPPFLAGS += -I$(top_srcdir)/../inc
AM_CPPFLAGS += -I$(top_srcdir)/inc

if DEBUG
DBGFLAGS = -ggdb -D_DEBUG_
else
DBGFLAGS = -g
endif

lib_LTLIBRARIES = libsairedis.la

libsairedis_la_SOURCES = sai_redis_acl.cpp \
						 sai_redis_buffer.cpp \
						 sai_redis_fdb.cpp \
						 sai_redis_hash.cpp \
						 sai_redis_hostintf.cpp \
						 sai_redis_interfacequery.cpp \
						 sai_redis_lag.cpp \
						 sai_redis_mirror.cpp \
						 sai_redis_neighbor.cpp \
						 sai_redis_nexthop.cpp \
						 sai_redis_nexthopgroup.cpp \
						 sai_redis_policer.cpp \
						 sai_redis_port.cpp \
						 sai_redis_qosmaps.cpp \
						 sai_redis_queue.cpp \
						 sai_redis_route.cpp \
						 sai_redis_router.cpp \
						 sai_redis_routerintf.cpp \
						 sai_redis_samplepacket.cpp \
						 sai_redis_scheduler.cpp \
						 sai_redis_schedulergroup.cpp \
						 sai_redis_stp.cpp \
						 sai_redis_switch.cpp \
						 sai_redis_vlan.cpp \
						 sai_redis_wred.cpp

libsairedis_la_CPPFLAGS = $(DBGFLAGS) $(AM_CPPFLAGS)
libsairedis_la_LIBADD =

