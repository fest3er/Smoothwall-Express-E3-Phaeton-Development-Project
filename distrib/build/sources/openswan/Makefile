# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = $(FREESWAN_TYPE)
VERSION = $(FREESWAN_VER)

BASE_URL = http://www.openswan.org/download/

#PATCH_FILE1 = makefile.patch
#PATCH_FILE2 = docmakefile.patch
#PATCH_FILE1 = openswan-paths.patch
#PATCH_FILE2 = openswan-script-paths.patch
#PATCH_FILE4 = openswan-logipsecevents.patch

TARBALL = $(FREESWAN_TYPE)-$(FREESWAN_VER)

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: patch
	rm -f /tools/bin/xmlto
	cp xmlto /tools/bin
	chmod 555 /tools/bin/xmlto
	@true
	
compile: configure
	@make -C $(COMPILE_DIR) USERCOMPILE='$(CFLAGS)' INC_USRLOCAL=/$(PKG_DIR) FINALCONFDIR=/$(PKG_DIR)/etc FINALRCDIR=/$(PKG_DIR)/bin programs

install: compile
	@mkdir -p $(PKG_ROOT)/$(PKG_DIR)
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT)/ INC_USRLOCAL=/$(PKG_DIR) FINALCONFDIR=/$(PKG_DIR)/etc FINALRCDIR=/$(PKG_DIR)/bin install
	@rm -rf $(PKG_ROOT)/$(PKG_DIR)/rc*

include ../Makefile.rules
