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
	@$(DL_CMD) http://www.kernel.org/pub/linux/kernel/v2.6/linux-$(KERNEL_VER).tar.bz2

install: download
	@tar -jxvf $(DOWNLOADS_DIR)/linux-$(KERNEL_VER).tar.bz2
ifeq ($(BUILD_PHASE), Stage_1)
	cd linux-$(KERNEL_VER); \
	  make mrproper; \
	  sed -e 's/^CONFIG_LOCALVERSION.*$$/CONFIG_LOCALVERSION="$(SWETYPE)"/' ../../kernel.config$(SWETYPE)-$(ARCH) > .config && \
	  make headers_check && \
	  make INSTALL_HDR_PATH=dest headers_install && \
	  cp -rv dest/include/* $(TOOLS_DIR)/include
endif
ifeq ($(BUILD_PHASE), Final)
	cd linux-$(KERNEL_VER); \
	  make mrproper
	cd linux-$(KERNEL_VER); \
	  sed -e 's/^CONFIG_LOCALVERSION.*$$/CONFIG_LOCALVERSION="$(SWETYPE)"/' ../../kernel.config$(SWETYPE)-$(ARCH) > .config
	cd linux-$(KERNEL_VER); \
	  make prepare
	cd linux-$(KERNEL_VER); \
	  make headers_check
	cd linux-$(KERNEL_VER); \
	  make INSTALL_HDR_PATH=$(PKG_ROOT)$(PKG_DIR) headers_install
	cd linux-$(KERNEL_VER); \
	  find $(PKG_ROOT)$(PKG_DIR)/include \( -name .install -o -name ..install.cmd \) -delete
endif

build-tarball: install
	@(cd $(PKG_ROOT); \
	tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz .; \
	tar -zxvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C /);

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf linux-$(KERNEL_VER)

all: build-tarball
