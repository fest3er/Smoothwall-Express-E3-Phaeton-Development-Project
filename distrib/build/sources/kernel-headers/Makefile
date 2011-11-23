# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile
include ../Makefile.conf
include ../Makefile.versions

PACKAGE = kernel-headers
VERSION = 0.0

download:
	@$(DL_CMD) $(KERNEL_URL)

install: download
	@tar -xvf $(DOWNLOADS_DIR)/linux-$(KERNEL_VER).tar.xz
ifeq ($(BUILD_PHASE), Stage_1)
	cd linux-$(KERNEL_VER); \
	  make mrproper; \
	  sed -e 's/^CONFIG_LOCALVERSION.*$$/CONFIG_LOCALVERSION="$(SWETYPE)"/' ../../linux.config-$(KERNEL_TYPE)-$(ARCH) > .config && \
	  make headers_check && \
	  make INSTALL_HDR_PATH=dest headers_install && \
	  cp -rv dest/include/* $(TOOLS_DIR)/include
endif
ifeq ($(BUILD_PHASE), Final)
	mkdir -p $(PKG_ROOT); \
	cd linux-$(KERNEL_VER); \
	  make mrproper
	cd linux-$(KERNEL_VER); \
	  sed -e 's/^CONFIG_LOCALVERSION.*$$/CONFIG_LOCALVERSION="$(SWETYPE)"/' ../../linux.config-$(KERNEL_TYPE)-$(ARCH) > .config
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
	tar -xvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C /);

clean:
	@echo -n " $(PACKAGE)"
	@rm -rf linux-$(KERNEL_VER)
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz
endif

all: build-tarball
