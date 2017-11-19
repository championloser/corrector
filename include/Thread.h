#ifndef __THREAD_H__
#define __THREAD_H__

#include"Noncopyable.h"
#include<pthread.h>

namespace jjx
{
class ThreadPool;
class Thread : private Noncopyable
{
public:
	Thread(ThreadPool &threadPool);
	virtual ~Thread();
	void start();
	void join();
protected:
	void run();
	static void * threadFunc(void *);
private:
	bool _isRunning;
	pthread_t _pthid;
	ThreadPool &_threadPool;
};
}//end of namespace jjx

#endif
