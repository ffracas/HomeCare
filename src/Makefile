CC = g++
CFLAGS = -std=c++17 -Wall
DFLAGS = -std=c++17 -g -Wall -O2
LIB = -ljsoncpp
LIBDIR = ../include
LDIRVR = ../include/vr

LIBSTR = $(LIBDIR)/structures/route/route.cpp \
         $(LIBDIR)/structures/route/routeops.cpp \
         $(LIBDIR)/structures/route/dataroute.cpp \
         $(LIBDIR)/structures/schedule/schedule.cpp \
         $(LIBDIR)/structures/schedule/scheduleoptimiser.cpp \
         $(LIBDIR)/structures/service_manager/servicemanager.cpp \
         $(LIBDIR)/structures/service_manager/infonode.cpp \
         $(LIBDIR)/structures/syncwindows/syncwindows.cpp \
         $(LIBDIR)/structures/validationode.cpp \
         $(LIBDIR)/structures/node.cpp 

LIBHCO = $(LIBDIR)/hc/hcvalidation.cpp \
         $(LIBDIR)/hc/hcsolution.cpp \
         $(LIBDIR)/readjson/hcdata.cpp

LIBSER = $(LIBDIR)/patient/patient.cpp \
         $(LIBDIR)/service/service.cpp \
         $(LIBDIR)/caregiver/caregiver.cpp \
         $(LIBDIR)/depot/depot.cpp

LIBOPT = $(LIBDIR)/optimisation/hcoptimisation.cpp \
         $(LIBDIR)/optimisation/roulette/roulette.cpp \
		 $(LIBDIR)/optimisation/optimisation_structures/alnsoptimisation.cpp \
         $(LIBDIR)/optimisation/optimisation_structures/costcoord.cpp \
         $(LIBDIR)/optimisation/optimisation_structures/kruskalgraph.cpp \
         $(LIBDIR)/optimisation/hc_removal/noderemoval.cpp \
         $(LIBDIR)/optimisation/hc_removal/randomremoval.cpp \
         $(LIBDIR)/optimisation/hc_removal/worstremoval.cpp \
         $(LIBDIR)/optimisation/hc_removal/relatedremoval.cpp \
         $(LIBDIR)/optimisation/hc_removal/clusterremoval.cpp \
         $(LIBDIR)/optimisation/hc_repair/noderepair.cpp \
         $(LIBDIR)/optimisation/hc_repair/greedyrepair.cpp \
         $(LIBDIR)/optimisation/hc_repair/regretrepair.cpp 
	   
TARGETS = HomeCare Validate

.PHONY: all clean debug

HomeCare: main.cpp
	rm -f $@
	$(CC) $(CFLAGS) $^ $(LIBSTR) $(LIBHCO) $(LIBSER) $(LIBOPT) -o $@ $(LIB)

Validate: validation.cpp
	rm -f $@
	$(CC) $(CFLAGS) $^ $(LIBSTR) $(LIBHCO) $(LIBSER) -o $@ $(LIB)

debug: main.cpp
	$(CC) $(DFLAGS) $^ $(LIBSTR) $(LIBHCO) $(LIBSER) $(LIBOPT) -o $@ $(LIB)

clean:
	rm -f $(TARGETS)

all: $(TARGETS)

