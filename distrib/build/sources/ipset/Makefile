# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

# The normal default target depends on 'clean' and 'all'.
# However!
# Building iptables is split in two parts:
#   1. Iptables is unpacked, patched as needed, and configured
#   2. The kernel is built; applying POM also patches iptables
#   3. Iptables is configured again, compiled, installed, packaged, etc.
# So the default target may not clean; only the unpack target pre-cleans.
#
# Of course, this should also mean that iptables and the kernel should
# always be built and rebuilt in this manner.

export GLOBALDEBUG = ON

DEFAULT = yes
default: all


include ../Makefile.conf
include ../Makefile.versions
include ../Makefile

export KNL_DIR:="$(SOURCES_DIR)/linux$(KERNEL_TYPE)/linux"

PACKAGE = ipset
VERSION = 4.5
VERSION = 6.13

EXTENSION = .tar.bz2
BASE_URL = http://ipset.netfilter.org

CONFIG_OPTS = --prefix=/usr --mandir=/usr/man --docdir=/usr/doc \
	        --with-kbuild=$(SOURCES_DIR)/linux$(KERNEL_TYPE)/linux

#CONFIGURE = yes
COMPILE = yes
INSTALL = yes


#configure: $(DIR)/ patch
#	true

compile: configure
	make $(JOBS) -C $(COMPILE_DIR) NO_EXTRA_WARN_FLAGS=yes



install: compile
	@mkdir -p $(PKG_ROOT)
	KERNEL_DIR=$(KNL_DIR) make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) PREFIX=$(PKG_DIR) install

include ../Makefile.rules
