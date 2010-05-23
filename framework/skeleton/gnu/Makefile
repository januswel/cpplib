# Makefile for gnu make

program = sample
objs = main.o name.o version.o about.o usage.o
CXX = g++
#CXXFLAGS = -Wall --pedantic
CXXFLAGS = -Wall --pedantic -O2 -DNDEBUG

$(program): $(objs)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^

main.o: ../main.cpp ../sample.hpp ../option.hpp ../main.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

name.o: ../name.cpp ../sample.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

version.o: ../version.cpp ../sample.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

about.o: ../about.cpp ../sample.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

usage.o: ../usage.cpp ../sample.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONEY: clean
clean:
	rm -f *.o $(program)
