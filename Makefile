gadget:
	g++ `wx-config --cxxflags` -o gadget ./src/*.cpp `wx-config --libs core,aui,richtext,stc` -std=c++17 -lstdc++fs

#  --libs core,aui,richtext,stc:	links all required libraries
#  -std=c++17:						compile under C++17 standard; for filesystem library
#  -lstdc++fs:						also for filesystem library

clean:
	rm -f gadget
