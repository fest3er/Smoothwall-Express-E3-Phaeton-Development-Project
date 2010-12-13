###############################################################################
# beep2 version 1.2a
# Original Copyright (C) 1997 Josef Pavlik <jetset@ibm.net>
# Copyright (C) 2002 YAMAGUCHI Shingo <shingo@kip.iis.toyama-u.ac.jp>
###############################################################################

# major and minor numbers of this release
MAJOR_REL=1
MINOR_REL=2

# this should be the name of this directory (under CVS)
DISTNAME = beep2
# this should be the name of this directory (when generating the release)
RELNAME = $(DISTNAME)-$(MAJOR_REL).$(MINOR_REL)

# this is the name of the archive file
DISTFILE = $(RELNAME).tar.gz

PREFIX  = /usr
JLOCALE = ja_JP.eucJP
#JLOCALE = ja

PROGRAM = beep2
BINDIR  = $(PREFIX)/bin
MANDIR  = $(PREFIX)/man/man1
JMANDIR = $(PREFIX)/man/$(JLOCALE)/man1
#MANDIR	= $(PREFIX)/share/man/man1
#JMANDIR	= $(PREFIX)/share/man/$(JLOCALE)/man1

CC	= gcc
CFLAGS	= -c -Wall -O2 -fomit-frame-pointer
LDFLAGS	= -s
MKDIR	= mkdir -p

DIRS	= src man

# generic export
export	PREFIX
export	JLOCALE
export	BINDIR
export	MANDIR
export	JMANDIR
export	PROGRAM
export	CC
export	CFLAGS
export	LDFLAGS
export	MKDIR


all:
	@for i in $(DIRS) ; do \
		$(MAKE) -C $$i all ; \
		if [ $$? -ne 0 ]; then break ; fi ; \
	done

install:
	@for i in $(DIRS) ; do \
		$(MAKE) -C $$i install ; \
		if [ $$? -ne 0 ]; then break ; fi ; \
	done

clean:
	@for i in $(DIRS) ; do \
		$(MAKE) -C $$i clean ; \
		if [ $$? -ne 0 ]; then break ; fi ; \
	done

release:
	$(MAKE) clean
	cd .. ; tar -cf - $(RELNAME) | gzip -c9 > $(DISTFILE)
