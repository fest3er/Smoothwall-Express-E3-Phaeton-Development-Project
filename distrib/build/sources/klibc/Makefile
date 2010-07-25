# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = klibc
VER_MM = 1.5
VERSION = $(VER_MM).18
KLIBKERNVER = 2.6.33.6

BASE_URL = http://www.kernel.org/pub/linux/libs/klibc/$(VER_MM)/

CONFIGURE = YES
configure: patch
	true

COMPILE = yes

compile: configure
	tar xvfj /build/downloads/linux-$(KLIBKERNVER).tar.bz2
	cp ../kernel.config-phaeton-$(KLIBKERNVER) linux-$(KLIBKERNVER)/.config
	cd linux-$(KLIBKERNVER); make prepare headers_install
	cd linux-$(KLIBKERNVER); ln -s ../arch/x86/include/asm include/asm
	cd $(DIR); \
	  make mrproper; \
	  ln -sf ../linux-$(KLIBKERNVER) linux; \
	  make -j 4

INSTALL = yes
install: compile
	cd $(DIR); \
	  make prefix=$(PKG_ROOT)$(PKG_DIR) install

CLEAN = yes
clean:
	@echo -n " $(PACKAGE)"
	@rm -rf $(DIR)
	@rm -rf $(COMPILE_DIR)
	@rm -rf linux-$(KLIBKERNVER)
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(BUILDDIR)/buildroot/$(PACKAGE)
	@rm -rf linux-*
endif

include ../Makefile.rules
