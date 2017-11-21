#include"../include/Corrector.h"
#include"../include/ReadConfigFile.h"
using jjx::Corrector;
using jjx::ReadConfigFile;
int main()
{
	Corrector corr;
	corr.loadDictionary(ReadConfigFile::getInstance()->find("DICT_EN:"));
	//corr.printVector();
	corr.createIndex();
	corr.printMap();
	return 0;
}
