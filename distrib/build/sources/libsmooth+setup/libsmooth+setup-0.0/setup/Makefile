# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../../../Makefile.versions

CC=gcc -g

LD=gcc
LDFLAGS=-g -L../libsmooth
LIBS=-lnewt -lslang -ldl -lsmooth

COMPILE=$(CC) -c $(INCLUDE) $(CFLAGS)

LINK=$(LD) $(LIBRARY) $(LDFLAGS)

all : programs

programs : setup

clean :
	-rm -f *.o setup core

######

OBJS=main.o hostname.o passwords.o networking.o misc.o \
	webproxy.o keymap.o isdn.o adsl.o dhcp.o timezone.o

setup: $(OBJS)
	$(LINK) $(OBJS) -o $@ $(LIBS)

%.o : %.c
	$(COMPILE) $< -o $@
		
main.o : main.c
adsl.o : adsl.c
console.o : console.c
dhcp.o : dhcp.c
hardware.o : hardware.c
hostname.o : hostname.c
isdn.o : isdn.c
keymap.o : keymap.c
misc.o : misc.c
networking.o : networking.c
passwords.o : passwords.c
resetadmin.o : resetadmin.c
setup.o : setup.c
timezone.o : timezone.c
webproxy.o : webproxy.c
