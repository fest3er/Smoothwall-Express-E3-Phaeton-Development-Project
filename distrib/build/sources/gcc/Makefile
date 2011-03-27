# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = gcc
VERSION = 4.4.3

BASE_URL = ftp://ftp.gnu.org/gnu/gcc/gcc-$(VERSION)

BASE_GMP = ftp://ftp.gnu.org/gnu/gmp
TAR_GMP = gmp-$(GMP_VER).tar.bz2

BASE_MPFR = http://www.mpfr.org/mpfr-$(MPFR_VER)
TAR_MPFR = mpfr-$(MPFR_VER).tar.bz2

COMPILE_DIR = $(DIR)-compile

DOWNLOAD = yes

download:
	@$(DL_CMD) $(BASE_GMP)/$(TAR_GMP)
	@$(DL_CMD) $(BASE_MPFR)/$(TAR_MPFR)
	@$(DL_CMD) $(BASE_URL)/$(TARBALL)$(EXTENSION) $(MD5)


# Tool Chain Build Stage 1
#
ifeq ($(BUILD_PHASE), Stage_1)

CONFIG_OPTS = --target=$(SWE_TGT) --prefix=$(TOOLS_DIR) \
	      --disable-nls --disable-shared --disable-multilib \
	      --disable-decimal-float --disable-threads \
	      --disable-libmudflap --disable-libssp \
	      --disable-libgomp --enable-languages=c



PATCH = yes
patch: $(DIR)/
	cd $(DIR); tar -xvf ../../../downloads/$(TAR_GMP); mv gmp-$(GMP_VER) gmp
	cd $(DIR); tar -xvf ../../../downloads/$(TAR_MPFR); mv mpfr-$(MPFR_VER) mpfr

COMPILE = yes
compile: configure
	@make $(JOBS) -C $(COMPILE_DIR)

INSTALL = yes
install: compile
	make -C $(COMPILE_DIR) install
	cd $(COMPILE_DIR); \
	  ln -vs libgcc.a `$(SWE_TGT)-gcc -print-libgcc-file-name \
	    | sed -e 's/libgcc/&_eh/'`
endif


# Tool Chain Build Stage 2
#
ifeq ($(BUILD_PHASE), Stage_2)
PRE_CONFIGURE = CC="$(SWE_TGT)-gcc -B$(TOOLS_DIR)/lib" \
	        AR=$(SWE_TGT)-ar RANLIB=$(SWE_TGT)-ranlib

CONFIG_OPTS = --prefix=$(TOOLS_DIR) --with-local-prefix=$(TOOLS_DIR) \
	--enable-clocale=gnu --enable-shared --enable-threads=posix  \
	--enable-__cxa_atexit --enable-languages=c,c++ \
	--disable-libstdcxx-pch --disable-multilib \
	--disable-bootstrap

PATCH_FILE1 = gcc-4.4.3-startfiles_fix-1.patch

PATCH = yes
patch: $(DIR)/
	cd $(DIR); \
	  patch -Np1 -i ../$(PATCH_FILE1); \
	  cp -v gcc/Makefile.in{,.orig}; \
	  sed 's@\./fixinc\.sh@-c true@' gcc/Makefile.in.orig > gcc/Makefile.in.tmp; \
	  sed 's/^T_CFLAGS =$$/& -fomit-frame-pointer/' gcc/Makefile.in.tmp > gcc/Makefile.in;
	cd $(DIR); \
	for file in `find gcc/config -name linux64.h \
	                          -o -name linux.h \
	                          -o -name sysv4.h`; do \
	  cp -uv $$file{,.orig}; \
	  (  sed -e 's@/lib\(64\)\?\(32\)\?/ld@$(TOOLS_DIR)&@g' \
	       -e 's@/usr@$(TOOLS_DIR)@g' "$$file.orig"; \
	     echo -e '\n#undef STANDARD_INCLUDE_DIR\n#define STANDARD_INCLUDE_DIR 0\n#define STANDARD_STARTFILE_PREFIX_1 ""\n#define STANDARD_STARTFILE_PREFIX_2 ""' \
	  ) > "$$file"; \
	  /bin/touch "$$file.orig"; \
	done
	cd $(DIR); tar -xvf ../../../downloads/$(TAR_GMP); mv gmp-$(GMP_VER) gmp
	cd $(DIR); tar -xvf ../../../downloads/$(TAR_MPFR); mv mpfr-$(MPFR_VER) mpfr

COMPILE = yes
compile: configure
	@make $(JOBS) -C $(COMPILE_DIR)
#	@make $(JOBS) -C $(COMPILE_DIR) -k check

INSTALL = yes
install:  compile
	@make -C $(COMPILE_DIR) install
	@ln -vsf gcc $(TOOLS_DIR)/bin/cc
endif


# Final Build
#
ifeq ($(BUILD_PHASE), Final)

CONFIG_OPTS =  --prefix=$(PKG_DIR) \
	--libexecdir=/usr/lib --enable-shared \
	--enable-threads=posix --enable-__cxa_atexit \
	--enable-clocale=gnu --enable-languages=c,c++ \
	--disable-bootstrap

PATCH = yes
patch: $(DIR)/
	cd $(DIR); tar -xvf ../../../downloads/$(TAR_GMP); mv gmp-$(GMP_VER) gmp
	cd $(DIR); tar -xvf ../../../downloads/$(TAR_MPFR); mv mpfr-$(MPFR_VER) mpfr
	sed -i 's/install_to_$$(INSTALL_DEST) //' $(DIR)/libiberty/Makefile.in
	sed -i 's/^XCFLAGS =$$/& -fomit-frame-pointer/' $(DIR)/gcc/Makefile.in
	sed -i 's@\./fixinc\.sh@-c true@' $(DIR)/gcc/Makefile.in


CONFIGURE = yes
configure: patch
	@(if [ ! -e $(COMPILE_DIR) ]; then mkdir $(COMPILE_DIR); fi)
ifneq ($(DIR), $(COMPILE_DIR))
	@(cd $(COMPILE_DIR); ../$(DIR)/configure $(CONFIG_OPTS))
else
	@(cd $(COMPILE_DIR); ./configure $(CONFIG_OPTS))
endif

COMPILE = yes
compile: configure
	@make $(JOBS) -C $(COMPILE_DIR)
#	@make -C $(COMPILE_DIR) -k check

INSTALL = yes
install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	(cd $(PKG_ROOT)$(PKG_DIR)/bin; ln -s gcc cc);
	(cd $(PKG_ROOT)$(PKG_DIR)/lib; ln -s $(PKG_DIR)/bin/cpp);
	(cd /bin; ln -s gcc cc);
	(cd /lib; ln -s /usr/bin/cpp);
endif

libgcc:
	tar -zcvf $(TARGET_DIR)/smoothwall-libgcc.tar.gz -C $(PKG_ROOT) $(PKG_DIR)/lib*/libgcc*

include ../Makefile.rules
