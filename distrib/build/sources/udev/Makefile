# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = udev
VERSION = 151
TARBALL = $(PACKAGE)-$(VERSION)

BASE_URL = http://www.us.kernel.org/pub/linux/utils/kernel/hotplug/

#PATCH_URL1=http://www.linuxfromscratch.org/patches/downloads/udev/udev-130-security_fixes-1.patch

PKG_DIR = 

CONFIG_OPTS = --prefix=/usr --with-rootlibdir=/lib --libexecdir=/lib/udev \
	      --sysconfdir=/etc --sbindir=/sbin --libdir=/usr/lib \
	      --docdir=/usr/share/doc/$(PACKAGE)-$(VERSION) --disable-extras \
	      --disable-introspection

COMPILE = yes
INSTALL = yes

compile: configure
	@# Remove and create needed links and targets
	-mkdir $(PKG_ROOT)$(PKG_DIR)
	cd $(PKG_ROOT)$(PKG_DIR); \
	rm -rf lib/firmware lib/udev; \
	install -dv lib/firmware; \
	install -dv lib/udev/devices/pts; \
	install -dv lib/udev/devices/shm; \
	install -dv lib/udev/rules/packages; \
	install -dv lib/udev/rules.d; \
	install -dv usr/share/doc/; \
	mknod -m0666 lib/udev/devices/null c 1 3; \
	mknod -m0600 lib/udev/devices/kmsg c 1 11; \
	ln -sv /proc/self/fd lib/udev/devices/fd; \
	ln -sv /proc/self/fd/0 lib/udev/devices/stdin; \
	ln -sv /proc/self/fd/1 lib/udev/devices/stdout; \
	ln -sv /proc/self/fd/2 lib/udev/devices/stderr; \
	ln -sv /proc/kcore lib/udev/devices/core

	@# Compile
	@make $(JOBS) -C $(COMPILE_DIR)

install: compile
	@mkdir -p $(PKG_ROOT)
	make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	( \
	  cd $(COMPILE_DIR); \
	  install -m644 -vD rules/packages/64-*.rules \
	      $(PKG_ROOT)$(PKG_DIR)/lib/udev/rules.d/; \
	  install -m644 -vD rules/packages/40-isdn.rules \
	      $(PKG_ROOT)$(PKG_DIR)/lib/udev/rules.d/; \
	)

include ../Makefile.rules
