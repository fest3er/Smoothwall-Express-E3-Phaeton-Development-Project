# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = kernel-headers
VERSION = 0.0

download:
	$(DL_CMD) http://www.kernel.org/pub/linux/kernel/v2.6/linux-$(KERNEL_VER).tar.bz2

install: download
	@tar -jxvf $(DOWNLOADS_DIR)/linux-$(KERNEL_VER).tar.bz2
	@(cd linux-$(KERNEL_VER); \
        make mrproper; \
        make include/linux/version.h; \
        make include/asm; \
        mkdir -p $(PKG_ROOT); \
        mkdir -p $(PKG_ROOT)$(PKG_DIR)/include/asm; \
        cp -v include/asm/* $(PKG_ROOT)$(PKG_DIR)/include/asm; \
        cp -Rv include/asm-generic $(PKG_ROOT)$(PKG_DIR)/include; \
        cp -Rv include/linux $(PKG_ROOT)$(PKG_DIR)/include; \
        touch $(PKG_ROOT)$(PKG_DIR)/include/linux/autoconf.h)


build-tarball: install
	@(cd $(PKG_ROOT); \
	tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz .; \
	tar -zxvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C /);

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf linux-$(KERNEL_VER)

all: build-tarball
	
