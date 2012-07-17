# Copyright 2004-2010 SmoothWall Ltd

# where to find it - relative path
IPTABLES_SRC_DIR:=../../iptables/iptables-$(IPTABLES_VER)
# where the IPT shared libs live
XTABLES_LIBDIR:=$(IPT_LIBEXEC)

KERNEL_DIR:=../../kernel-roadster/linux


CC=gcc 
CXX=g++
COPT_FLAGS:=$(CFLAGS) -g -DDEBUG

CFLAGS:=$(COPT_FLAGS) -DIPTABLES_VERSION=\"$(IPTABLES_VER)\" -Wall -Wunused -I$(IPTABLES_SRC_DIR)/include/ -I.

LDFLAGS      = -rdynamic
LDLIBS       = -ldl -lnsl -lxtables -liptc

PROGS = ipbatch ip6batch

all : $(PROGS)

clean : 
	-rm -f $(PROGS) *.o core

######

ipbatch.o: ipbatch.c $(IPTABLES_SRC_DIR)/include/iptables.h
	$(CC) -c $(CFLAGS) -DXTABLES_LIBDIR=\"$(XTABLES_LIBDIR)\" ipbatch.c -o ipbatch.o
ip6batch.o: ipbatch.c $(IPTABLES_SRC_DIR)/include/iptables.h
	$(CC) -c $(CFLAGS) -DIP6T -DXTABLES_LIBDIR=\"$(XTABLES_LIBDIR)\" ipbatch.c -o ip6batch.o

ipbatch: ipbatch.o $(IPTABLES_SRC_DIR)/iptables/xtables_multi-iptables.o \
                   $(IPTABLES_SRC_DIR)/iptables/xtables_multi-xshared.o
	$(CC) $(LDLIBS) $(LDFLAGS) $(CFLAGS) -o $@  $^
ip6batch: ip6batch.o $(IPTABLES_SRC_DIR)/iptables/xtables_multi-ip6tables.o \
                     $(IPTABLES_SRC_DIR)/iptables/xtables_multi-xshared.o
	$(CC) $(CFLAGS) -DIPT_LIB_DIR=\"$(IPT_LIB_DIR)\" $(LDFLAGS) -o $@ $^ $(LDLIBS)
