# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = daq
VERSION = 0.2

DIR = $(PACKAGE)-$(VERSION)

SNORT_URL = http://www.snort.org/downloads/263


DOWNLOAD = yes
download:
	@if [ ! -e ../../downloads/$(TARBALL)$(EXTENSION).done ]; then \
	  wget --no-check-certificate \
	       -O ../../downloads/$(TARBALL)$(EXTENSION) $(SNORT_URL); \
	  if [ $? -eq 0 ]; then \
	    touch ../../downloads/$(TARBALL)$(EXTENSION).done; \
	  fi \
	fi


include ../Makefile.rules
