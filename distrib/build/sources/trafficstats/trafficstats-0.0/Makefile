#! /usr/bin/make
# Makefile to make the trafficmon/trafficlogger iptables extensions against a ready compiled iptables source tree
# Martin Houston, SmoothWall Ltd. 
# This version with added support for ipt_ACCOUNT

# where to find the external stuff - you will probably have to change this
IPTABLES_SRC_DIR=/build/sources/iptables/iptables-$(IPTABLES_VERSION)
IPROUTE_INC_DIR=/build/sources/iproute2/iproute2/include
IPT_LIB_DIR=/usr/lib/iptables
KERNEL_DIR=/build/sources/kernel-phaeton/linux-$(KERNEL_VER)
IPROUTE_LIB_DIR=/build/sources/iproute2/iproute2/lib
IPTABLES_OBJS= $(IPTABLES_SRC_DIR)/iptables.o \
	$(IPTABLES_SRC_DIR)/libipq/libipq.a \
	$(IPTABLES_SRC_DIR)/libiptc/libiptc.a

# COPT_FLAGS=-O2
# COPT_FLAGS:=-g
MYCFLAGS=-DMODSWROOT=\"$(MODSWROOT)\" -DIPTABLES_VERSION=\"$(IPTABLES_VERSION)\" -Wall -Wunused -I/usr/include/smoothwall -I$(KERNEL_DIR)/include -I$(IPTABLES_SRC_DIR)/include/ -I$(IPROUTE_INC_DIR)  -I. 


OBJS= trafstats_core.o trafstats_iptables.o traffic_config.o

# LDFLAGS      = -lsmoothwall -L$(IPROUTE_LIB_DIR) -rdynamic
LDFLAGS      = -lsmoothd -L$(IPROUTE_LIB_DIR) -rdynamic
LDLIBS       = -lipt_ACCOUNT -lcurses -lnetlink -ldl -lnsl -lxtables



all: trafficmon trafficlogger

# no fancy include paths should be needed for this unit test
unittests: reltimes_unittest 

# want to build the unit test from source each time, with profiling code
reltimes_unittest: reltimes_unittest.cpp reltimes.hpp timestamp.hpp
	$(CXX) -pg -o reltimes_unittest reltimes_unittest.cpp
	./reltimes_unittest
	mv gmon.out gmon.reltimes_unittest


trafficmon:	trafficmon.o $(OBJS) $(IPTABLES_OBJS)
	$(CXX) $(CFLAGS) $(MYCFLAGS) $(LDFLAGS) -o trafficmon trafficmon.o $(OBJS) $(IPTABLES_OBJS) $(LDLIBS)

trafficmon.o: trafficmon.cpp trafstats_core.hpp timestamp.hpp
	$(CXX) -c $(CFLAGS) $(MYCFLAGS) trafficmon.cpp

trafstats_core.o: trafstats_core.cpp trafstats_core.hpp
	$(CXX) -c $(CFLAGS) $(MYCFLAGS) trafstats_core.cpp

trafficlogger:	trafficlogger.o $(OBJS) $(IPTABLES_OBJS)
	$(CXX) $(CFLAGS) $(MYCFLAGS) $(LDFLAGS) -o trafficlogger trafficlogger.o $(OBJS) $(IPTABLES_OBJS) $(LDLIBS)

trafficlogger.o: trafficlogger.cpp trafstats_core.hpp reltimes.hpp timestamp.hpp
	$(CXX) -c $(CFLAGS) $(MYCFLAGS) trafficlogger.cpp

traffic_config.o: traffic_config.cpp traffic_config.hpp 
	$(CXX) -c $(CFLAGS) $(MYCFLAGS) traffic_config.cpp


# this has to be C not C++ code as the iptables headers wont compile in a C++ prog :(
trafstats_iptables.o: trafstats_iptables.h
	$(CC)  -c $(CFLAGS) $(MYCFLAGS) -o trafstats_iptables.o trafstats_iptables.c


clean:
	@-rm *.o
	@-rm trafficmon trafficlogger

