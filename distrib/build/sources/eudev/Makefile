# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = eudev
VERSION = 9e4b735b05
TARBALL = $(PACKAGE)-$(VERSION)

gitURL = http://www.us.kernel.org/pub/linux/utils/kernel/hotplug
gitBranch = master

PATCH_FILE1 = rules-makefile.am.patch

PKG_DIR = 

CONFIG_OPTS = --prefix=                   \
              --bindir=/sbin              \
	      --libexecdir=/usr/lib       \
	      --includedir=/usr/include   \
	      --datarootdir=/usr/share    \
              --disable-manpages          \
              --disable-gudev             \
              --disable-introspection     \
              --disable-keymap            \
              --disable-gtk-doc           \
              --disable-gtk-doc-html      \
              --disable-gtk-doc-pdf


CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: patch
	cd $(COMPILE_DIR); bash autogen.sh
	cd $(COMPILE_DIR); $(PRE_CONFIGURE) ../$(DIR)/configure --prefix=$(PKG_DIR) $(CONFIG_OPTS)

	

compile: configure
	@make $(JOBS) -C $(COMPILE_DIR)

install: compile
	@mkdir -p $(PKG_ROOT)
	make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	cd $(PKG_ROOT); install -dv etc/udev/rules.d;
	cd $(PKG_ROOT); install -dv lib/firmware;
	cd $(PKG_ROOT); install -dv lib/udev/devices/pts;
	cd $(PKG_ROOT); install -dv lib/udev/devices/shm;
	

include ../Makefile.rules
