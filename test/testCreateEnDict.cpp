#include"../include/CreateEnDict.h"
#include"../include/ReadConfigFile.h"
using jjx::CreateEnDict;
using jjx::ReadConfigFile;
int main()
{
	CreateEnDict creEnDict;
	creEnDict.loadFile(ReadConfigFile::getInstance()->find("LIB_EN:"), ".txt");
	creEnDict.dumpFile(ReadConfigFile::getInstance()->find("DICT_EN:"));
	return 0;
}
