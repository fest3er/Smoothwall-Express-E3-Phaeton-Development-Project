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

DEFAULT = yes
default: all
unpack: clean configure


include ../Makefile.conf
include ../Makefile.versions


PACKAGE = iptables
VERSION =  $(IPTABLES_VER)

EXTENSION = .tar.bz2
BASE_URL = ftp://ftp.netfilter.org/pub/iptables

# iptables 1.4.2
ifeq ($(VERSION), 1.4.2)
CONFIG_OPTS = --with-kbuild=$(SOURCES_DIR)/kernel-phaeton/linux --enable-libipq

CONFIGURE = yes

configure: $(DIR)/ patch
	(if [ ! -e $(COMPILE_DIR) ]; then mkdir $(COMPILE_DIR); fi)
ifneq ($(DIR), $(COMPILE_DIR))
	(cd $(COMPILE_DIR); ../$(DIR)/configure --prefix=$(PKG_DIR) $(CONFIG_OPTS))
else
	(cd $(COMPILE_DIR); ./configure --prefix=$(PKG_DIR) $(CONFIG_OPTS))
endif

COMPILE = yes

compile:
	make -C $(COMPILE_DIR) COPT_FLAGS='$(CFLAGS)' KERNEL_DIR=$(SOURCES_DIR)/kernel-phaeton/linux PREFIX=$(PKG_DIR)


INSTALL = yes

install: compile
	make -C $(DIR) KERNEL_DIR=$(SOURCES_DIR)/kernel-phaeton/linux DESTDIR=$(PKG_ROOT) install
	@mkdir -p $(PKG_ROOT)/sbin
	@(cd $(PKG_ROOT)/sbin; rm -f iptables; ln -s /usr/sbin/iptables)
endif

# iptables 1.3.8
ifeq ($(VERSION), 1.3.8)

CONFIGURE = yes
COMPILE = yes
INSTALL = yes


configure: $(DIR)/ patch
	@true

compile: configure
	make -C $(COMPILE_DIR) COPT_FLAGS='$(CFLAGS)' KERNEL_DIR=$(SOURCES_DIR)/kernel-phaeton/linux PREFIX=$(PKG_ROOT)$(PKG_DIR)


install: compile
	make -C $(DIR) KERNEL_DIR=$(SOURCES_DIR)/kernel-phaeton/linux PREFIX=$(PKG_ROOT) install
	@mkdir -p $(PKG_ROOT)/sbin
	@(cd $(PKG_ROOT)/sbin; rm -f iptables; ln -s /usr/sbin/iptables)
endif

include ../Makefile.rules
