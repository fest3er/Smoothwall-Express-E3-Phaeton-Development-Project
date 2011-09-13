ifdef GLOBALDEBUG
CXXFLAGS = -g -O0
else
CXXFLAGS = $(CFLAGS)
endif

# where to find it - relative path
IPTABLES_SRC_DIR = ../../iptables/iptables-$(IPTABLES_VER)
# where the IPT shared libs live
export IPT_LIB_DIR = $(IPT_LIBEXEC)

OBJS=
LIB_OBJS = module.o UDSocket.o pid.o setuid.o config.o ipbatch.o ipbatchc.o \
	$(IPTABLES_SRC_DIR)/iptables.o $(IPTABLES_SRC_DIR)/xshared.o

MAIN = main.cpp
LIBS = -L. -ldl -lsmoothd -lxtables -liptc
BIN = smoothd

MODULES = sysshutdown sysinstall sysim sysp3scan sysalcatel sysdhcpd \
	sysdnsproxy sysntpd syssnort syssquid syssshd sysupnpd sysadvnet \
	sysipblock sysfullfirewall sysoutgoing sysupdown sysipsec systimeset \
	sysiptables systraffic syssip sysclamav syssmoothinfo

MODULEFLAGS = $(CXXFLAGS) -shared -Wl,-soname,$@.so -o $@.so $@.cpp $(LIBS)

all: smoothd $(MODULES) smoothcom ipbatchtest

modules: $(MODULES)

libsmoothd.so : $(LIB_OBJS)
	$(CXX) -shared  -o libsmoothd.so $(CXXFLAGS) $(LIB_OBJS)
	@install -D ./libsmoothd.so /usr/lib/libsmoothd.so

smoothd: libsmoothd.so $(OBJS)
	$(CXX) -o $(BIN) $(CXXFLAGS) $(DFLAGS) $(MAIN) ipbatchc.o $(OBJS) $(LIBS)

ipbatchtest: libsmoothd.so $(OBJS)
	$(CXX) -o ipbatchtest $(CXXFLAGS) $(DFLAGS) ipbatchtest.cpp $(OBJS) $(LIBS) 

smoothcom: libsmoothd.so
	$(CXX) -o smoothcom $(CXXFLAGS) smoothcom.cpp -ldl -lxtables -liptc -lsmoothd

clean:
	rm -f $(OBJS) $(BIN) *.o *.so modules smoothcom ipbatchtest

sysshutdown : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysiptables : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysinstall : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysim : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysp3scan : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysalcatel : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysdhcpd : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysdnsproxy : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysntpd : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
syssnort : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
syssquid : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
syssshd : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysupnpd : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysadvnet : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysipblock : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysfullfirewall : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysoutgoing : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
syssmoothinfo : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysupdown : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysipsec : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
systimeset : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysiptabes : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
systraffic : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
syssip : libsmoothd.so $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysclamav : libsmoothd.so $(OBJS)
	$(CXX) $(MODULEFLAGS)

ipbatchc.o: ipbatch.h 
	$(CC) -c $(CFLAGS) -DIPTABLES_VERSION=\"$(IPTABLES_VER)\" -Wall -Wunused -I$(KERNEL_DIR)/include -I$(IPTABLES_SRC_DIR)/include/ -I. -DIPT_LIB_DIR=\"$(IPT_LIB_DIR)\"  ipbatchc.c 
