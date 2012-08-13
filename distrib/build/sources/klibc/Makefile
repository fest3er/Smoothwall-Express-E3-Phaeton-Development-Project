# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile
include ../Makefile.conf
include ../Makefile.versions

PACKAGE = klibc
VER_MM = 2.0
VERSION = $(VER_MM).1

EXTENSION = .tar.bz2

BASE_URL = http://www.kernel.org/pub/linux/libs/klibc/$(VER_MM)/

DOWNLOAD = yes
download:
	@$(DL_CMD) $(BASE_URL)/$(TARBALL)$(EXTENSION) $(MD5)
	@$(DL_CMD) http://www.kernel.org/pub/linux/kernel/v3.0/linux-$(KLIBKERNVER).tar.xz

CONFIGURE = YES
configure: patch
	true

COMPILE = yes

compile: configure
	tar -xvf /build/downloads/linux-$(KLIBKERNVER).tar.xz
	sed -e 's/^CONFIG_LOCALVERSION.*$$/CONFIG_LOCALVERSION="$(KERNEL_TYPE)"/' ../linux.config-domU-i686 > linux-$(KLIBKERNVER)/.config
	cd linux-$(KLIBKERNVER); make prepare headers_install
	cd linux-$(KLIBKERNVER); ln -s ../arch/x86/include/asm include/asm
	cd $(DIR); \
	  make mrproper; \
	  ln -sf ../linux-$(KLIBKERNVER) linux; \
	  make $(JOBS)

INSTALL = yes
install: compile
	@mkdir -p $(PKG_ROOT)
	cd $(DIR); \
	  make prefix=$(PKG_ROOT)$(PKG_DIR) install

CLEAN = yes
clean:
	@echo -n " $(PACKAGE)"
	@rm -rf $(DIR)
	@rm -rf $(COMPILE_DIR)
	@rm -rf linux-$(KLIBKERNVER)
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz
endif

include ../Makefile.rules
