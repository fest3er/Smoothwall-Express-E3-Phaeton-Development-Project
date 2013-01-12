# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = dialog
VERSION = 1.2-20121230
EXTENSION = .tgz

BASE_URL = ftp://invisible-island.net/$(PACKAGE)

PRECONFIGURE = LDFLAGS=-ltinfo
CONFIG_OPTS = --with-ncurses=tinfo

include ../Makefile.rules
