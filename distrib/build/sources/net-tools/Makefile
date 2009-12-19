# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = net-tools
VERSION = 1.60

EXTENSION = .tar.bz2

BASE_URL = http://www.tazenda.demon.co.uk/phil/net-tools/

PATCH_URL1 = http://www.linuxfromscratch.org/patches/blfs/6.3/net-tools-1.60-gcc34-3.patch
PATCH_URL2 = http://www.linuxfromscratch.org/patches/blfs/6.3/net-tools-1.60-kernel_headers-2.patch
PATCH_URL3 = http://www.linuxfromscratch.org/patches/blfs/6.3/net-tools-1.60-mii_ioctl-1.patch

PATCH_FILE1 = net-tools-1.60-gcc34-3.patch
PATCH_FILE2 = net-tools-1.60-kernel_headers-2.patch
PATCH_FILE3 = net-tools-1.60-mii_ioctl-1.patch

PKG_DIR = 

CONFIGURE = yes
configure: patch
	cd $(DIR); sed -i 's@<net/if_arp.h>@<net/if_arp.h>\n#include <stdlib.h>@' \
	    lib/ec_hw.c
	@cp -f config.h $(DIR)
	@cp -f config.make $(DIR)

INSTALL = yes
install: compile
	@make -C $(DIR) BASEDIR=$(PKG_ROOT)$(PKG_DIR) install

include ../Makefile.rules
