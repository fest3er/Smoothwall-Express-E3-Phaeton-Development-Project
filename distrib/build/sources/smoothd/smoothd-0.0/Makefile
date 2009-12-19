
CXXFLAGS = $(CFLAGS)
# where to find it - relative path
IPTABLES_SRC_DIR = ../../iptables/iptables-$(IPTABLES_VER)
# where the IPT shared libs live
IPT_LIB_DIR = /usr/lib/iptables

OBJS=
LIB_OBJS = module.o UDSocket.o pid.o setuid.o config.o ipbatch.o ipbatchc.o \
	$(IPTABLES_SRC_DIR)/iptables.o \
	$(IPTABLES_SRC_DIR)/libiptc/libiptc.a
MAIN = main.cpp
LIBS = -ldl /usr/lib/libsmoothd.so  -lxtables
BIN = smoothd

MODULES = sysshutdown sysinstall sysim sysp3scan sysalcatel sysdhcpd \
	sysdnsproxy sysntpd syssnort syssquid syssshd sysupnpd sysadvnet \
	sysipblock sysxtaccess sysupdown sysipsec systimeset sysiptables \
	systraffic syssip sysclamav systimedaccess

MODULEFLAGS = $(CXXFLAGS) -shared -Wl,-soname,$@.so -o $@.so $@.cpp $(LIBS)

all: libsmoothd smoothd $(MODULES) smoothcom ipbatchtest

modules: $(MODULES)

libsmoothd : $(LIB_OBJS)
	$(CXX) -shared  -o libsmoothd.so $(LIB_OBJS)
	@install -D ./libsmoothd.so /usr/lib/libsmoothd.so

smoothd: $(OBJS) 
	$(CXX) -o $(BIN) $(DFLAGS) $(MAIN) $(OBJS) $(LIBS)

ipbatchtest: $(OBJS) 
	$(CXX) -g -o ipbatchtest $(DFLAGS) ipbatchtest.cpp $(OBJS) $(LIBS) 

smoothcom:
	$(CXX) -o smoothcom $(CXXFLAGS) smoothcom.cpp -ldl -lxtables /usr/lib/libsmoothd.so 

clean:
	rm -f $(OBJS) $(BIN) *.o *.so modules smoothcom

sysshutdown : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysiptables : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysinstall : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysim : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysp3scan : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysalcatel : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysdhcpd : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysdnsproxy : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysntpd : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
syssnort : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
syssquid : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
syssshd : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysupnpd : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysadvnet : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysipblock : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysxtaccess : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysupdown : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysipsec : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
systimeset : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysiptabes : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
systraffic : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
syssip : $(OBJS)  
	$(CXX) $(MODULEFLAGS)
sysclamav : $(OBJS)
	$(CXX) $(MODULEFLAGS)
systimedaccess : $(OBJS)
	$(CXX) $(MODULEFLAGS)

ipbatchc.o: ipbatch.h 
	$(CC) -c $(CFLAGS) -DIPTABLES_VERSION=\"$(IPTABLES_VER)\" -Wall -Wunused -I$(KERNEL_DIR)/include -I$(IPTABLES_SRC_DIR)/include/ -I. -DIPT_LIB_DIR=\"$(IPT_LIB_DIR)\"  ipbatchc.c 
