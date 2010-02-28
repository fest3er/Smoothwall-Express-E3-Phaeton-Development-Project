# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = libpcap
VERSION = 1.0.0

BASE_URL = http://www.tcpdump.org/release/

INSTALL = yes

install: compile
ifeq ($(BUILD_PHASE), Final)
	@rm -rf $(PKG_ROOT); mkdir -p $(PKG_ROOT)
endif
	@make -C $(COMPILE_DIR) DESTDIR=$(PKG_ROOT) install
	@mkdir -p $(PKG_ROOT)/usr/include/net
	@(cd $(PKG_ROOT)/usr/include/net; ln -s ../pcap-bpf.h bpf.h)

include ../Makefile.rules
