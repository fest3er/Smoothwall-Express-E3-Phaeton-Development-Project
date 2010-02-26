CFLAGS=-g
all: ipt_vis

.cpp.o:
	g++ $(CFLAGS) -c -o $@ $(@:.o=.cpp)

ipt_vis: ipt_vis.o
	g++ $(CFLAGS) -o $@ $+ -lncurses -lexpat
	# strip $@

ipt_vis.o: ipt_vis.hpp ipt_vis.cpp

install: ipt_vis
	-mkdir -p $(DESTDIR)/usr/bin
	/usr/bin/install -c $+ $(DESTDIR)/usr/bin

clean:
	-rm *.o ipt_vis
