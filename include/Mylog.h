#ifndef __MYLOG_H__
#define __MYLOG_H__

#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/RollingFileAppender.hh>
#include<log4cpp/Category.hh>
#include<log4cpp/PatternLayout.hh>
#include<log4cpp/Priority.hh>
using namespace log4cpp;

namespace jjx
{
#define FILENAME "../log/mylog.log"		//日志文件位置
#define PRIORITY Priority::DEBUG 	//日志级别
#define ISROLLINGFILE 1			//是否设置回滚文件，1设置，0不设置
#define ROLLINGFILESIZE 1024*1024	//设置回滚文件大小
#define ROLLINGFILENUM 5		//设置保留的回滚文件个数
class Mylog
{
public:
	static Mylog *getInstance(const char *filename=FILENAME);
	Category &_root;
private:
	Mylog(const char *filename);
	static Mylog *_pMylog;
	class Recycle
	{
	public:
		~Recycle()
		{
			delete _pMylog;
			_pMylog=NULL;
			Category::shutdown();
		}
	};
	static Recycle rec;
};
}//end of namespace jjx

#endif
