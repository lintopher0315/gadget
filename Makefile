gadget:
	g++ `wx-config --cxxflags` -o gadget ./src/*.cpp `wx-config --libs all`

clean:
	rm -f gadget
