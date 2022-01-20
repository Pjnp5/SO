CC = gcc
CFLAGS = -Wall

SUFFIX = $(shell getconf LONG_BIT)

PILOT = semSharedMemPilot
HOSTESS = semSharedMemHostess
PASSENGER = semSharedMemPassenger
MAIN = probSemSharedMemAirLift

OBJS = sharedMemory.o semaphore.o logging.o

.PHONY: all pg pt ht pg_ht all_bin \
	main pilot hostess passenger \
	pilot_bin hostess_bin passenger_bin \
	clean cleanall doc

all:        passenger      hostess     pilot       main clean
pg:   	    passenger      hostess_bin pilot_bin   main clean
pt:   	    passenger_bin  hostess_bin pilot       main clean
ht:   	    passenger_bin  hostess     pilot_bin   main clean
pg_ht:		passenger      hostess     pilot_bin   main clean
all_bin:	passenger_bin  hostess_bin pilot_bin   main clean

pilot:	$(PILOT).o $(OBJS)
	$(CC) -o ../run/$@ $^ -lm

hostess:		$(HOSTESS).o $(OBJS)
	$(CC) -o ../run/$@ $^

passenger:	$(PASSENGER).o $(OBJS)
	$(CC) -o ../run/$@ $^ -lm

main:		$(MAIN).o $(OBJS)
	$(CC) -o ../run/$(MAIN) $^ -lm

pilot_bin:
	cp ../run/pilot_bin_$(SUFFIX) ../run/pilot

hostess_bin:
	cp ../run/hostess_bin_$(SUFFIX) ../run/hostess

passenger_bin:
	cp ../run/passenger_bin_$(SUFFIX) ../run/passenger

clean:
	rm -f *.o

cleanall:	clean
	rm -f ../run/$(MAIN) ../run/pilot ../run/hostess ../run/passenger

doc:
	(cd ../doc; doxygen)
