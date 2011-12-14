CXX := g++
CXXFLAGS := -g -std=c++0x

acrs-demo: addr.o addr4.o addrnetform.o addr6netform.o addr4netform.o addr6.o route.o route4.o route6.o acrs-demo.o
	$(CXX) $(CXXFLAGS) -o acrs-demo addr4.o addr6.o addrnetform.o addr6netform.o addr4netform.o addr.o route4.o route6.o route.o acrs-demo.o

acrs-demo.o: acrs-demo.cpp acrs.hpp addr.hpp route.hpp route4.hpp addr4.hpp route6.hpp addr6.hpp addr6netform.o addr4netform.o
	$(CXX) $(CXXFLAGS) -c acrs-demo.cpp

addr4.o: addr4.cpp addr4.hpp addr.hpp addr4netform.o
	$(CXX) $(CXXFLAGS) -c addr4.cpp

addr6.o: addr6.cpp addr6.hpp addr.hpp addr6netform.o
	$(CXX) $(CXXFLAGS) -c addr6.cpp

addr6netform.o: addr6netform.cpp addr6netform.hpp addrnetform.cpp addrnetform.hpp
	$(CXX) $(CXXFLAGS) -c addr6netform.cpp

addr4netform.o: addr4netform.cpp addr4netform.hpp addrnetform.cpp addrnetform.hpp
	$(CXX) $(CXXFLAGS) -c addr4netform.cpp

addrnetform.o: addrnetform.cpp addrnetform.hpp
	$(CXX) $(CXXFLAGS) -c addrnetform.cpp

addr.o: addr.cpp addr.hpp
	$(CXX) $(CXXFLAGS) -c addr.cpp

route4.o: route4.cpp addr4.hpp addr.hpp route4.hpp route.hpp
	$(CXX) $(CXXFLAGS) -c route4.cpp

route6.o: route6.cpp addr6.hpp addr.hpp addr6netform.hpp route6.hpp route.hpp
	$(CXX) $(CXXFLAGS) -c route6.cpp

route.o: route.cpp addr.hpp route.hpp
	$(CXX) $(CXXFLAGS) -c route.cpp

clean:
	rm -f *.o acrs-demo
