./bin/server.out ./bin/client.out : ./src/server.cpp ./srcClient/client.cpp
#	g++ ./src/*.cpp -o ./bin/server.out -std=c++11 -pthread -llog4cpp
	g++ ./srcClient/*.cpp src/ReadConfigFile.cpp -o ./bin/client.out -std=c++11 -pthread -llog4cpp
.PHONY:clean
clean:
#	rm ./bin/server.out 
	rm ./bin/client.out
