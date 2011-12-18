include Makefile.inc

TEST_DIR="test"
.PHONY : test

acrs-demo: addr.o addr4.o addrnetform.o addr6netform.o addr4netform.o addr6.o route.o route4.o route6.o acrs-demo.o
	$(RUN_CXX) -o acrs-demo addr4.o addr6.o addrnetform.o addr6netform.o addr4netform.o addr.o route4.o route6.o route.o acrs-demo.o

acrs-demo.o: acrs-demo.cpp acrs.hpp addr.hpp route.hpp route4.hpp addr4.hpp route6.hpp addr6.hpp addr6netform.hpp addr4netform.hpp
	$(RUN_CXX) -c acrs-demo.cpp

addr4.o: addr4.cpp addr4.hpp addr.hpp addr4netform.hpp
	$(RUN_CXX) -c addr4.cpp

addr6.o: addr6.cpp addr6.hpp addr.hpp addr6netform.hpp
	$(RUN_CXX) -c addr6.cpp

addr6netform.o: addr6netform.cpp addr6netform.hpp addrnetform.cpp addrnetform.hpp
	$(RUN_CXX) -c addr6netform.cpp

addr4netform.o: addr4netform.cpp addr4netform.hpp addrnetform.cpp addrnetform.hpp
	$(RUN_CXX) -c addr4netform.cpp

addrnetform.o: addrnetform.cpp addrnetform.hpp
	$(RUN_CXX) -c addrnetform.cpp

addr.o: addr.cpp addr.hpp
	$(RUN_CXX) -c addr.cpp

route4.o: route4.cpp addr4.hpp addr.hpp route4.hpp route.hpp
	$(RUN_CXX) -c route4.cpp

route6.o: route6.cpp addr6.hpp addr.hpp addr6netform.hpp route6.hpp route.hpp
	$(RUN_CXX) -c route6.cpp

route.o: route.cpp addr.hpp route.hpp
	$(RUN_CXX) -c route.cpp

test:
	make test -C $(TEST_DIR)

clean:
	rm -f *.o acrs-demo
	make clean -C $(TEST_DIR)

all: acrs-demo test
