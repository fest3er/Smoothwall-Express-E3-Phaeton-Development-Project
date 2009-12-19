# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = procinfo
VERSION = 18

BASE_URL = ftp://ftp.cistron.nl/pub/people/00-OLD/svm/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: $(DIR)/
	@true
	
compile: configure
	@make -C $(COMPILE_DIR) LDLIBS=-lncurses

install: compile
	@make -C $(COMPILE_DIR) prefix=$(PKG_ROOT)/$(PKG_DIR) install
	
include ../Makefile.rules
