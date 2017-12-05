#include"CreateCnDict.h"
#include"../include/ReadConfigFile.h"
using jjx::CreateCnDict;
using jjx::ReadConfigFile;
int main()
{
	CreateCnDict creCnDict;
	creCnDict.loadFile(ReadConfigFile::getInstance()->find("LIB_CN:"), ".txt");
	creCnDict.dumpFile(ReadConfigFile::getInstance()->find("DICT_CN:"));
	return 0;
}
