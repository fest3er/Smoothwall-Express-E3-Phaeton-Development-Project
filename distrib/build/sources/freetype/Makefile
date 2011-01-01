include ../Makefile.conf

PACKAGE = freetype
VERSION = 2.1.10

#BASE_URL = http://people.ee.ethz.ch/~oetiker/webtools/rrdtool/pub/libs/
BASE_URL = http://download.savannah.gnu.org/releases/freetype/

EXTENSION = .tar.bz2

CONFIGURE = yes

configure: patch
	@(cd $(DIR)/builds/unix; ./configure --prefix=/$(PKG_DIR) $(CONFIG_OPTS))

include ../Makefile.rules

