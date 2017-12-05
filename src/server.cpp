#include"../include/Socket.h"
#include"../include/Acceptor.h"
#include"../include/Connection.h"
#include"../include/ReactorThreadpool.h"
#include"../include/Mylog.h"
#include"../include/ReadConfigFile.h"
#include"../include/CreateEnDict.h"
#include"../include/CreateCnDict.h"
#include"../include/Corrector.h"
#include"../include/CacheManager.h"
#include"../include/int2str.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
using std::cout;
using std::endl;
using jjx::Socket;
using jjx::Acceptor;
using jjx::Connection;
using jjx::Task;
using jjx::ReactorThreadpool;
using jjx::Mylog;
using jjx::ReadConfigFile;
using jjx::CreateEnDict;
using jjx::CreateCnDict;
using jjx::Corrector;
using jjx::CacheManager;
using jjx::Cache;

//读取配置文件信息
const string IP = ReadConfigFile::getInstance()->find("IP:");
const int PORT = jjx::str2int(ReadConfigFile::getInstance()->find("PORT:"));
const int PTH_NUM = jjx::str2int(ReadConfigFile::getInstance()->find("PTH_NUM:"));
const int PTH_TASKSIZE = jjx::str2int(ReadConfigFile::getInstance()->find("PTH_TASKSIZE:"));
const string LIB_EN = ReadConfigFile::getInstance()->find("LIB_EN:");
const string DICT_EN = ReadConfigFile::getInstance()->find("DICT_EN:");
const string LIB_CN = ReadConfigFile::getInstance()->find("LIB_CN:");
const string DICT_CN = ReadConfigFile::getInstance()->find("DICT_CN:");
const string CACHE_FILE = ReadConfigFile::getInstance()->find("CACHE_FILE:");
const size_t WORD_NUM = jjx::str2int(ReadConfigFile::getInstance()->find("WORD_NUM:"));

int HandleNewCon(shared_ptr<Connection> pCon)
{
	char buf[]="Welcome, I am Server";
	pCon->send(buf, strlen(buf));
	return 0;
}
int BusinessRecvData(void *p, shared_ptr<Connection> pCon)
{
	char buf[256];
	memset(buf, 0, sizeof(buf));
	int ret=pCon->recv(buf, sizeof(buf));//接收对端传来的数据
	if(ret<=0)return -1;
	Mylog::getInstance()->_root.debug("recv from : %s:%d: %s",
					  pCon->getPeerIp().c_str(),
					  pCon->getPeerPort(),
					  buf);
	shared_ptr<Task> pTask(new Task);//将数据打包成任务
	pTask->_message=buf;
	pTask->_pCon=pCon;
	ReactorThreadpool *pReaThrPool=(ReactorThreadpool*)p;
	//将任务绑定到_compute函数上，并将函数交给线程池处理
	pReaThrPool->addTaskToThreadPool(std::bind(pReaThrPool->_compute, pTask));
	return 0;
}
int Compute(void *pCorrector, void * pCachManag, void *pReaThr, shared_ptr<Task> pTask) 
{
	//计算线程的任务：将传过来的数据先在线程自己的缓存中查找，
	//缓存找不到就在词典中查找，并将结果传递给IO线程
	//之后将查询结果记入缓存
	CacheManager *pCachM=(CacheManager*)pCachManag;
	Cache &myCache=pCachM->getCacheByIndex(jjx::threadIndex);
	shared_ptr<vector<string>> pVec=myCache.findWord(pTask->_message);
	if(pVec->size()==0)//如果cache中没有找到
	{
		Corrector *pCorr=(Corrector*)pCorrector;
		pVec=pCorr->findWord(pTask->_message, 10);//在词典中查找
		if(pVec->size()!=0)myCache.addWord(pTask->_message, pVec);//如果词典中找到了就添加进缓存
	}
	pTask->_message.clear();
	for(size_t i=0; i<WORD_NUM && i<pVec->size(); ++i)
	{
		pTask->_message=pTask->_message+" "+(*pVec)[i];
	}
	if(pTask->_message.size()==0)pTask->_message=" ";//如果都没有找到，填一个空格
	//将结果放到任务队列
	ReactorThreadpool *pReaThrPool=(ReactorThreadpool*)pReaThr;
	pReaThrPool->addTaskToVeactor(pTask);
	//通过eventfd通知IO线程
	pReaThrPool->writeEventfd();
	return 0;
}
int BusinessSendData(void *p)
{
	ReactorThreadpool *pReaThrPool=(ReactorThreadpool*)p;
	//将eventfd读空
	pReaThrPool->readEventfd();
	//从vector中用副本换出任务
	shared_ptr<vector<shared_ptr<Task>>> pVectorTaskCopy=pReaThrPool->copyTaskFromVeactor();
	//将每个任务发送至目的地
	for(size_t i=0; i<(*pVectorTaskCopy).size(); ++i)
	{
		shared_ptr<Task> pTask=(*pVectorTaskCopy)[i];
		pTask->_pCon->send(pTask->_message.c_str(), pTask->_message.size());
		Mylog::getInstance()->_root.debug("send to   : %s:%d: %s",
						  pTask->_pCon->getPeerIp().c_str(),
						  pTask->_pCon->getPeerPort(),
						  pTask->_message.c_str());
	}
	return 0;
}
int WriteCacheToFile(void *pCachManag, void *p)
{
	CacheManager *pCachM=(CacheManager *)pCachManag;
	ReactorThreadpool *pReaThrPool=(ReactorThreadpool *)p;
	//将写缓存的函数打包丢给线程池处理
	pReaThrPool->addTaskToThreadPool(std::bind(&CacheManager::updateCache, pCachM));
	return 0;
}
int DisConnect(shared_ptr<Connection> pCon)
{
	Mylog::getInstance()->_root.debug("DisConnect: %s:%d",
					  pCon->getPeerIp().c_str(),
					  pCon->getPeerPort());
	return 0;
}
int main()
{
	Socket soc(IP, PORT);
	soc.reuseAddr();//重用地址
	soc.bind();
	soc.listen();
	Acceptor acc(soc);
	ReactorThreadpool reaThrPool(acc, PTH_NUM, PTH_TASKSIZE);

	CreateEnDict creEnDict;
	creEnDict.loadFile(LIB_EN, ".txt");//创建英文词典
	creEnDict.dumpFile(DICT_EN);//输出英文词典到文件

	CreateCnDict creCnDict;
	creCnDict.loadFile(LIB_CN, ".txt");//创建中文词典
	creCnDict.dumpFile(DICT_CN);//输出中文词典到文件

	Corrector corr;
	corr.loadDictionary(DICT_EN);//加载英文词典
	corr.loadDictionary(DICT_CN);//加载中文词典
	corr.createIndex();//建立索引

	CacheManager cachManag;
	cachManag.initCache(CACHE_FILE, PTH_NUM);//初始化缓存

	reaThrPool.setHandleNewCon(::HandleNewCon);
	reaThrPool.setBusinessRecvData(::BusinessRecvData);
	reaThrPool.setCompute(std::bind(::Compute,
					&corr,//将词典地址绑定给计算线程函数
					&cachManag,//将缓存地址绑定给计算线程函数
				        std::placeholders::_1,
					std::placeholders::_2));
	reaThrPool.setBusinessSendData(::BusinessSendData);
	reaThrPool.setWriteCacheToFile(std::bind(::WriteCacheToFile, &cachManag, std::placeholders::_1));
	reaThrPool.setDisConnect(::DisConnect);
	reaThrPool.start();
	return 0;
}
