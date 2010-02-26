# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../../../Makefile.versions

CC=gcc 

LD=ld
LDFLAGS=-i

COMPILE=$(CC) -c $(INCLUDE) $(CFLAGS)

LINK=$(LD) $(LIBRARY) $(LDFLAGS)

all : programs

programs : makelangs libsmooth libsmooth.o 

clean :
	-rm -f *.o libsmooth.so core langs.h

makelangs :
	-cp -f langs.h.temp langs.h
	-cat english.c | ./makelangs.pl >> langs.h

######

OBJS=english.o varval.o main.o netstuff.o

libsmooth: $(OBJS)
	$(CC)  -fPIC -Wall -shared -Wl,-soname,$@.so -o $@.so $(OBJS) $(LIBS)
libsmooth.o: $(OBJS)
	$(LINK) $(OBJS) -o $@ $(LIBS)

%.o : %.c
	$(COMPILE) $< -o $@
		
english.o : english.c
#german.o : german.c
#dutch.o : dutch.c
#danish.o : danish.c

varval.o : varval.c
main.o : main.c
netstuff.o : netstuff.c


