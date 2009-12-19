# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = vim
VERSION = 7.0

EXTENSION = .tar.bz2

BASE_URL = ftp://ftp.vim.org/pub/vim/unix/

DIR = vim70

INSTALL = yes

install: compile
	@(cd $(DIR); \
	install -s -D src/vim $(PKG_ROOT)/$(PKG_DIR)/bin/vim; \
	install -d runtime/ $(PKG_ROOT)/$(PKG_DIR)/share/vim; \
	cp -R runtime/* $(PKG_ROOT)/$(PKG_DIR)/share/vim; \
	install runtime/vimrc_example.vim $(PKG_ROOT)/$(PKG_DIR)/share/vim/vimrc; )
	@(cd $(PKG_ROOT)/$(PKG_DIR)/bin; ln -s vim vi);

include ../Makefile.rules
