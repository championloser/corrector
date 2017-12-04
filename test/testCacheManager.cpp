#include"CacheManager.h"
#include"../include/ReadConfigFile.h"
#include<stdlib.h>
#include<iostream>
using std::cout;
using std::endl;
using jjx::CacheManager;
using jjx::ReadConfigFile;
int main()
{
	CacheManager cm;
	cm.initCache(ReadConfigFile::getInstance()->find("CACHE_FILE:"),
		     atoi(ReadConfigFile::getInstance()->find("PTH_NUM:").c_str()));
	cm.printCacheVec();
	cout<<endl;
	cm.earseCache();
	cm.printCacheVec();
	cout<<endl;
	cm.updateCache();
	cm.printCacheVec();
	return 0;
}
