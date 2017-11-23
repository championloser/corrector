#include"../include/Cache.h"
#include"../include/ReadConfigFile.h"
#include"../include/Mylog.h"
#include"../include/MutexLockGuard.h"
#include<fstream>
#include<sstream>
using std::ifstream;
using std::istringstream;

namespace jjx
{
Cache::Cache()
: _pCache(new map<string, shared_ptr<vector<string>>>)
{
	readCacheFromFile(ReadConfigFile::getInstance()->find("CACHE_FILE:"));
}
int Cache::readCacheFromFile(const string &cacheFile)
{
	ifstream ifs(cacheFile);
	if(!ifs.is_open())
	{
		Mylog::getInstance()->_root.debug("Cache: read cache file error");
		exit(EXIT_FAILURE);
	}
	string line;
	string key;
	string word;
	while(std::getline(ifs, line))
	{
		istringstream iss(line);
		iss>>key;
		shared_ptr<vector<string>> pCandidateWord(new vector<string>);
		while(!iss.eof())
		{
			iss>>word;
			pCandidateWord->push_back(word);
		}
		_pCache->insert(std::make_pair(key, pCandidateWord));
	}
	ifs.close();//记得关闭文件描述符
	return 0;
}
bool Cache::isTidCacheExist(long threadTid)
{
	auto ret=_cacheMap.find(threadTid);
	if(ret==_cacheMap.end())return false;
	else return true;
}
shared_ptr<map<string, shared_ptr<vector<string>>>> Cache::findCacheByTid(long threadTid)
{
	
}
int Cache::createCacheByTid(long threadTid)
{

	return 0;
}
int Cache::addItemByTid(long threadTid, string &key, shared_ptr<vector<string>> candidateWord)
{
	return 0;
}
int Cache::writeCacheToFile(const string &cacheFile)
{
}
}//end of namespace jjx
