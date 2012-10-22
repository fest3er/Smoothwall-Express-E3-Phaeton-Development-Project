# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../../../Makefile.conf

PACKAGE = Net-SSLeay
VERSION = 1.48

BASE_URL = http://search.cpan.org/CPAN/authors/id/M/MI/MIKEM/

CONFIGURE = yes

configure: $(DIR)/
	@+cd $(DIR); echo 'y' | perl Makefile.PL

include ../../../Makefile.rules
