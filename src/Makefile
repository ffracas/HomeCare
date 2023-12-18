CC = g++
CFLAGS = -std=c++11 -Wall -ljsoncpp
DFLAGS = -std=c++11 -Wall -g
LIBDIR = ../include
	
hc: main.cpp
	rm -f ./HomeCare
	$(CC) $(CFLAGS) main.cpp $(LIBDIR)/readjson/hcsolver.cpp $(LIBDIR)/patient/patient.cpp $(LIBDIR)/service/service.cpp $(LIBDIR)/caregiver/caregiver.cpp $(LIBDIR)/depot/depot.cpp -o HomeCare

main: main.cpp
	rm -f ./HomeCare
	$(CC) $(CFLAGS) main.cpp $(LIBDIR)/problemdef/problemdef.cpp $(LIBDIR)/params/params.cpp $(LIBDIR)/route/route.cpp $(LIBDIR)/arc/arc.cpp $(LIBDIR)/node/node.cpp -o HomeCare

debug: main.cpp
	rm -f ./HomeCare
	$(CC) $(DFLAGS) main.cpp $(LIBDIR)/problemdef/problemdef.cpp $(LIBDIR)/params/params.cpp $(LIBDIR)/route/route.cpp $(LIBDIR)/arc/arc.cpp $(LIBDIR)/node/node.cpp -o HomeCare

