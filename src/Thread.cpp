#include"../include/Thread.h"
#include"../include/ThreadPool.h"
#include<unistd.h>
#include<sys/syscall.h>
#include<iostream>
using std::cout;
using std::endl;

namespace jjx
{
Thread::Thread(ThreadPool &threadPool)
: _isRunning(false)
, _threadPool(threadPool)
{}
Thread::~Thread()
{
	if(_isRunning)pthread_detach(_pthid);
}
void Thread::start()
{
	pthread_create(&_pthid, NULL, threadFunc, (void *)this);
	_isRunning=true;
}
void Thread::join()
{
	pthread_join(_pthid, NULL);
	_isRunning=false;
}
void * Thread::threadFunc(void *p)
{
	Thread *pth=static_cast<Thread *>(p);
	cout<<"I am pthread: "<<::syscall(SYS_gettid)<<","<<::pthread_self()<<endl;
	pth->run();
	return NULL;
}
void Thread::run()
{
	_threadPool.threadFun();
}
}//end of namespace jj,
