# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = vim
MAJ = 7
MIN = 3
VERSION = $(MAJ).$(MIN)

BASE_URL = ftp://ftp.vim.org/pub/vim/unix/
EXTENSION = .tar.bz2

EXTENSION = .tar.bz2

DIR = $(PACKAGE)$(MAJ)$(MIN)

CONFIG_OPTS = --prefix=/usr --with-tlib=tinfo

INSTALL = yes

install: compile
	mkdir -p $(PKG_ROOT)
	+cd $(DIR); \
	  $(MAKE) DESTDIR=$(PKG_ROOT) install; \
	cd $(PKG_ROOT)/usr/bin; ln -s vim vi;
	cd $(PKG_ROOT)/usr/share/man; \
	  for L in {,*/}/man1/vim.1; do \
	    ln -sv vim.1 `dirname $$L`/vi.1; \
	  done
	cd $(PKG_ROOT)/usr/share; \
	  mkdir doc; \
	  cd doc; \
	  ln -sv ../vim/vim73/doc vim-$(MAJ).$(MIN)

include ../Makefile.rules
