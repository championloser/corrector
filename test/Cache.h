#ifndef __CACHE_H__
#define __CACHE_H__

#include"MutexLock.h"
#include<map>
#include<memory>
#include<vector>
#include<string>
using std::map;
using std::shared_ptr;
using std::vector;
using std::string;
namespace jjx
{
class Cache
{
public:
	Cache();
	int readCacheFromFile(const string &cacheFile);
	bool isTidCacheExist(long threadTid);
	shared_ptr<map<string, shared_ptr<vector<string>>>> findCacheByTid(long threadTid);
	int createCacheByTid(long threadTid);
	int addItemByTid(long threadTid, string &key, shared_ptr<vector<string>> candidateWord);
	int writeCacheToFile(const string &cacheFile);
private:
	MutexLock _cacheLock;
	map<long, shared_ptr<map<string, shared_ptr<vector<string>>>>> _cacheMap;
	shared_ptr<map<string, shared_ptr<vector<string>>>> _pCache;
};
}//end of namespace jjx

#endif
