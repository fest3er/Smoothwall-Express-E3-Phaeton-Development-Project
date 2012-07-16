# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = expect
VER_MM = 5.43
VERSION = $(VER_MM).0

BASE_URL = http://anduin.linuxfromscratch.org/sources/LFS/lfs-packages/6.6/

TARBALL = $(PACKAGE)-$(VERSION)

DIR = $(PACKAGE)-$(VER_MM)

ifeq ($(BUILD_PHASE), Stage_3)

# Patch files are from LFS 6.6
PATCH_FILE1 = expect-5.43.0-spawn-1.patch
PATCH_FILE2 = expect-5.43.0-tcl_8.5.5_fix-1.patch

CONFIG_OPTS = --prefix=$(TOOLS_DIR) --with-tcl=/$(TOOLS_DIR)/lib \
	--with-tclinclude=$(TOOLS_DIR)/include --with-x=no

CONFIGURE = yes

configure: patch
	cd $(COMPILE_DIR); \
	  cp -v configure{,.orig}; \
	  sed 's:/usr/local/bin:/bin:' configure.orig > configure; \
	  $(PRE_CONFIGURE) ./configure $(CONFIG_OPTS)

INSTALL = yes

install: compile
	@cd $(COMPILE_DIR); make SCRIPTS="" DESTDIR=$(PKG_ROOT) install
endif

include ../Makefile.rules
