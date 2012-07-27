# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = udev
VERSION = 182
TARBALL = $(PACKAGE)-$(VERSION)

BASE_URL = http://www.us.kernel.org/pub/linux/utils/kernel/hotplug/

#PATCH_URL1=http://www.linuxfromscratch.org/patches/downloads/udev/udev-130-security_fixes-1.patch

PKG_DIR = 

CONFIG_OPTS = --prefix=/usr           \
              --with-rootprefix=''    \
              --bindir=/sbin          \
              --sysconfdir=/etc       \
              --libexecdir=/lib       \
              --enable-rule_generator \
              --disable-introspection \
              --disable-keymap        \
              --disable-gudev         \
              --with-usb-ids-path=no  \
              --with-pci-ids-path=no  \
              --with-systemdsystemunitdir=no

COMPILE = yes
INSTALL = yes

compile: configure
	@# Compile
	@make $(JOBS) -C $(COMPILE_DIR)

install: compile
	@mkdir -p $(PKG_ROOT)
	@# Remove and create needed links and targets
	-mkdir -p $(PKG_ROOT)$(PKG_DIR)/sbin
	-x; cd $(PKG_ROOT)$(PKG_DIR); \
	rm -rf lib/firmware lib/udev; \
	install -dv lib/firmware; \
	install -dv lib/udev/devices/pts; \
	install -dv lib/udev/devices/shm; \
	install -dv usr/share/doc/; \
	ln -sv /lib/udev/udevd sbin/udevd; \
	ln -sv /usr/bin/udevadn sbin/udevadm;

	make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install

include ../Makefile.rules
