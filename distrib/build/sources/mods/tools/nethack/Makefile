include ../../../Makefile.conf

PACKAGE = nethack
VERSION = 343-src

BASE_URL = http://www.nethack.de/download/

EXTENSION = .tgz

DIR = $(PACKAGE)-3.4.3

CONFIGURE = yes

configure: patch
	@(cd $(DIR)/sys/unix; sh setup.sh)	

include ../../../Makefile.rules
