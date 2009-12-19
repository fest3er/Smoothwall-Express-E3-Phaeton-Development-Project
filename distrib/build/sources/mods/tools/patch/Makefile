include ../../../Makefile.conf

PACKAGE = patch
VERSION = 2.5.4

BASE_URL = ftp://ftp.gnu.org/gnu/patch

CONFIG_OPTS = --host=linux

INSTALL = yes

install: compile
	@make -C $(COMPILE_DIR) prefix=$(PKG_ROOT)/$(PKG_DIR) install

include ../../../Makefile.rules
