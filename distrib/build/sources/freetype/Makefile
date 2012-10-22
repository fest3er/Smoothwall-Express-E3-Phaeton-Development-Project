include ../Makefile.conf

PACKAGE = freetype
#VERSION = 2.1.10
VERSION = 2.4.10

#BASE_URL = http://people.ee.ethz.ch/~oetiker/webtools/rrdtool/pub/libs/
BASE_URL = http://download.savannah.gnu.org/releases/freetype/

EXTENSION = .tar.bz2

CONFIG_OPTS = --prefix=$(PKG_DIR)

CONFIGURE = yes

configure: patch
	#(cd $(DIR)/builds/unix; ./configure $(CONFIG_OPTS))
	(cd $(DIR); ./configure $(CONFIG_OPTS))

include ../Makefile.rules

