#include"ReadConfigFile.h"
#include<iostream>
using std::endl;
using std::cout;
using jjx::ReadConfigFile;
int main()
{
	ReadConfigFile::getInstance()->printMap();
	cout<<ReadConfigFile::getInstance()->find("IP:")<<endl;
	return 0;
}
