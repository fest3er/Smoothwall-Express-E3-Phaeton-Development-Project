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

CONFIG_OPTS = --prefix=/usr           \
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
	# Udev insists on making the docs even though expressly told not to
	# So brutally bypass it
	@cd $(DIR); sed -i -e 's=.*http://docbook.*=\ttrue=' Makefile
	# *Now* we can build
	@make $(JOBS) -C $(COMPILE_DIR)

install: compile
	@mkdir -p $(PKG_ROOT)
	make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	cd $(PKG_ROOT); install -dv etc/udev/rules.d;
	cd $(PKG_ROOT); install -dv lib/firmware;
	cd $(PKG_ROOT); install -dv lib/udev/devices/pts;
	cd $(PKG_ROOT); install -dv lib/udev/devices/shm;
	cd $(PKG_ROOT)/lib/udev/rules.d; \
	    sed -i -e 's/\(^ENV{MATCHADDR}=="52:54:00:.*generator_end"\)/#\1/' \
	       75-persistent-net-generator.rules


include ../Makefile.rules
