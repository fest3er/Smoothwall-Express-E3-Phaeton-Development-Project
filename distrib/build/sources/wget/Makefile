# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = wget
VERSION = 1.14

BASE_URL = ftp://ftp.gnu.org/gnu/wget
EXTENSION = .tar.xz

ifeq ($(BUILD_PHASE), Stage_3)
CONFIG_OPTS = --without-ssl
endif

ifeq ($(BUILD_PHASE), Final)
CONFIG_OPTS = --with-ssl=openssl --with-libssl-prefix=/usr/ssl
endif

COMPILE = yes
compile: configure
	$(MAKE) $(JOBS) -C $(COMPILE_DIR)

include ../Makefile.rules
