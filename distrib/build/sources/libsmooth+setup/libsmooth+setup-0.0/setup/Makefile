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

OBJS=main.o hostname.o passwords.o networking.o misc.o webproxy.o \
	keymap.o restore.o isdn.o adsl.o dhcp.o defaults.o

setup: $(OBJS)
	$(LINK) $(OBJS) -o $@ $(LIBS)

%.o : %.c
	$(COMPILE) $< -o $@
		
main.o : main.c
hostname.o : hostname.c
passwords.o : passwords.c
networking.o : networking.c
misc.o : misc.c
keymap.o : keymap.c
webproxy.o : webproxy.c
restore.o : restore.c
console.o : console.c
resetadmin.o : resetadmin.c
setup.o : setup.c
hardware.o : hardware.c
isdn.o : isdn.c
adsl.o : adsl.c
dhcp.o : dhcp.c
defaults.o : defaults.c
