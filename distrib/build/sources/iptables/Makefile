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


PACKAGE = iptables
VERSION =  $(IPTABLES_VER)

EXTENSION = .tar.bz2
BASE_URL = ftp://ftp.netfilter.org/pub/iptables

CONFIG_OPTS = --prefix=/usr --mandir=/usr/man --docdir=/usr/doc \
	        --enable-devel --enable-libipq

CONFIGURE = yes
COMPILE = yes
INSTALL = yes


configure: $(DIR)/ patch
	(if [ ! -e $(COMPILE_DIR) ]; then mkdir $(COMPILE_DIR); fi)
ifneq ($(DIR), $(COMPILE_DIR))
	(cd $(COMPILE_DIR); ../$(DIR)/configure --prefix=$(PKG_DIR) $(CONFIG_OPTS))
else
	(cd $(COMPILE_DIR); ./configure --prefix=$(PKG_DIR) $(CONFIG_OPTS))
endif

compile: configure
        make $(JOBS) -C $(COMPILE_DIR) COPT_FLAGS='$(CFLAGS)' PREFIX=$(PKG_DIR)


install: compile
	@mkdir -p $(PKG_ROOT)
	make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	@mkdir -p $(PKG_ROOT)/sbin
	@(cd $(PKG_ROOT)/sbin; rm -f iptables; ln -s /usr/sbin/iptables)

include ../Makefile.rules
