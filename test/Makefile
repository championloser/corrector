server.out: ../src/server.cpp
#	g++ testCreateEnDict.cpp ../src/CreateEnDict.cpp ../src/Mylog.cpp ../src/ReadConfigFile.cpp ../src/GetFilenameFromDir.cpp -o createEnDict.out -pthread -llog4cpp -std=c++11	
#	g++ testCorrector.cpp ../src/Corrector.cpp ../src/Mylog.cpp ../src/ReadConfigFile.cpp -o corrector.out -pthread -llog4cpp -std=c++11
	g++ testCacheManager.cpp ../src/Mylog.cpp ../src/ReadConfigFile.cpp Cache.cpp CacheManager.cpp -o cacheManager.out -pthread -llog4cpp -std=c++11
.PHONY:clean
clean:
#	rm createEnDict.out
#	rm corrector.out
	rm cacheManager.out
