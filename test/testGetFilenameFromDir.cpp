#include"../include/GetFilenameFromDir.h"
#include"../include/ReadConfigFile.h"
using jjx::GetFilenameFromDir;
using jjx::ReadConfigFile;
int main()
{
	GetFilenameFromDir getEnFile(ReadConfigFile::getInstance()->find("LIB_EN:"), ".txt");
	getEnFile.printFilename();
	GetFilenameFromDir getCnFile(ReadConfigFile::getInstance()->find("LIB_CN:"), ".txt");
	getCnFile.printFilename();
	return 0;
}
