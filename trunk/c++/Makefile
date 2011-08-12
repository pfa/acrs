CXX := g++
CXXFLAGS := -g
PROGRAMS := run-tests acrs-demo

acrs-demo: acrs-demo.o ip4addr.o ip4route.o acrs4.o
	$(CXX) $(CXXFLAGS) -o acrs-demo acrs-demo.o ip4addr.o ip4route.o acrs4.o

run-tests: run-tests.o ip4addr.o ip4addr-test.o ip4route.o ip4route-test.o acrs4-test.o acrs4.o
	$(CXX) $(CXXFLAGS) -o run-tests run-tests.o ip4addr.o ip4addr-test.o ip4route.o ip4route-test.o acrs4-test.o acrs4.o -lcpptest

acrs4.o: acrs4.cpp acrs4.hpp ip4route.hpp ip4addr.hpp
	$(CXX) $(CXXFLAGS) -c acrs4.cpp

acrs-demo.o: acrs-demo.cpp acrs4.hpp ip4route.hpp ip4addr.hpp
	$(CXX) $(CXXFLAGS) -c acrs-demo.cpp

ip4addr.o: ip4addr.cpp ip4addr.hpp
	$(CXX) $(CXXFLAGS) -c ip4addr.cpp

ip4addr-test.o: ip4addr-test.cpp ip4addr.hpp ip4addr-test.hpp
	$(CXX) $(CXXFLAGS) -c ip4addr-test.cpp

ip4route.o: ip4route.cpp ip4addr.hpp ip4route.hpp
	$(CXX) $(CXXFLAGS) -c ip4route.cpp

ip4route-test.o: ip4route-test.cpp ip4route.hpp ip4addr.hpp ip4route-test.hpp ip4addr-test.hpp
	$(CXX) $(CXXFLAGS) -c ip4route-test.cpp

acrs4-test.o: acrs4-test.cpp acrs4.hpp ip4route.hpp ip4addr.hpp acrs4-test.hpp
	$(CXX) $(CXXFLAGS) -c acrs4-test.cpp

run-tests.o: run-tests.cpp ip4addr-test.hpp ip4addr.hpp ip4route-test.hpp ip4route.hpp acrs4.hpp acrs4-test.hpp acrs4.hpp acrs4-test.hpp
	$(CXX) $(CXXFLAGS) -c run-tests.cpp

all: $(PROGRAMS)

test: run-tests
	./run-tests

gcov: CXXFLAGS += -fprofile-arcs -ftest-coverage
gcov: all

clean:
	rm -f *.o

gcov-clean:
	rm -rf usr bits ext home i486-linux-gnu *.gcda *gcno *.css *.html *.png *.info
