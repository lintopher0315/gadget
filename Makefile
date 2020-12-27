gadget:
	g++ `wx-config --cxxflags` -o gadget ./src/*.cpp `wx-config --libs all` -std=c++17 -lstdc++fs

clean:
	rm -f gadget
