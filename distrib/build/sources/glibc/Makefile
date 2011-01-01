# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = glibc
VERSION = 2.11.2

EXTENSION = .tar.bz2

BASE_URL =  ftp://ftp.gnu.org/gnu/glibc

COMPILE_DIR = $(DIR)-compile


# Phase 1 compile
#
ifeq ($(BUILD_PHASE), Stage_1)

CONFIG_OPTS = --prefix=$(TOOLS_DIR) \
    --host=$(SWE_TGT) --build=$$(../glibc-$(VERSION)/scripts/config.guess) \
    --disable-profile --enable-add-ons \
    --enable-kernel=2.6.18  --with-headers=$(TOOLS_DIR)/include \
    libc_cv_forced_unwind=yes libc_cv_c_cleanup=yes

PREPARE = yes
# Prepare
$(DIR)/: download
	tar -jxvf $(DOWNLOADS_DIR)/$(DIR)$(EXTENSION)
	mkdir -p $(COMPILE_DIR)
	cd $(COMPILE_DIR); \
	  echo "CFLAGS += -march=i686 -mtune=native" > configparms

COMPILE = yes
INSTALL = yes
BUILDTARBALL = yes

compile: configure
	@make ${JOBS} -C $(COMPILE_DIR)
	cp -v $(DIR)/iconvdata/gconv-modules $(COMPILE_DIR)/iconvdata
	mkdir -p $(TOOLS_DIR)/etc && touch $(TOOLS_DIR)/etc/ld.so.conf

# post-compile is an 'orphan': a standalone target to run the tests
post-compile: compile
	@cd $(COMPILE_DIR); make $(JOBS) -k check 2>&1 | tee glibc-check-log
	@cd $(COMPILE_DIR); grep Error glibc-check-log

install: compile
	touch $(TOOLS_DIR)/etc/ld.so.conf
	make -C $(COMPILE_DIR) install

endif


# Final compile
#

ifeq ($(BUILD_PHASE), Final)

ifeq ($(ARCH), i686)
BUILD = --build=i686-pc-linux-gnu
endif


CONFIG_OPTS = --prefix=/usr --disable-profile --enable-add-ons \
    --enable-kernel-2.6.18 --libexecdir=/usr/lib/glibc

DOWNLOAD = yes
download:
	$(DL_CMD) $(BASE_URL)/glibc-$(VERSION)$(EXTENSION)
	$(DL_CMD) $(PATCH_URL1) $(PATCH_MD51)
	$(DL_CMD) $(PATCH_URL2) $(PATCH_MD52)



PREPARE = yes
# Prepare
$(DIR)/: download
	tar -jxvf $(DOWNLOADS_DIR)/$(DIR)$(EXTENSION)
	cd $(DIR); \
	  DL=$$(readelf -l /bin/sh | sed -n 's@.*interpret.*/tools\(.*\)]$$@\1@p') \
	  sed -i "s|libs -o|libs -L/usr/lib -Wl,-dynamic-linker=$$DL -o|" \
	  scripts/test-installation.pl; \
	  unset DL
	cd $(DIR); echo "CFLAGS += -march=i686 -mtune=native" > configparms; \
	  sed -i 's|@BASH@|/bin/bash|' elf/ldd.bash.in

COMPILE = yes
INSTALL = yes

compile: configure
	@make ${JOBS} -C $(COMPILE_DIR)
	cp -v $(DIR)/iconvdata/gconv-modules $(COMPILE_DIR)/iconvdata
	mkdir -p $(PKG_ROOT)/etc && touch $(PKG_ROOT)/etc/ld.so.conf

# post-compile is an 'orphan': a standalone target to run the tests
post-compile: compile
	@cd $(COMPILE_DIR); make $(JOBS) -k check 2>&1 | tee glibc-check-log
	@cd $(COMPILE_DIR); grep Error glibc-check-log

install: compile
	@mkdir -p $(PKG_ROOT)/lib
	@mkdir -p $(PKG_ROOT)/usr/lib

ifdef BITS_64
	@(cd $(PKG_ROOT); ln -s lib lib64)
	@(cd $(PKG_ROOT)/usr; ln -s lib lib64)
endif

	touch $(PKG_ROOT)/etc/ld.so.conf
	make -C $(COMPILE_DIR) install_root=$(PKG_ROOT) install
	make $(JOBS) -C $(COMPILE_DIR) localedata/install-locales
endif

BUILDTARBALL = yes

# buildtarball
$(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .
	@tar -zxvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C / --exclude ./etc/localtime

include ../Makefile.rules
