# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

CC=gcc 
CFLAGS=-O2 -Wall

COMPILE=$(CC) $(CFLAGS)

PROGS=listtables

all : $(PROGS)

clean : 
	-rm -f $(PROGS) *.o core

######

% : %.c
	$(COMPILE) $< -o $@

listtables : listtables.c
