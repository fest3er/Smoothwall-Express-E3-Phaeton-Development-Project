# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = procinfo
VERSION = 18

#BASE_URL = ftp://ftp.cistron.nl/pub/people/00-OLD/svm/
#BASE_URL = http://ftp.debian.org/debian/pool/main/p/procinfo/  <-- has wierd file naming convention: procinfo_18.orig.tar.gz
BASE_URL = http://www.kozmix.org/src/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: $(DIR)/
	@true
	
compile: configure
	@make -C $(COMPILE_DIR) LDLIBS=-lncurses

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) prefix=$(PKG_ROOT)$(PKG_DIR) install
	
include ../Makefile.rules
