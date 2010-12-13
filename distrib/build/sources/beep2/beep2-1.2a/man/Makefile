###############################################################################
# beep2 version 1.2a
# Original Copyright (C) 1997 Josef Pavlik <jetset@ibm.net>
# Copyright (C) 2002 YAMAGUCHI Shingo <shingo@kip.iis.toyama-u.ac.jp>
###############################################################################

dummy:
	@echo '*** This directory is not the top directory.'
	@echo '*** Please up to parent directory.'

all:
	gzip -c9 en/beep2.1 > en/beep2.1.gz
	gzip -c9 ja/beep2.1 > ja/beep2.1.gz

install:	all
	$(MKDIR) $(MANDIR)
	install -m 0644 en/beep2.1.gz $(MANDIR)
	$(MKDIR) $(JMANDIR)
	install -m 0644 ja/beep2.1.gz $(JMANDIR)

clean:
	rm -f */*~
	rm -f en/beep2.1.gz
	rm -f ja/beep2.1.gz
