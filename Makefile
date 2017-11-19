./bin/server.out : ./src/server.cpp
	g++ ./src/*.cpp -o ./bin/server.out -std=c++11 -pthread 
	g++ ./srcClient/*.cpp -o ./bin/client.out -std=c++11
.PHONY:clean
clean:
	rm ./bin/server.out ./bin/client.out
