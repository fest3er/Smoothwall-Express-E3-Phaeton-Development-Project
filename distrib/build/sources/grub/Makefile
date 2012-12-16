# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

# Confuser: This is grub-legacy, which is still, inexplicably, named grub.
#   So it's built as grub, but tarred and stored as grub-legacy.
#   Grub-legacy is installed in .../distrib.
#   Grub is installed in installroot and runtimeroot.
#   Grub and grub-legacy do not need to co-exist in any filesystem.
# Grub is also needed to convert the ISO to flash. Thus it has to be
#   available in the installroot initramfs and it has to be available
#   via /distrib in the chroot build environment.
# Grub-legacy is not needed in the build environment or in the installroot
#   tree; it is needed only on the installed hard drive.

include ../Makefile.conf

PACKAGE = grub
VERSION = 0.97

TARBALL = grub-$(VERSION)
BASE_URL = ftp://alpha.gnu.org/gnu/grub/

COMPILE_DIR = $(PACKAGE)-$(VERSION)-compile

PATCH_FILE1 = grub-fedora-17.patch
#PATCH_FILE1 = cvs-sync.patch
#PATCH_FILE2 = grub-special_device_names.diff
#PATCH_FILE3 = raid_cciss.diff
#PATCH_FILE4 = grub-ext4-patch.diff
#PATCH_FILE5 = grub-0.97-gpt.diff


PRE_CONFIGURE = CFLAGS="$(CFLAGS) -march=i686 -Os -Wall -fno-reorder-functions"
CONFIG_OPTS = --prefix=$(PKG_DIR)

#CFLAGS := $(CFLAGS) -march=i686 -Wall -fno-reorder-functions

reconf: patch
	cd $(DIR); autoreconf --force

CONFIGURE = yes
configure: reconf
	(if [ ! -e $(COMPILE_DIR) ]; then mkdir $(COMPILE_DIR); fi)
	(cd $(COMPILE_DIR); $(PRE_CONFIGURE) ../$(DIR)/configure --prefix=$(PKG_DIR) $(CONFIG_OPTS))

INSTALL = yes
install: compile
	@mkdir -p $(PKG_ROOT)
	make -C $(COMPILE_DIR) DESTDIR=${PKG_ROOT} install
	mkdir -pv ${PKG_ROOT}/boot/grub
	cp -av ${PKG_ROOT}/usr/share/grub/*/* $(PKG_ROOT)/boot/grub
	touch ${PKG_ROOT}/boot/grub/default

BUILDTARBALL = yes
$(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz: install
	@tar cvfz $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .
	@tar xvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C /

include ../Makefile.rules
