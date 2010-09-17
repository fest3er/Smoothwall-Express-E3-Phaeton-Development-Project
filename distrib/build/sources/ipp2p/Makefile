include ../Makefile.conf
include ../Makefile.versions

PACKAGE =  ipp2p
VERSION =  $(IPP2P_VER)

LICENCE_FILE = $(DIR)/COPYING

BASE_URL = http://www.ipp2p.org/downloads/

# Patch source: http://burnachurch.com/download/6/
PATCH_FILE1 = ipp2p-2.6.26.patch
PATCH_FILE3 = Makefile.patch
#PATCH_FILE2 = ipp2p-limits.patch

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: patch
	@true

compile: configure
	@make -C $(COMPILE_DIR) CFLAGS='$(CFLAGS)' KERNEL_SRC=$(SOURCES_DIR)/kernel-phaeton/linux KERNEL_SERIES=2.6 IPTABLES_VERSION=$(IPTABLES_VER) IPTABLES_SRC=$(SOURCES_DIR)/iptables/iptables-$(IPTABLES_VER) PREFIX=/$(PKG_DIR) libipt_ipp2p.so

install: compile
	@mkdir -p $(PKG_ROOT)
	@install -D $(COMPILE_DIR)/libipt_ipp2p.so $(PKG_ROOT)$(PKG_DIR)/lib/iptables/libipt_ipp2p.so

include ../Makefile.rules
