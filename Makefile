ServerSrc:=$(wildcard ./src/*.cpp)
ServerObj:=$(patsubst %.cpp, %.o, $(ServerSrc))
ServerElf:= ./bin/server.out
ClientSrc:=$(wildcard ./srcClient/*.cpp)
ClientObj:=$(patsubst %.cpp, %.o, $(ClientSrc))
ClientElf:= ./bin/client.out

LIBS:= -lpthread -llog4cpp
CXX:=g++
CXXFLAGS:= -Wall -std=c++11 $(LIBS)
$(ServerElf) $(ClientElf):$(ServerObj) $(ClientObj)
	$(CXX) $(ServerObj) -o $(ServerElf) $(CXXFLAGS)
	$(CXX) $(ClientObj) -o $(ClientElf) $(CXXFLAGS)

.PHONY:clean
clean:
	rm -rf $(ServerElf)
	rm -rf $(ClientElf)
	rm -rf $(ServerObj)
	rm -rf $(ClientObj)
