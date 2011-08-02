CXX := g++
CXXFLAGS := -g
PROGRAMS := run-tests acrs-demo

acrs-demo : acrs-demo.cpp ip4addr.o ip4route.cpp acrs4.o
	$(CXX) $(CXXFLAGS) -o acrs-demo acrs-demo.cpp ip4addr.o ip4route.cpp acrs4.o 

run-tests : run-tests.cpp ip4addr.o ip4addr-test.o
	$(CXX) $(CXXFLAGS) -o run-tests run-tests.cpp ip4addr.o ip4addr-test.o -lcpptest

ip4route.o : ip4route.cpp ip4route.hpp
	$(CXX) $(CXXFLAGS) -c ip4route.cpp

acrs4.o : acrs4.cpp acrs4.hpp
	$(CXX) $(CXXFLAGS) -c acrs4.cpp

ip4addr.o : ip4addr.cpp ip4addr.hpp
	$(CXX) $(CXXFLAGS) -c ip4addr.cpp

ip4addr-test.o : ip4addr-test.cpp ip4addr-test.hpp
	$(CXX) $(CXXFLAGS) -c ip4addr-test.cpp

all : $(PROGRAMS)

test : run-tests
	./run-tests

clean :
	rm *.o
