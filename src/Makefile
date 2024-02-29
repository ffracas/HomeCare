CC = g++
CFLAGS = -std=c++11 -Wall
DFLAGS = -std=c++11 -g -Wall -O2
LIB = -ljsoncpp
LIBDIR = ../include
LDIRVR = ../include/vr

LIBSTR = $(LIBDIR)/structures/validationode.cpp \
         $(LIBDIR)/structures/node.cpp \
         $(LIBDIR)/structures/route.cpp

LIBHCO = $(LIBDIR)/hc/hcvalidation.cpp \
         $(LIBDIR)/hc/hcsolution.cpp \
         $(LIBDIR)/readjson/hcdata.cpp

LIBSER = $(LIBDIR)/patient/patient.cpp \
         $(LIBDIR)/service/service.cpp \
         $(LIBDIR)/caregiver/caregiver.cpp \
         $(LIBDIR)/depot/depot.cpp

LIBOPT = $(LIBDIR)/optimisation/hcoptimisation.cpp \
		 $(LIBDIR)/optimisation/optimisation_structures/alnsoptimisation.cpp \
         $(LIBDIR)/optimisation/optimisation_structures/infonode.cpp \
         $(LIBDIR)/optimisation/optimisation_structures/servicemanager.cpp \
         $(LIBDIR)/optimisation/optimisation_structures/routesopt.cpp \
         $(LIBDIR)/optimisation/optimisation_structures/costcoord.cpp \
         $(LIBDIR)/optimisation/optimisation_structures/kruskalgraph.cpp \
         $(LIBDIR)/optimisation/hc_removal/noderemoval.cpp \
         $(LIBDIR)/optimisation/hc_removal/randomremoval.cpp \
         $(LIBDIR)/optimisation/hc_removal/worstremoval.cpp \
         $(LIBDIR)/optimisation/hc_removal/relatedremoval.cpp \
         $(LIBDIR)/optimisation/hc_removal/clusterremoval.cpp \
         $(LIBDIR)/optimisation/hc_repair/noderepair.cpp \
         $(LIBDIR)/optimisation/hc_repair/greedyrepair.cpp
	   
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

