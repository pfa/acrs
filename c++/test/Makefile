include ../Makefile.inc

run-tests: run-tests.o
	$(RUN_CXX) -o run-tests run-tests.o

run-tests.o: run-tests.cpp
	$(RUN_CXX) -c run-tests.cpp

test: run-tests
	./run-tests

clean:
	rm -f *.o run-tests
