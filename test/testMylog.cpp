#include"Mylog.h"
using jjx::Mylog;
int main()
{
	char buf[]="jiaxian";
	Mylog::getInstance()->_root.emerg("This is an emerg message %s", buf);
	Mylog::getInstance()->_root.fatal("This is an emerg message %s", buf);
	Mylog::getInstance()->_root.alert("This is an emerg message %s", buf);
	Mylog::getInstance()->_root.crit("This is an emerg message %s", buf);
	Mylog::getInstance()->_root.error("This is an emerg message %s", buf);
	Mylog::getInstance()->_root.warn("This is an emerg message %s", buf);
	Mylog::getInstance()->_root.notice("This is an emerg message %s", buf);
	Mylog::getInstance()->_root.info("This is an emerg message %s", buf);
	Mylog::getInstance()->_root.debug("This is an emerg message %s", buf);
	return 0;
}
