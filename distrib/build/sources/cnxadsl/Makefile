# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = cnxadsl
VERSION = 2.6-1.5

BASE_URL = http://patrick.spacesurfer.com/adsl/

TARBALL = CnxADSL-6.1.2.007-PIM-$(VERSION)

EXTENSION = .tar.bz2

CONFIGURE = yes
COMPILE = yes
INSTALL = yes

configure: patch
	@true

compile: configure
	@make -C $(DIR)/DownLoadApp

install: compile
	@(for I in cnxadslload cnxadslstatus; do \
		install -D $(DIR)/DownLoadApp/$$I $(PKG_ROOT)/$(PKG_DIR)/sbin/$$I; \
	done)
	@(for I in CnxTgF.hex; do \
		install -D $(DIR)/DownLoadApp/$$I $(PKG_ROOT)/$(PKG_DIR)/etc/cnxadsl/$$I; \
	done)
	
include ../Makefile.rules
