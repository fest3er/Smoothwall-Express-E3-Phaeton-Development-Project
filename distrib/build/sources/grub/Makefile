# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = grub
VERSION = 0.97

BASE_URL = ftp://alpha.gnu.org/gnu/grub/

COMPILE_DIR = $(PACKAGE)-$(VERSION)-compile

PATCH_FILE1 = cvs-sync.patch
PATCH_FILE2 = grub-special_device_names.diff
PATCH_FILE3 = raid_cciss.diff
PATCH_FILE4 = grub-ext4-patch.diff
PATCH_FILE5 = grub-0.97-gpt.diff


CONFIG_OPTS = --prefix=$(PKG_DIR)

INSTALL = yes

install: compile
	@mkdir -p $(PKG_ROOT)
	make -C $(COMPILE_DIR) DESTDIR=${PKG_ROOT} install
	mkdir -pv ${PKG_ROOT}/boot/grub
	cp -av ${PKG_ROOT}/usr/lib/grub/*/* $(PKG_ROOT)/boot/grub
	touch ${PKG_ROOT}/boot/grub/default

include ../Makefile.rules
