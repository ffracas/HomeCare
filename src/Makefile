CC = g++
CFLAGS = -std=c++11 -Wall -ljsoncpp
DFLAGS = -std=c++11 -Wall -g
LIBDIR = ../include
LDIRVR = ../include/vr
	
hc: main.cpp
	rm -f ./HomeCare
	$(CC) $(CFLAGS) main.cpp $(LIBDIR)/structures/node.cpp $(LIBDIR)/structures/arc.cpp $(LIBDIR)/structures/route.cpp $(LIBDIR)/hc/hcsolution.cpp $(LIBDIR)/readjson/hcdata.cpp $(LIBDIR)/patient/patient.cpp $(LIBDIR)/service/service.cpp $(LIBDIR)/caregiver/caregiver.cpp $(LIBDIR)/depot/depot.cpp -o HomeCare

main: main.cpp
	rm -f ./HomeCare
	$(CC) $(CFLAGS) main.cpp $(LDIRVR)/problemdef/problemdef.cpp $(LDIRVR)/params/params.cpp $(LDIRVR)/route/route.cpp $(LDIRVR)/arc/arc.cpp $(LDIRVR)/node/node.cpp -o HomeCare

debug: main.cpp
	rm -f ./HomeCare
	$(CC) $(DFLAGS) main.cpp $(LDIRVR)/problemdef/problemdef.cpp $(LDIRVR)/params/params.cpp $(LDIRVR)/route/route.cpp $(LDIRVR)/arc/arc.cpp $(LDIRVR)/node/node.cpp -o HomeCare

