# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

CC=gcc

COMPILE = $(CC) $(CFLAGS)

PROGS = setdmzholes setportfw setoutbound setxtaccess restartdhcp restartsnort \
	restartsquid smoothiedeath smoothierebirth alcateladslfw \
	installpackage ipsecctrl  restartdnsproxy restartssh \
	setadvnet setipblock restartupnp restartntpd updown

all : $(PROGS)

clean : 
	-rm -f $(PROGS) *.o core

######

% : %.c
	$(COMPILE) $< -o $@

setdmzholes : setdmzholes.c
setportfw : setportfw.c
setoutbound : setoutbound.c
setxtaccess : setxtaccess.c
restartdhcp : restartdhcp.c
restartsnort : restartsnort.c
restartsquid : restartsquid.c
smoothiedeath : smoothiedeath.c
smoothierebirth : smoothierebirth.c
restartdnsproxy : restartdnsproxy.c
restartssh : restartssh.c
alcateladslfw : alcateladslfw.c
setadvnet : setadvnet.c
setipblock : setipblock.c
restartupnp : restartupnp.c
restartntpd : restartntpd.c
updown : updown.c
