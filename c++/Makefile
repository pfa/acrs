CXX := g++
CXXFLAGS := -g
PROGRAMS := run-tests

run-tests : run-tests.cpp ip4addr.o ip4addr-test.o
	$(CXX) $(CXXFLAGS) -o run-tests run-tests.cpp ip4addr.o ip4addr-test.o -lcpptest

ip4addr.o : ip4addr.cpp ip4addr.hpp
	$(CXX) $(CXXFLAGS) -c ip4addr.cpp

ip4addr-test.o : ip4addr-test.cpp ip4addr-test.hpp
	$(CXX) $(CXXFLAGS) -c ip4addr-test.cpp

all : $(PROGRAMS)
clean :
	rm *.o
test :
	./run-tests ; exit 0
