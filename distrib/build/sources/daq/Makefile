# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = daq

# Deal with snort: get the latest version number dynamically. :)
#
VERSION := $(shell ../../get-snort-ver.sh -v $(PACKAGE))
URL := $(shell ../../get-snort-ver.sh -u $(PACKAGE))

BASE_URL = http://dl.snort.org

DIR = $(PACKAGE)-$(VERSION)

PATCH_FILE1 = snort-pidfix.patch

# You're looking in here because the build system failed to find
# the snort source package or it failed to build.
#
# This makefile should always get the latest version of snort, whatever it is.
# However, the build might fail if the pidfile fix patch is no longer correct.

DOWNLOAD = yes
download:
	@if [ ! -e ../../downloads/$(TARBALL)$(EXTENSION).done ]; then \
	  wget --no-check-certificate \
	       -O ../../downloads/$(TARBALL)$(EXTENSION) $(BASE_URL)$(URL); \
	  if [ $$? -eq 0 ]; then \
	    touch ../../downloads/$(TARBALL)$(EXTENSION).done; \
	  fi; \
	fi


include ../Makefile.rules
