# Makefile for compiling most perl modules
# (c) 2009 Steve McNeill
#
# $Id: Makefile.perl 338 2009-07-26 15:34:01Z steve@domesticsecurity.com.au $
#

INSTALL = yes
CONFIGURE = yes

configure:  $(DIR)/
	@(cd $(DIR); export PERL5LIB="$(MOD_DIR)/lib/perl5"; $(PRE_CONFIGURE) echo 'y' | perl Makefile.PL $(PERL_CONFIG_OPTS) INSTALLBASE=$(MOD_DIR) )

install: compile
	-mkdir $(PKG_ROOT)
	@make -C $(DIR)
ifndef NOPERLTEST	
	@export PERL5LIB="$(MOD_DIR)/lib/perl5"; make -C $(DIR) test
endif
	@make -C $(DIR) DESTDIR=$(PKG_ROOT) install

include ../../Makefile.package
include ../../../Makefile.rules

# vim: syn=make
