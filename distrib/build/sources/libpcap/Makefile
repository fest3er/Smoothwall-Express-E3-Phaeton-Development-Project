# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = libpcap
VERSION = 1.0.0

BASE_URL = http://www.tcpdump.org/release/

CONFIGURE = yes
INSTALL = yes

configure: patch
	(if [ ! -e $(COMPILE_DIR) ]; then mkdir $(COMPILE_DIR); fi)
ifneq ($(DIR), $(COMPILE_DIR))
	(cd $(COMPILE_DIR); $(PRE_CONFIGURE) ../$(DIR)/configure --prefix=$(PKG_DIR) $(CONFIG_OPTS))
else
	(cd $(COMPILE_DIR); $(PRE_CONFIGURE) ./configure --prefix=$(PKG_DIR) $(CONFIG_OPTS))
endif
	sed -i 's@INSTALL = /usr/bin/install -c@INSTALL = /usr/bin/install -c -D@' $(DIR)/Makefile

install: compile
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(PKG_ROOT); mkdir -p $(PKG_ROOT)
endif
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	@mkdir -p $(PKG_ROOT)/usr/include/net
	@(cd $(PKG_ROOT)/usr/include/net; ln -s ../pcap-bpf.h bpf.h)

include ../Makefile.rules
