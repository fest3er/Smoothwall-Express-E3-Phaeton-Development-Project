# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

# $Id: Makefile 218 2005-09-14 10:01:50Z lawrence $
# $Symbol$

VERSION =		1.4
RCSSYMBOL =		R1_4

SRCS_C =		ibod.c
SRCS_H =		ibod.h
SRCS_MISC =		Makefile ibod.cf
SRCS_MAN =		ibod.1 ibod.cf.4
SRCS_ALL =		$(SRCS_C) $(SRCS_H) $(SRCS_MISC) $(SRCS_MAN)

all:			ibod

freeze:
			rcs -n$(RCSSYMBOL): $(SRCS_ALL)

dist-src:
			@set -x; \
			tar cvf tmp.tar $(SRCS_ALL); \
			gzip tmp.tar; \
			mv tmp.tar.gz ibod-$(VERSION)-src.tar.gz
			
dist-bin:		all
			@set -x; \
			tar cvf tmp.tar ibod $(SRCS_MAN) ibod.cf; \
			gzip tmp.tar; \
			mv tmp.tar.gz ibod-$(VERSION)-bin.tar.gz
