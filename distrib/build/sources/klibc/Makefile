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

CONFIGURE = YES
configure: patch
	true

COMPILE = yes

compile: configure
	cd ../linux-kvm/linux-$(KERNEL_VER); if [ ! -h include/asm ]; then \
	  ln -s ../arch/x86/include/asm include/asm; fi; \
	  cp arch/x86/include/generated/asm/* arch/x86/include/asm/
	cd $(DIR); \
	  make mrproper; \
	  ln -sf ../../linux-kvm/linux-$(KERNEL_VER) linux; \
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
