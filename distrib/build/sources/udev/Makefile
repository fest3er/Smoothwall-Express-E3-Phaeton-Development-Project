# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = udev
VERSION = 171
TARBALL = $(PACKAGE)-$(VERSION)

BASE_URL = http://www.us.kernel.org/pub/linux/utils/kernel/hotplug/

#PATCH_URL1=http://www.linuxfromscratch.org/patches/downloads/udev/udev-130-security_fixes-1.patch

PKG_DIR = 

CONFIG_OPTS = --prefix=/usr --with-rootlibdir=/lib --libexecdir=/lib/udev \
	      --sysconfdir=/etc --sbindir=/sbin --libdir=/usr/lib \
	      --docdir=/usr/share/doc/$(PACKAGE)-$(VERSION) --disable-extras \
	      --disable-introspection --disable-udev_acl --disable-gudev \
	      --without-gperf --disable-hwdb --disable-keymap

COMPILE = yes
INSTALL = yes

compile: configure
	@# Compile
	@make $(JOBS) -C $(COMPILE_DIR)

install: compile
	@mkdir -p $(PKG_ROOT)
	@# Remove and create needed links and targets
	@# rules are not wanted; we'll get them from udev-config
	-mkdir $(PKG_ROOT)$(PKG_DIR)
	cd $(PKG_ROOT)$(PKG_DIR); \
	rm -rf lib/firmware lib/udev; \
	install -dv lib/firmware; \
	install -dv lib/udev/devices/pts; \
	install -dv lib/udev/devices/shm; \
	install -dv usr/share/doc/; \
	ln -sv /proc/kcore lib/udev/devices/core

	make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install

	@# rules are not wanted; we'll get them from udev-config
	cd $(PKG_ROOT)$(PKG_DIR); \
	rm -f lib/udev/rules.d/*

include ../Makefile.rules
