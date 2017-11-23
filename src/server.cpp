#include"../include/Socket.h"
#include"../include/Acceptor.h"
#include"../include/Connection.h"
#include"../include/ReactorThreadpool.h"
#include"../include/Mylog.h"
#include"../include/ReadConfigFile.h"
#include"../include/CreateEnDict.h"
#include"../include/Corrector.h"
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
using jjx::Corrector;

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
int Compute(void *pCorrector, void *pReaThr, shared_ptr<Task> pTask) 
{
	//计算线程的任务：将传过来的数据在词典中查找，并将结果传递给IO线程
	Corrector *pCorr=(Corrector*)pCorrector;
	shared_ptr<vector<string>> pVec=pCorr->findWord(pTask->_message, 10);
	size_t candidateNum=atoi(ReadConfigFile::getInstance()->find("WORD_NUM:").c_str());
	pTask->_message.clear();
	for(size_t i=0; i<candidateNum && i<pVec->size(); ++i)
	{
		pTask->_message=pTask->_message+" "+(*pVec)[i];
	}
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
int DisConnect(shared_ptr<Connection> pCon)
{
	Mylog::getInstance()->_root.debug("DisConnect: %s:%d",
					  pCon->getPeerIp().c_str(),
					  pCon->getPeerPort());
	return 0;
}
int main()
{
	Socket soc(ReadConfigFile::getInstance()->find("IP:"), 
		   atoi(ReadConfigFile::getInstance()->find("PORT:").c_str()));
	soc.reuseAddr();//重用地址
	soc.bind();
	soc.listen();
	Acceptor acc(soc);
	ReactorThreadpool reaThrPool(acc,
				     atoi(ReadConfigFile::getInstance()->find("PTH_NUM:").c_str()),
				     atoi(ReadConfigFile::getInstance()->find("PTH_TASKSIZE:").c_str()));

	CreateEnDict creEnDict;
	creEnDict.loadFile(ReadConfigFile::getInstance()->find("LIB_EN:"), ".txt");//创建英文词典
	creEnDict.dumpFile(ReadConfigFile::getInstance()->find("DICT_EN:"));//输出词典到文件
	Corrector corr;
	corr.loadDictionary(ReadConfigFile::getInstance()->find("DICT_EN:"));//加载词典
	corr.createIndex();//建立索引

	reaThrPool.setHandleNewCon(::HandleNewCon);
	reaThrPool.setBusinessRecvData(::BusinessRecvData);
	reaThrPool.setCompute(std::bind(::Compute, &corr,
				        std::placeholders::_1,
					std::placeholders::_2));//将词典地址绑定给计算线程函数
	reaThrPool.setBusinessSendData(::BusinessSendData);
	reaThrPool.setDisConnect(::DisConnect);
	reaThrPool.start();
	return 0;
}
