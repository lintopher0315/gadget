gadget:
	g++ `wx-config --cxxflags` -o gadget ./src/*.cpp `wx-config --libs`

clean:
	rm -f gadget
