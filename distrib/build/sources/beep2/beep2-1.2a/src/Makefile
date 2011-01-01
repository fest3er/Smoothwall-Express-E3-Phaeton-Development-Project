###############################################################################
# beep2 version 1.2a
# Original Copyright (C) 1997 Josef Pavlik <jetset@ibm.net>
# Copyright (C) 2002 YAMAGUCHI Shingo <shingo@kip.iis.toyama-u.ac.jp>
###############################################################################

OBJS	= beep.o \
          log.o \
          main.o \
          parse_f.o \
          parse_s.o \
          s2f.o \
          usage.o \
          version.o

dummy:
	@echo '*** This directory is not the top directory.'
	@echo '*** Please up to parent directory.'

all:		beep	

install:	all
	$(MKDIR) $(BINDIR)
	install -m 4755 $(PROGRAM) $(BINDIR)

beep:   beep.h $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(PROGRAM)

.c.o:
	$(CC) $(CFLAGS) $*.c

clean:
	rm -f *.o
	rm -f *core
	rm -f *~
	rm -f $(PROGRAM)
