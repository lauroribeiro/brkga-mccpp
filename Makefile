# Compiler binary:
CPP= g++

# Recommended compiler flags for speed:
#	OpenMP enabled
#	full binary code optimization
#	full error and warning reports
#	no range checking within BRKGA:
CFLAGS= -O3 -fopenmp -Wextra -Wall -Weffc++ -ansi -pedantic -Woverloaded-virtual -Wcast-align -Wpointer-arith

# Compiler flags for debugging; uncomment if needed:
#	range checking enabled in the BRKGA API
#	OpenMP disabled
#	no binary code optimization
#CFLAGS= -DRANGECHECK -Wextra -Wall -Weffc++ -ansi -pedantic -Woverloaded-virtual -Wcast-align -Wpointer-arith

# Objects:
OBJECTS= MCCPPSolver.o MCCPPDecoder.o Graph.o Population.o brkga-mccpp.o

# Targets:
all: brkga-mccpp

brkga-mccpp: $(OBJECTS)
	$(CPP) $(CFLAGS) $(OBJECTS) -o brkga-mccpp

brkga-mccpp.o:
	$(CPP) $(CFLAGS) -c brkga-mccpp.cpp

MCCPPSolver.o:
	$(CPP) $(CFLAGS) -c MCCPPSolver.cpp

MCCPPDecoder.o:
	$(CPP) $(CFLAGS) -c MCCPPDecoder.cpp

Graph.o:
	$(CPP) $(CFLAGS) -c Graph.cpp

Population.o:
	$(CPP) $(CFLAGS) -c brkgaAPI/Population.cpp

clean:
	rm -f api-usage $(OBJECTS)
