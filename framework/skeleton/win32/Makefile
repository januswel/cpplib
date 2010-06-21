# Makefile for nmake
#
#  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
#  see LICENSE for redistributing, modifying, and so on.

program = sample
objs = main.obj global.obj name.obj version.obj about.obj usage.obj
CXX = cl
#CXXFLAGS = /EHsc /W4 /nologo
CXXFLAGS = /EHsc /W4 /nologo /O2 /DNDEBUG /MD

$(program): $(objs)
	$(CXX) $(CXXFLAGS) /Fe$(program) $(objs)

main.obj: ..\main.cpp ..\sample.hpp ..\option.hpp ..\main.hpp
	$(CXX) $(CXXFLAGS) -c ..\main.cpp

global.obj: ..\global.cpp ..\sample.hpp
	$(CXX) $(CXXFLAGS) -c ..\global.cpp

name.obj: ..\name.cpp ..\sample.hpp
	$(CXX) $(CXXFLAGS) -c ..\name.cpp

version.obj: ..\version.cpp ..\sample.hpp
	$(CXX) $(CXXFLAGS) -c ..\version.cpp

about.obj: ..\about.cpp ..\sample.hpp
	$(CXX) $(CXXFLAGS) -c ..\about.cpp

usage.obj: ..\usage.cpp ..\sample.hpp
	$(CXX) $(CXXFLAGS) -c ..\usage.cpp

.PHONEY: clean
clean:
	del *.obj *.exe
