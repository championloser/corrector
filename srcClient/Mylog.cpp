#include"../include/Mylog.h"
#include"../include/ReadConfigFile.h"
#include<stdlib.h>
#include<iostream>
using std::cout;
using std::endl;
using jjx::ReadConfigFile;

namespace jjx
{
Mylog * Mylog::_pMylog=NULL;
Mylog::Recycle Mylog::rec;
Mylog * Mylog::getInstance()
{
	if(NULL==_pMylog)_pMylog=new Mylog(ReadConfigFile::getInstance()->find("LOG_FILENAME:").c_str());
	return _pMylog;
}
Mylog::Mylog(const char *filename)
: _root(Category::getRoot())
, _filename(filename)
{
	if(atoi(ReadConfigFile::getInstance()->find("LOG_ISCOUT:").c_str()))
	{
		OstreamAppender *fAppender=new OstreamAppender("fAppender",&std::cout);
		PatternLayout *pLayout=new PatternLayout();
		pLayout->setConversionPattern("%d: [%p]: %m%n");
		fAppender->setLayout(pLayout);
		_root.addAppender(fAppender);
		_root.setPriority(Priority::DEBUG);
	}
	else if(atoi(ReadConfigFile::getInstance()->find("LOG_ISROLLINGFILE:").c_str()))
	{
		RollingFileAppender *fAppender=new RollingFileAppender
			("fAppender",
		 	_filename, 
			atoi(ReadConfigFile::getInstance()->find("LOG_ROLLINGFILESIZE:").c_str()),
			atoi(ReadConfigFile::getInstance()->find("LOG_ROLLINGFILENUM:").c_str()));
		PatternLayout *pLayout=new PatternLayout();
		pLayout->setConversionPattern("%d: [%p]: %m%n");
		fAppender->setLayout(pLayout);
		_root.addAppender(fAppender);
		_root.setPriority(Priority::DEBUG);
	}else{
		FileAppender *fAppender=new FileAppender("fAppender",_filename);
		PatternLayout *pLayout=new PatternLayout();
		pLayout->setConversionPattern("%d: [%p]: %m%n");
		fAppender->setLayout(pLayout);
		_root.addAppender(fAppender);
		_root.setPriority(Priority::DEBUG);
	}
};
}//end of namespace jjx
