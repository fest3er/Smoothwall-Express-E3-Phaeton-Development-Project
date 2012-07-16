# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = ncurses
VERSION = 5.9

BASE_URL = ftp://ftp.gnu.org/gnu/ncurses/

ifeq ($(BUILD_PHASE), Stage_3)
CONFIG_OPTS = --with-shared \
    --without-debug --without-ada --enable-overwrite
else
CONFIG_OPTS = --with-shared  --without-debug --with-termlib
endif

include ../Makefile.rules
