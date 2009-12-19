# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = groff
VERSION = 1.18.1.4

BASE_URL = ftp://ftp.gnu.org/gnu/groff

PRE_CONFIGURE = PAGE=letter

CONFIG_OPTS = --prefix=$(PKG_DIR) --enable-multibyte

PATCH_URL1 = http://www.linuxfromscratch.org/patches/lfs/6.4/groff-1.18.1.4-debian_fixes-1.patch
PATCH_FILE1 = groff-1.18.1.4-debian_fixes-1.patch

PATCH = yes
patch: $(DIR)/
	cp $(DOWNLOADS_DIR)/`basename $(PATCH_URL1)` ./
	cd $(DIR); \
	FILENAME=../$(PATCH_FILE1); \
	if echo $$FILENAME | grep -q \.gz$$; then \
	  zcat $$FILENAME | patch -p1; \
	elif echo $$FILENAME | grep -q \.bz2$$; then \
	  bzcat $$FILENAME | patch -p1; \
	else \
	  cat $$FILENAME | patch -p1; \
	fi
	cd $(DIR); \
	sed -i -e 's/2010/002D/' -e 's/2212/002D/' \
	       -e 's/2018/0060/' -e 's/2019/0027/' font/devutf8/R.proto

INSTALL = yes
install: compile
	@mkdir -p $(PKG_ROOT)$(PKG_DIR)
	@make -C $(COMPILE_DIR) prefix=$(PKG_ROOT)$(PKG_DIR) \
	    docdir=${PKG_ROOT}$(PKG_DIR)/share/doc/groff-1.18.1.4 install

include ../Makefile.rules
