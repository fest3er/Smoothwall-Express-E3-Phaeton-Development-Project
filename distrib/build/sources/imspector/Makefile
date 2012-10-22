# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = imspector
VERSION = 0.9

BASE_URL = http://www.imspector.org/downloads/

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

OPTS = PREFIX=$(PKG_ROOT)$(PKG_DIR)
DIR = imspector-$(VERSION)

PATCH_FILE1 = ssl_method.patch
# use the patch fest3er used since mine seems
# to have stopped working.
#PATCH_FILE1 = ssl-state.cpp.patch

configure: patch
	@true

compile: configure
	@make $(JOBS) -C $(DIR) $(OPTS)

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(DIR) $(OPTS) install
	@rm -f $(PKG_ROOT)$(PKG_DIR)/etc/imspector/imspector.conf

include ../Makefile.rules
