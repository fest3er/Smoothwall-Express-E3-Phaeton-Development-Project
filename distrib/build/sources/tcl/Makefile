# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = tcl
VERSION = $(TCL_VER)

BASE_URL = http://voxel.dl.sourceforge.net/sourceforge/tcl/

#PATCH_FILE1 = configure-tcl-fixes-bash3.patch
#PATCH_FILE2 = dash-fix.patch
#PATCH_FILE3 = tcl-unix-config-quote.patch

TARBALL = $(PACKAGE)$(VERSION)-src

DIR = $(PACKAGE)$(VERSION)

ifdef BITS_64
CONFIG_OPTS = --enable-64bit
endif

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

compile: configure
	@make -C $(DIR)/unix

ifeq ($(BUILD_PHASE), Stage_3)
configure: patch
	@(cd $(DIR)/unix; ./configure --prefix=$(TOOLS_DIR) $(CONFIG_OPTS))

install: compile
	make -C $(DIR)/unix install
	chmod -v u+w ${TOOLS_DIR}/lib/libtcl8.5.so
	make -C $(DIR)/unix install-private-headers
	rm -f $(TOOLS_DIR)/bin/tclsh
	ln -sv tclsh8.5 $(TOOLS_DIR)/bin/tclsh
endif

ifeq ($(BUILD_PHASE), Final)
configure: patch
	@(cd $(DIR)/unix; ./configure --prefix=$(PKG_DIR) $(CONFIG_OPTS))

install: compile
	@rm -rf $(PKG_ROOT); mkdir -p $(PKG_ROOT)
	@make -C $(DIR)/unix INSTALL_ROOT=$(PKG_ROOT) install
	@ln -s tclsh8.4 $(PKG_ROOT)$(PKG_DIR)/bin/tclsh
endif

include ../Makefile.rules
