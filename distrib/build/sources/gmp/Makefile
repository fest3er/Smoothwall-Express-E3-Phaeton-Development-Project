# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = gmp
VERSION = $(GMP_VER)

BASE_URL = ftp://ftp.gnu.org/gnu/gmp
EXTENSION = .tar.bz2

ifeq ($(ARCH), i686)
#CONFIG_OPTS = --disable-nls --host=i686-pc-linux-gnu
CONFIG_OPTS = --prefix=$(PKG_DIR) ABI=32 --enable-cxx --enable-mpbsd
endif

CONFIGURE = yes
COMPILE = yes
RECOMPILE = yes
INSTALL = yes
PACKAGEINFO = yes

configure: patch
	(if [ ! -e $(COMPILE_DIR) ]; then mkdir $(COMPILE_DIR); fi)
ifneq ($(DIR), $(COMPILE_DIR))
	(cd $(COMPILE_DIR); $(PRE_CONFIGURE) ../$(DIR)/configure --prefix=$(PKG_DIR) $(CONFIG_OPTS))
else
	(cd $(COMPILE_DIR); $(PRE_CONFIGURE) ./configure --prefix=$(PKG_DIR) $(CONFIG_OPTS))
endif

compile: configure
	@make ${JOBS} -C $(COMPILE_DIR)

recompile: 
	@make ${JOBS} -C $(COMPILE_DIR)

install: compile
	@mkdir -p $(PKG_ROOT)
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install


ifndef PACKAGEINFO
packageinfo.html:
	@echo "<li><span style='font-size:large;'>$(PACKAGE) $(VERSION)</span><br>" >>/tmp/packageinfo.html
	@cp -avR $(DOWNLOADS_DIR)/$(TARBALL)$(EXTENSION) /tmp/downloads
	@echo "Download: <a href='$(BASE_URL)$(TARBALL)$(EXTENSION)'>$(BASE_URL)$(TARBALL)$(EXTENSION)</a>" >>/tmp/packageinfo.html
	@echo "(<a href='downloads/$(TARBALL)$(EXTENSION)'>Local mirror</a>)<br>" >>/tmp/packageinfo.html
	@echo "Patches:" >>/tmp/packageinfo.html
	@echo "<ul>" >>/tmp/packageinfo.html
	@(for PATCH in $(PATCH_FILE1) $(PATCH_FILE2) $(PATCH_FILE3) $(PATCH_FILE4) $(PATCH_FILE5); do \
		echo "<li><a href='downloads/$$PATCH'>$$PATCH</a>" >>/tmp/packageinfo.html; \
		cp -avR $$PATCH /tmp/downloads; \
	done) 
	@echo "</ul>" >>/tmp/packageinfo.html

endif

all: $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz

include ../Makefile.rules
