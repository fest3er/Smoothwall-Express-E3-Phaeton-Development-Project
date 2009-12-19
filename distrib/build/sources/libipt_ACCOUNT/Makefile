# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = libipt_ACCOUNT
VERSION =  1.3

BASE_URL = http://www.intra2net.com/de/produkte/opensource/ipt_account/

PATCH_FILE1 = ipt_ACCOUNT_cl.c.patch

CONFIGURE = yes
COMPILE = yes

configure: patch
	@(if [ ! -e $(COMPILE_DIR) ]; then mkdir $(COMPILE_DIR); fi)
ifneq ($(DIR), $(COMPILE_DIR))
	@(cd $(DIR); autoreconf -fv)
	@(cd $(COMPILE_DIR); $(PRE_CONFIGURE) ../$(DIR)/configure --prefix=$(PKG_DIR) $(CONFIG_OPTS))
else
	@(cd $(COMPILE_DIR); autoreconf -fv)
	@(cd $(COMPILE_DIR); $(PRE_CONFIGURE) ./configure --prefix=$(PKG_DIR) $(CONFIG_OPTS))
endif

compile: configure
	@make -C $(DIR) CFLAGS="$(CFLAGS) -I$(SOURCES_DIR)/kernel-phaeton/linux/include"

include ../Makefile.rules
