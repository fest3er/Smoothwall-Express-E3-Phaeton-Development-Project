# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = binutils
VERSION = 2.19.1

BASE_URL = http://ftp.gnu.org/gnu/binutils
EXTENSION = .tar.gz

BASE_GMP = ftp://ftp.gnu.org/gnu/gmp
TAR_GMP = gmp-$(GMP_VER).tar.bz2

BASE_MPFR = http://www.mpfr.org/mpfr-current
TAR_MPFR = mpfr-$(MPFR_VER).tar.bz2

COMPILE_DIR = $(DIR)-compile

#PATCH_FILE1 = binutils-2.18-configure-1.patch
#PATCH_FILE2 = binutils-2.18-GCC43-1.patch

# Tool Chain Build Stage_1
#
ifeq ($(BUILD_PHASE), Stage_1)

PRE_CONFIGURE = CC="gcc -B/usr/bin"

CONFIG_OPTS = --prefix=$(TOOLS_DIR) --disable-nls --disable-werror

COMPILE = yes
compile: configure
	@make -C $(COMPILE_DIR) configure-host
	@make $(JOBS) -C $(COMPILE_DIR) LDFLAGS="-all-static"

INSTALL = yes
install: compile
	@mkdir -p $(TOOLS_DIR)/lib
ifdef BITS_64
	(cd $(TOOLS_DIR); ln -s lib lib64)
endif
	make -C $(COMPILE_DIR) install
	
	@ # Get ready for first adjusting
	make -C $(COMPILE_DIR)/ld clean
	make $(JOBS) -C $(COMPILE_DIR)/ld LDFLAGS="-all-static" LIB_PATH=$(TOOLS_DIR)/lib
	cp -v $(COMPILE_DIR)/ld/ld-new $(TOOLS_DIR)/bin

endif # Stage 1


# Tool Chain Build Stage_2
#
ifeq ($(BUILD_PHASE), Stage_2)

CONFIG_OPTS = --prefix=$(TOOLS_DIR) --disable-nls \
	      --with-lib-path=$(TOOLS_DIR)/lib

INSTALL = yes
install: compile
	@make -C $(COMPILE_DIR) install
	
	@ # Get ready for second adjusting
	make -C $(COMPILE_DIR)/ld clean
	make $(JOBS) -C $(COMPILE_DIR)/ld LIB_PATH=/usr/lib:/lib
	cp -v  $(COMPILE_DIR)/ld/ld-new $(TOOLS_DIR)/bin
endif # Stage 2


# SWE Final Build Stage
#
ifeq ($(BUILD_PHASE), Final)
CONFIG_OPTS = --prefix=$(PKG_ROOT)$(PKG_DIR) --enable-shared

DOWNLOAD = yes
download:
	@$(DL_CMD) $(BASE_GMP)/$(TAR_GMP)
	@$(DL_CMD) $(BASE_MPFR)/$(TAR_MPFR)
	@$(DL_CMD) $(BASE_URL)/$(TARBALL)$(EXTENSION) $(MD5)

PATCH = yes
patch: $(DIR)/
	cd $(DIR); tar jxfv ../../../downloads/$(TAR_GMP); mv gmp-$(GMP_VER) gmp
	cd $(DIR); tar jxfv ../../../downloads/$(TAR_MPFR); mv mpfr-$(MPFR_VER) mpfr
	rm -fv $(DIR)/etc/standards.info
	sed -i.bak '/^INFO/s/standards.info //' $(DIR)/etc/Makefile.in


COMPILE = yes
compile: configure
	@make ${JOBS} -C $(COMPILE_DIR) tooldir=$(PKG_ROOT)$(PKG_DIR)
	
INSTALL = yes
install: compile
	@mkdir -p $(PKG_ROOT)$(PKG_DIR)/include
	@make ${JOBS} -C $(COMPILE_DIR) tooldir=$(PKG_ROOT)$(PKG_DIR) install
	@cp -v $(DIR)/include/libiberty.h $(PKG_ROOT)$(PKG_DIR)/include

adjust:
	# Return specs file to 'au natural'
	GCC_LIBNAM=`gcc -print-libgcc-file-name`; \
	  GCC_LIBNAMDIR=`dirname $${GCC_LIBNAM}`; \
	  rm -fv $${GCC_LIBNAMDIR}/specs

	make -C $(COMPILE_DIR)/ld INSTALL=$(TOOLS_DIR)/bin/install install
endif # Final

include ../Makefile.rules
