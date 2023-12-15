CC = g++
CFLAGS = -std=c++11 -Wall -ljsoncpp
DFLAGS = -std=c++11 -Wall -g
	

main: main.cpp
	rm -f ./HomeCare
	$(CC) $(CFLAGS) main.cpp ../include/problemdef/problemdef.cpp ../include/params/params.cpp ../include/route/route.cpp ../include/arc/arc.cpp ../include/node/node.cpp -o HomeCare

debug: main.cpp
	rm -f ./HomeCare
	$(CC) $(DFLAGS) main.cpp ../include/problemdef/problemdef.cpp ../include/params/params.cpp ../include/route/route.cpp ../include/arc/arc.cpp ../include/node/node.cpp -o HomeCare

