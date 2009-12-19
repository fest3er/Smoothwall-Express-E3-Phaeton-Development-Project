# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../../../Makefile

VERSION_DEFINES = -DPRODUCT_NAME="\"$(PRODUCT_NAME)$(PRODUCT_EXTRA)\"" -DPRODUCT_VERSION="\"$(PRODUCT_VERSION)\""

CC=gcc
CFLAGS=-O2 -Wall

COMPILE=$(CC) $(VERSION_DEFINES) $(CFLAGS)

PROGS=loader iowrap

all : $(PROGS)

clean : 
	-rm -f $(PROGS) *.o core

######

% : %.c
	$(COMPILE) $< -o $@

loader : loader.c
iowrap : iowrap.c
