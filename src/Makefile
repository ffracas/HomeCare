CC = g++
CFLAGS = -std=c++11 -Wall
DFLAGS = -std=c++11 -Wall -g
LIB = -ljsoncpp
LIBDIR = ../include
LDIRVR = ../include/vr

LIBSTR = $(LIBDIR)/structures/validationode.cpp \
         $(LIBDIR)/structures/node.cpp \
         $(LIBDIR)/structures/arc.cpp \
         $(LIBDIR)/structures/route.cpp

LIBHCO = $(LIBDIR)/hc/hcvalidation.cpp \
         $(LIBDIR)/hc/hcsolution.cpp \
         $(LIBDIR)/readjson/hcdata.cpp

LIBSER = $(LIBDIR)/patient/patient.cpp \
         $(LIBDIR)/service/service.cpp \
         $(LIBDIR)/caregiver/caregiver.cpp \
         $(LIBDIR)/depot/depot.cpp

LIBOPT = $(LIBDIR)/optimisation/hcoptimisation.cpp \
		 $(LIBDIR)/optimisation/optimisation_structures/optimisationobject.cpp \
         $(LIBDIR)/optimisation/optimisation_structures/infonode.cpp
	   
TARGETS = HomeCare Validate

.PHONY: all clean debug

HomeCare: main.cpp
	rm -f $@
	$(CC) $(CFLAGS) $^ $(LIBSTR) $(LIBHCO) $(LIBSER) $(LIBOPT) -o $@ $(LIB)

Validate: validation.cpp
	rm -f $@
	$(CC) $(CFLAGS) $^ $(LIBSTR) $(LIBHCO) $(LIBSER) -o $@ $(LIB)

debug: main.cpp
	$(CC) $(DFLAGS) $^ $(LIBSTR) $(LIBHCO) $(LIBSER) $(LIBOPT) -o HomeCare $(LIB)

clean:
	rm -f $(TARGETS)

all: $(TARGETS)

