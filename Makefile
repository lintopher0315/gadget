CC=gcc
CXX=g++
RM=rm -f
CXXFLAGS=`wx-config --cxxflags` -std=c++17 -lstdc++fs
LDLIBS=`wx-config --libs core,aui,richtext,stc`

SRCS=Gadget.cpp Frame.cpp Window.cpp Editor.cpp FileTree.cpp Panel.cpp Command.cpp CommandBar.cpp StatusBar.cpp StatusSection.cpp HelpFile.cpp FileHelper.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

VPATH=./src

all: gadget

gadget: $(OBJS)
	$(CXX) -o gadget $(OBJS) $(CXXFLAGS) $(LDLIBS)

Gadget.o: Gadget.cpp Gadget.h ./src/res/gadget.xpm
	$(CXX) -c ./src/Gadget.cpp $(CXXFLAGS)

Frame.o: Frame.cpp Frame.h
	$(CXX) -c ./src/Frame.cpp $(CXXFLAGS)

Window.o: Window.cpp Window.h
	$(CXX) -c ./src/Window.cpp $(CXXFLAGS)

Editor.o: Editor.cpp Editor.h
	$(CXX) -c ./src/Editor.cpp $(CXXFLAGS)

FileTree.o: FileTree.cpp FileTree.h
	$(CXX) -c ./src/FileTree.cpp $(CXXFLAGS)

Panel.o: Panel.cpp Panel.h
	$(CXX) -c ./src/Panel.cpp $(CXXFLAGS)

Command.o: Command.cpp Command.h
	$(CXX) -c ./src/Command.cpp $(CXXFLAGS)

CommandBar.o: CommandBar.cpp CommandBar.h
	$(CXX) -c ./src/CommandBar.cpp $(CXXFLAGS)

StatusBar.o: StatusBar.cpp StatusBar.h
	$(CXX) -c ./src/StatusBar.cpp $(CXXFLAGS)

StatusSection.o: StatusSection.cpp StatusSection.h
	$(CXX) -c ./src/StatusSection.cpp $(CXXFLAGS)

HelpFile.o: HelpFile.cpp HelpFile.h
	$(CXX) -c ./src/HelpFile.cpp $(CXXFLAGS)

FileHelper.o: FileHelper.cpp FileHelper.h
	$(CXX) -c ./src/FileHelper.cpp $(CXXFLAGS)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) gadget
