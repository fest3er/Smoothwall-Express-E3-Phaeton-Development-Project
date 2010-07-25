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

#PATCH_URL1 = http://www.linuxfromscratch.org/patches/lfs/6.4/grub-0.97-disk_geometry-1.patch
#PATCH_URL2 = http://www.linuxfromscratch.org/patches/lfs/6.4/grub-0.97-256byte_inode-1.patch
PATCH_FILE1 = grub-0.97-disk_geometry-1.patch
PATCH_FILE2 = grub-0.97-256byte_inode-1.patch

CONFIG_OPTS = --prefix=$(PKG_DIR)

INSTALL = yes

install: compile
	make -C $(COMPILE_DIR) DESTDIR=${PKG_ROOT} install
	mkdir -pv ${PKG_ROOT}/boot/grub
	cp -v ${PKG_ROOT}/usr/lib/grub/i386-pc/stage[12] $(PKG_ROOT)/boot/grub
	cp -v ${PKG_ROOT}/usr/lib/grub/i386-pc/e2fs_stage1_5 $(PKG_ROOT)/boot/grub
	cp -v ${PKG_ROOT}/usr/lib/grub/i386-pc/reiserfs_stage1_5 $(PKG_ROOT)/boot/grub

include ../Makefile.rules
