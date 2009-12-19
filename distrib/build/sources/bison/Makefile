# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = bison
VERSION = 2.3

BASE_URL = ftp://ftp.gnu.org/gnu/bison/

CONFIG_OPTS = --prefix=${PKG_DIR}

PATCH = yes
patch: $(DIR)/
	cd $(DIR); echo '#define YYENABLE_NLS 1' >> config.h

include ../Makefile.rules
