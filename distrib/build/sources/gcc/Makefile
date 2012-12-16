# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = gcc
VERSION = 4.7.2

BASE_URL = ftp://ftp.gnu.org/gnu/gcc/gcc-$(VERSION)
EXTENSION = .tar.bz2

BASE_MPC = http://www.multiprecision.org/mpc/download
TAR_MPC = mpc-$(MPC_VER).tar.gz

BASE_GMP = ftp://ftp.gnu.org/gnu/gmp
TAR_GMP = gmp-$(GMP_VER).tar.bz2

BASE_MPFR = http://www.mpfr.org/mpfr-$(MPFR_VER)
TAR_MPFR = mpfr-$(MPFR_VER).tar.bz2

COMPILE_DIR = $(DIR)-compile

DOWNLOAD = yes

download:
	@$(DL_CMD) $(BASE_GMP)/$(TAR_GMP)
	@$(DL_CMD) $(BASE_MPFR)/$(TAR_MPFR)
	@$(DL_CMD) $(BASE_MPC)/$(TAR_MPC)
	@$(DL_CMD) $(BASE_URL)/$(TARBALL)$(EXTENSION) $(MD5)


# Tool Chain Build Stage 1
#
ifeq ($(BUILD_PHASE), Stage_1)

CONFIG_OPTS = --target=$(SWE_TGT) \
	      --prefix=$(TOOLS_DIR) \
	      --with-sysroot=$(ROOT) \
	      --with-newlib \
	      --without-headers \
	      --with-local-prefix=$(TOOLS_DIR) \
	      --with-native-system-header-dir=$(TOOLS_DIR)/include \
	      --disable-nls \
	      --disable-shared \
	      --disable-decimal-float \
	      --disable-threads \
	      --disable-libmudflap \
	      --disable-libssp \
	      --disable-libgomp \
	      --disable-libquadmath \
	      --enable-languages=c \
	      --with-mpfr-include=$(SOURCES_DIR)/$(PACKAGE)/$(DIR)/mpfr/src \
	      --with-mpfr-lib=$(SOURCES_DIR)/$(PACKAGE)/$(COMPILE_DIR)/mpfr/src/.libs

PATCH = yes
patch: $(DIR)/
	cd $(DIR); tar -xvf ../../../downloads/$(TAR_GMP); mv gmp-$(GMP_VER) gmp
	cd $(DIR); tar -xvf ../../../downloads/$(TAR_MPFR); mv mpfr-$(MPFR_VER) mpfr
	cd $(DIR); tar -xvf ../../../downloads/$(TAR_MPC); mv mpc-$(MPC_VER) mpc
	cd $(DIR); \
	  for file in `find gcc/config -name linux64.h -o -name linux.h -o -name sysv4.h`; \
	  do \
  	    cp -uv $$file{,.orig}; \
  	    sed -e 's@/lib\(64\)\?\(32\)\?/ld@$(TOOLS_DIR)&@g' \
      	        -e 's@/usr@$(TOOLS_DIR)@g' $$file.orig > $$file; \
  	    echo '#undef STANDARD_STARTFILE_PREFIX_1' >> $$file; \
  	    echo '#undef STANDARD_STARTFILE_PREFIX_2' >> $$file; \
  	    echo '#define STANDARD_STARTFILE_PREFIX_1 "$(TOOLS_DIR)/lib/"' >> $$file; \
  	    echo '#define STANDARD_STARTFILE_PREFIX_2 ""' >> $$file; \
  	    touch $$file.orig; \
	  done
	cd $(DIR); sed -i '/k prot/agcc_cv_libc_provides_ssp=yes' gcc/configure

COMPILE = yes
compile: configure
	$(MAKE) $(JOBS) -C $(COMPILE_DIR)

INSTALL = yes
install: compile
	$(MAKE) -C $(COMPILE_DIR) install
	cd $(COMPILE_DIR); \
	  ln -vs libgcc.a `$(SWE_TGT)-gcc -print-libgcc-file-name \
	    | sed -e 's/libgcc/&_eh/'`
endif


# Tool Chain Build Stage 2
#
ifeq ($(BUILD_PHASE), Stage_2)
PATCH = yes
COMPILE = yes
INSTALL = yes

patch: $(DIR)/
	cd $(DIR); cat gcc/limitx.h gcc/glimits.h gcc/limity.h > \
	  `dirname $$($$SWE_TGT-gcc -print-libgcc-file-name)`/include-fixed/limits.h
ifeq ($(BITS_32), yes)
	cd $(DIR); cp -v gcc/Makefile.in{,.tmp}; \
	  sed -e 's/^T_CFLAGS =$$/& -fomit-frame-pointer/' gcc/Makefile.in.tmp \
	    > gcc/Makefile.in
endif
	cd $(DIR); \
	for file in `find gcc/config -name linux64.h \
	                          -o -name linux.h \
	                          -o -name sysv4.h`; do \
	  cp -uv $$file{,.orig}; \
	  (  sed -e 's@/lib\(64\)\?\(32\)\?/ld@$(TOOLS_DIR)&@g' \
		 -e 's@/usr@$(TOOLS_DIR)@g' "$$file.orig"; \
	     echo '#undef STANDARD_STARTFILE_PREFIX_1'; \
	     echo '#undef STANDARD_STARTFILE_PREFIX_2'; \
	     echo '#define STANDARD_STARTFILE_PREFIX_1 "$(TOOLS_DIR)/lib/"'; \
	     echo '#define STANDARD_STARTFILE_PREFIX_2 ""'; \
	  ) > "$$file"; \
	  /bin/touch "$$file.orig"; \
	done
	cd $(DIR); tar -xvf ../../../downloads/$(TAR_GMP); mv gmp-$(GMP_VER) gmp
	cd $(DIR); tar -xvf ../../../downloads/$(TAR_MPFR); mv mpfr-$(MPFR_VER) mpfr
	cd $(DIR); tar -xvf ../../../downloads/$(TAR_MPC); mv mpc-$(MPC_VER) mpc

PRE_CONFIGURE = CC=$(SWE_TGT)-gcc \
	        AR=$(SWE_TGT)-ar RANLIB=$(SWE_TGT)-ranlib

CONFIG_OPTS = \
	      --prefix=$(TOOLS_DIR)       \
	      --with-local-prefix=$(TOOLS_DIR) \
	      --with-native-system-header-dir=$(TOOLS_DIR)/include \
	      --enable-clocale=gnu        \
	      --enable-shared             \
	      --enable-threads=posix      \
	      --enable-__cxa_atexit       \
	      --enable-languages=c,c++    \
	      --disable-libstdcxx-pch     \
	      --disable-bootstrap         \
	      --disable-libgomp           \
	      --with-mpfr-include=$(SOURCES_DIR)/$(PACKAGE)/$(DIR)/mpfr/src \
	      --with-mpfr-lib=$(SOURCES_DIR)/$(PACKAGE)/$(COMPILE_DIR)/mpfr/src/.libs
#	      --disable-multilib

compile: configure
	make $(JOBS) -C $(COMPILE_DIR)

install:  compile
	make -C $(COMPILE_DIR) install
	ln -vs gcc $(TOOLS_DIR)/bin/cc
endif


# Final Build
#
ifeq ($(BUILD_PHASE), Final)
PATCH = yes
CONFIGURE = yes
COMPILE = yes

patch: $(DIR)/
	cd $(DIR); sed -i 's/install_to_$$(INSTALL_DEST) //' libiberty/Makefile.in
ifeq ($(BITS_32), yes)
	cd $(DIR); cp -v gcc/Makefile.in{,.tmp}; \
	  sed -e 's/^T_CFLAGS =$$/& -fomit-frame-pointer/' gcc/Makefile.in.tmp \
	    > gcc/Makefile.in
endif
	cd $(DIR); sed -i -e '/autogen/d' -e '/check.sh/d' fixincludes/Makefile.in

CONFIG_OPTS =  \
		--prefix=$(PKG_DIR) \
		--libexecdir=/usr/lib    \
		--enable-shared          \
		--enable-threads=posix   \
		--enable-__cxa_atexit    \
		--enable-clocale=gnu     \
		--enable-languages=c,c++ \
		--disable-bootstrap      \
		--with-system-zlib
#		--disable-multilib

configure: patch
	@(if [ ! -e $(COMPILE_DIR) ]; then mkdir $(COMPILE_DIR); fi)
ifneq ($(DIR), $(COMPILE_DIR))
	@(cd $(COMPILE_DIR); ../$(DIR)/configure $(CONFIG_OPTS))
else
	@(cd $(COMPILE_DIR); ./configure $(CONFIG_OPTS))
endif

compile: configure
	@make $(JOBS) -C $(COMPILE_DIR)
#	@make -C $(COMPILE_DIR) -k check

INSTALL = yes
install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	(cd $(PKG_ROOT)$(PKG_DIR)/bin; ln -s gcc cc);
	(cd $(PKG_ROOT)$(PKG_DIR)/lib; ln -s $(PKG_DIR)/bin/cpp);
	ln -svf /usr/bin/cpp /lib
	ln -svf gcc /usr/bin/cc
endif

libgcc:
	tar -zcvf $(TARGET_DIR)/smoothwall-libgcc.tar.gz -C $(PKG_ROOT) $(PKG_DIR)/lib*/libgcc*

include ../Makefile.rules
