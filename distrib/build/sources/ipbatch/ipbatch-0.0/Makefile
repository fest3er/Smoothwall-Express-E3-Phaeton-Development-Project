# where to find it - relative path
IPTABLES_SRC_DIR:=../../iptables/iptables-$(IPTABLES_VER)
# where the IPT shared libs live
IPT_LIB_DIR:=$(IPT_LIBEXEC)

KERNEL_DIR:=../../kernel-phaeton/linux

CC=gcc 
CXX=g++
COPT_FLAGS:=$(CFLAGS) -g -DDEBUG
CFLAGS:=$(COPT_FLAGS) -DIPTABLES_VERSION=\"$(IPTABLES_VER)\" -Wall -Wunused -I$(KERNEL_DIR)/include -I$(IPTABLES_SRC_DIR)/include/ -I.

LDFLAGS      = -rdynamic
LDLIBS       = -ldl -lnsl -lxtables

PROGS = ipbatch

all : $(PROGS)

clean : 
	-rm -f $(PROGS) *.o core

######

ipbatch.o: ipbatch.c $(IPTABLES_SRC_DIR)/include/iptables.h
	$(CC) -c $(CFLAGS) -DIPT_LIB_DIR=\"$(IPT_LIB_DIR)\" ipbatch.c 

ipbatch: ipbatch.o $(IPTABLES_SRC_DIR)/iptables.o $(IPTABLES_SRC_DIR)/libiptc/libiptc.a
	$(CC) $(CFLAGS) -DIPT_LIB_DIR=\"$(IPT_LIB_DIR)\" $(LDFLAGS) -o $@ $^ $(LDLIBS)

