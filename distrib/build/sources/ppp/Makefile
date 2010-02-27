# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = ppp
VERSION = 2.4.4

BASE_URL = http://samba.org/ftp/ppp/

PATCH_FILE1 = pppstats-Makefile.patch

COMPILE = yes
INSTALL = yes

COMPILE_FLAGS = -DHAVE_PATHS_H -DIPX_CHANGE -DHAVE_MMAP -D_PATH_VARRUN=\\\"/var/run\\\"

compile: configure
	cat $(DIR)/pppstats/Makefile.linux
	@make -C $(DIR) COMPILE_FLAGS="$(COMPILE_FLAGS)"

install: compile
	-mkdir $(PKG_ROOT)
	@make -C $(DIR) DESTDIR=$(PKG_ROOT)$(PKG_DIR) install
	
include ../Makefile.rules
