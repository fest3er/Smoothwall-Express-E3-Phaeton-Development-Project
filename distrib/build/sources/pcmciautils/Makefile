# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = pcmciautils
VERSION = 017
EXTENSION=.tar.bz2

DIR = $(PACKAGE)-$(VERSION)

#BASE_URL = http://www.kernel.org/pub/linux/utils/kernel/pcmcia
#NOTE: As of 04/07/2012, only versions through 009 were available at the above link.
BASE_URL = http://pkgs.fedoraproject.org/repo/pkgs/$(PACKAGE)/$(PACKAGE)-$(VERSION)$(EXTENSION)/5245af28eeba57ec0606a874d44d10f7

CONFIGURE = yes
configure: $(DIR)/
	@true

include ../Makefile.rules
