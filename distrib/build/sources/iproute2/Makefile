# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf
include ../Makefile.versions

PACKAGE = iproute2
VERSION = $(KERNEL_MMR)

BASE_URL = http://devresources.linuxfoundation.org/dev/iproute2/download
EXTENSION=.tar.bz2

#PATCH_FILE1 = iproute2-2.4.7-nomisc.patch

CONFIGURE = yes
COMPILE = yes

configure: patch
	@sed -i '/^TARGETS/s@ arpd@@g' $(COMPILE_DIR)/misc/Makefile
#	@true

compile: configure
	@make -C $(COMPILE_DIR) KERNEL_INCLUDE=/usr/include
	@ln -s $(DIR) $(PACKAGE)

include ../Makefile.rules
