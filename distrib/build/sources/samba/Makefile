# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = samba
VERSION = 3.6.7

BASE_URL = http://www.samba.org/samba/ftp/stable/

COMPILE_DIR = $(DIR)/source3

CONFIGURE = yes

configure: $(DIR)/
	@( cd $(COMPILE_DIR); ./configure --prefix=/usr; ./config.status; )

COMPILE = yes

compile: configure
	@make -C $(COMPILE_DIR)

include ../Makefile.rules
