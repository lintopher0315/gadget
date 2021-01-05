gadget:
	g++ `wx-config --cxxflags` -o gadget ./src/*.cpp `wx-config --libs all` -std=c++17 -lstdc++fs

# --libs all: includes all available wxWidgets libraries
#  -std=c++17: compile under C++17 standard; for filesystem library
#  -lstdc++fs: also for filesystem library

clean:
	rm -f gadget
