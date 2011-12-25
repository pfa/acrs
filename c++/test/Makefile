include ../Makefile.inc
CXXFLAGS := $(CXXFLAGS) -lcpptest

run-tests: run-tests.o addr6netform-test.o ../addr6netform.o
	$(CXX) $(CXXFLAGS) -o run-tests run-tests.o addr6netform-test.o ../addr6netform.o ../addr4netform.o ../addrnetform.o

addr6netform-test.o: addr6netform-test.cpp addr6netform-test.hpp
	$(CXX) $(CXXFLAGS) -c addr6netform-test.cpp

run-tests.o: run-tests.cpp
	$(CXX) $(CXXFLAGS) -c run-tests.cpp

test: run-tests
	./run-tests

clean:
	rm -f *.o run-tests
