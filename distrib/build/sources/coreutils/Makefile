# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = coreutils
VERSION = 8.5

BASE_URL = ftp://ftp.gnu.org/gnu/coreutils/

CONFIGURE = yes

#PATCH_URL1 = http://www.linuxfromscratch.org/patches/lfs/6.4/coreutils-6.12-uname-1.patch
#PATCH_URL2 = http://www.linuxfromscratch.org/patches/lfs/6.4/coreutils-6.12-old_build_kernel-1.patch
#PATCH_URL3 = http://www.linuxfromscratch.org/patches/lfs/6.4/coreutils-6.12-i18n-2.patch
#
#PATCH_FILE1 = coreutils-6.12-old_build_kernel-1.patch

configure: patch
	@cd $(COMPILE_DIR); \
	  ./configure --prefix=$(PKG_DIR) --enable-install-program=hostname;

ifeq ($(BUILD_PHASE), Final)

#PATCH_FILE1 = coreutils-6.12-uname-1.patch
#PATCH_FILE2 = coreutils-6.12-old_build_kernel-1.patch
#PATCH_FILE3 = coreutils-6.12-i18n-2.patch

INSTALL = yes
install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	@(mkdir -p $(PKG_ROOT)/bin; \
	for FILE in basename cat chgrp chmod chown cp cut date dd df echo env false link ln \
		 ls mkdir mknod mv nice pwd rm rmdir sleep sort stty sync touch true uname unlink; \
	do \
		mv $(PKG_ROOT)$(PKG_DIR)/bin/$$FILE $(PKG_ROOT)/bin; \
	done);
	ln -s /bin/env $(PKG_ROOT)$(PKG_DIR)/bin/env
endif
	         
include ../Makefile.rules
