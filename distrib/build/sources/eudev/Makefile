# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = eudev
VERSION = 74852a5aeb
TARBALL = $(PACKAGE)-$(VERSION)
# Stated explicitly: dlverify uses only gzip to make the tarball
EXTENSION = .tar.gz

# There's no tarball; fetch via git
# Caveat: dlverify looks in MIRROR_UNO and MIRROR_DOS for the mfred
#   tarball; if not found there, it fetches via git
PRE_DL += pkgName=$(PACKAGE) gitCommitID=$(VERSION)
PRE_DL += gitURL=git://github.com/gentoo/eudev.git
PRE_DL += gitBranch=master


PATCH_FILE1 = rules-makefile.am.patch
#PATCH_FILE2 = udevd-makefile.am.patch

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
	cd $(PKG_ROOT)/lib/udev/rules.d; \
	    sed -i -e 's/\(^ENV{MATCHADDR}=="52:54:00:.*generator_end"\)/#\1/' \
	       75-persistent-net-generator.rules
	

include ../Makefile.rules
