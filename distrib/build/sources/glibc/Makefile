# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

export GLOBALDEBUG = OFF

include ../Makefile.conf

PACKAGE = glibc
VERSION = 2.8-20080929

EXTENSION = .tar.bz2

BASE_URL =  ftp://ftp.lfs-matrix.net/pub/lfs/lfs-packages/development/

COMPILE_DIR = $(DIR)-compile


ifeq ($(BUILD_PHASE), Final)
PATCH_URL1 = http://www.linuxfromscratch.org/patches/lfs/6.4/glibc-2.8-20080929-iconv_tests-1.patch
PATCH_URL2 = http://www.linuxfromscratch.org/patches/lfs/6.4/glibc-2.8-20080929-ildoubl_test-1.patch
endif


ifeq ($(ARCH), i686)
BUILD = --build=i686-pc-linux-gnu
endif


# Phase 1 compile
#
ifeq ($(BUILD_PHASE), Stage_1)

CONFIG_OPTS = --prefix=$(TOOLS_DIR) --disable-profile --enable-add-ons \
    --enable-kernel=2.6.0 --with-binutils=$(TOOLS_DIR)/bin \
    --without-gd --with-headers=$(TOOLS_DIR)/include \
    --without-selinux --without-cvs

PREPARE = yes
# Prepare
$(DIR)/: download
	tar -jxvf $(DOWNLOADS_DIR)/$(DIR)$(EXTENSION)
	(cd $(DIR); sed -i 's@/etc/ld.so.preload@/tools/etc/ld.so.preload@' elf/rtld.c; \
	echo "CFLAGS += -march=i686 -mtune=native" > configparms)
	mkdir -p $(TOOLS_DIR)/etc && touch $(TOOLS_DIR)/etc/ld.so.conf
endif


# Final compile
#

ifeq ($(BUILD_PHASE), Final)

PATCH_URL1 = http://www.linuxfromscratch.org/patches/lfs/6.4/glibc-2.8-20080929-iconv_tests-1.patch
PATCH_URL2 = http://www.linuxfromscratch.org/patches/lfs/6.4/glibc-2.8-20080929-ildoubl_test-1.patch

CONFIG_OPTS = --prefix=/usr --disable-profile --enable-add-ons \
    --enable-kernel-2.6.0 --libexecdir=/usr/lib/glibc

DOWNLOAD = yes
download:
	$(DL_CMD) $(BASE_URL)/glibc-$(VERSION)$(EXTENSION)
	$(DL_CMD) $(PATCH_URL1) $(PATCH_MD51)
	$(DL_CMD) $(PATCH_URL2) $(PATCH_MD52)



PREPARE = yes
# Prepare
$(DIR)/: download
	tar -jxvf $(DOWNLOADS_DIR)/$(DIR)$(EXTENSION)
	(cd $(DIR); sed -i '/vi_VN.TCVN/d' localedata/SUPPORTED; \
	echo "CFLAGS += -march=i686 -mtune=native" > configparms; \
	sed -i 's|@BASH@|/bin/bash|' elf/ldd.bash.in)
endif

INSTALL = yes
COMPILE = yes
BUILDTARBALL = yes

compile: configure
	@make ${JOBS} -C $(COMPILE_DIR)
	cp -v $(DIR)/iconvdata/gconv-modules $(COMPILE_DIR)/iconvdata
	mkdir -p $(TOOLS_DIR)/etc && touch $(TOOLS_DIR)/etc/ld.so.conf

# post-compile is an 'orphan': a standalone target to run the tests
post-compile:
	@cd $(COMPILE_DIR); make $(JOBS) -k check 2>&1 | tee glibc-check-log
	@cd $(COMPILE_DIR); grep Error glibc-check-log

install: compile
ifeq ($(BUILD_PHASE), Final)
	@mkdir -p $(PKG_ROOT)/lib
	@mkdir -p $(PKG_ROOT)/usr/lib

ifdef BITS_64
	@(cd $(PKG_ROOT); ln -s lib lib64)
	@(cd $(PKG_ROOT)/usr; ln -s lib lib64)
endif
endif
	make -C $(COMPILE_DIR) install_root=$(PKG_ROOT) install
	make -C $(COMPILE_DIR) localedata/install-locales

# buildtarball
$(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .
	@tar -zxvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C / --exclude ./etc/localtime

include ../Makefile.rules
