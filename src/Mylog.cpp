#include"../include/Mylog.h"

namespace jjx
{
Mylog * Mylog::_pMylog=NULL;
Mylog::Recycle Mylog::rec;
Mylog * Mylog::getInstance(const char *filename)
{
	if(NULL==_pMylog)_pMylog=new Mylog(filename);
	return _pMylog;
}
Mylog::Mylog(const char *filename)
: _root(Category::getRoot())
{
#if ISROLLINGFILE
	RollingFileAppender *fAppender=new RollingFileAppender("fAppender", filename, 
								ROLLINGFILESIZE, ROLLINGFILENUM);
#else
	FileAppender *fAppender=new FileAppender("fAppender",filename);
#endif
	PatternLayout *pLayout2=new PatternLayout();
	pLayout2->setConversionPattern("%d: [%p]: %m%n");
	fAppender->setLayout(pLayout2);
	_root.addAppender(fAppender);
	_root.setPriority(PRIORITY);
}
}//end of namespace jjx
