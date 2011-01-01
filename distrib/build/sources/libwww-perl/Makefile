# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = libwww-perl
VERSION = 5.837

#BASE_URL = http://cpan.m.flirble.org/authors/id/G/GA/GAAS/
BASE_URL = http://search.cpan.org/CPAN/authors/id/G/GA/GAAS/


CONFIGURE = yes

configure: $(DIR)/
	@(cd $(DIR); perl Makefile.PL -n ) 

include ../Makefile.rules
