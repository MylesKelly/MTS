

all: solver 

include Makefile.config

SOURCES = MTS.cpp SystemSolver.cpp SunLinSolWrapper.cpp ErrorChecker.cpp Solver.cpp Matrices.cpp DGStatic.cpp PhysicsCases.cpp


HEADERS = gridStructures.hpp SunLinSolWrapper.hpp SunMatrixWrapper.hpp SystemSolver.hpp ErrorChecker.hpp ErrorTester.hpp TransportSystem.hpp PhysicsCases.hpp DGSoln.hpp
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

PHYSICS_SOURCES = $(wildcard PhysicsCases/*.cpp)
PHYSICS_OBJECTS = $(patsubst %.cpp,%.o,$(PHYSICS_SOURCES))

CXXFLAGS += -I.

%.o: %.cpp Makefile $(HEADERS)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

solver: $(OBJECTS) $(PHYSICS_OBJECTS) $(HEADERS) Makefile
	$(CXX) $(CXXFLAGS) -g -o solver $(OBJECTS) $(PHYSICS_OBJECTS) $(LDFLAGS)

#unit_tests: $(TEST_SOURCES) $(TESTOBJECTS) $(HEADERS) Makefile
#	$(CXX) $(CXXFLAGS) -g -o unit_test_suite $(TEST_SOURCES) $(TESTOBJECTS) $(LDFLAGS)
#	./unit_test_suite

clean: 
	rm -f solver unit_test_suite errortest dbsolver $(OBJECTS) $(ERROBJECTS) $(TESTOBJECTS)

regression_tests: solver
	cd UnitTests; ./CheckRegressionTests.sh

.PHONY: clean regression_tests unit_tests 
