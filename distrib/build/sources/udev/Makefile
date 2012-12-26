# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = udev
VERSION = 195

TARBALL = $(PACKAGE)-$(VERSION)

BASE_URL = https://bitbucket.org/braindamaged/udev/downloads

PKG_DIR = 

CONFIG_OPTS = --prefix=/usr           \
              --with-rootprefix=''    \
              --bindir=/sbin          \
              --sysconfdir=/etc       \
              --libexecdir=/lib       \
              --enable-rule_generator \
              --disable-manpages      \
              --disable-gudev         \
              --disable-introspection \
              --disable-keymap        \
              --disable-doc           \
              --disable-man           \
              --disable-rtf           \
              --disable-xml           \
              --disable-chm           \
              --disable-chi           \
              --disable-html          \
              --disable-ps            \
              --disable-pdf           \
              --with-usb-ids-path=no  \
              --with-pci-ids-path=no  


COMPILE = yes
INSTALL = yes

compile: configure
	@# Compile
	@make $(JOBS) -C $(COMPILE_DIR)

install: compile
	@mkdir -p $(PKG_ROOT)
	@# Remove and create needed links and targets
	#-mkdir -p $(PKG_ROOT)$(PKG_DIR)/sbin
	#-mkdir -p $(PKG_ROOT)$(PKG_DIR)/run
	#-x; cd $(PKG_ROOT)$(PKG_DIR); \
	#rm -rf lib/firmware lib/udev; \
	#install -dv lib/firmware; \
	#install -dv lib/udev/devices/pts; \
	#install -dv lib/udev/devices/shm; \
	#install -dv usr/share/doc/; \
	#ln -sv /lib/udev/udevd sbin/udevd; \
	#ln -sv /usr/bin/udevadm sbin/udevadm;

	make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install

include ../Makefile.rules
