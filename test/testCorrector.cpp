#include"../include/Corrector.h"
#include"../include/ReadConfigFile.h"
#include<iostream>
using std::cout;
using std::cin;
using std::endl;
using jjx::Corrector;
using jjx::ReadConfigFile;
int main()
{
	Corrector corr;
	corr.loadDictionary(ReadConfigFile::getInstance()->find("DICT_EN:"));
	//corr.printVector();
	corr.createIndex();
	string word;
	while(true)
	{
		cout<<"please input a word, 'exit' for quit: ";
		cin>>word;
		if(word=="exit")break;
		cout<<*(corr.findWord(word))<<endl;;
	}
	return 0;
}
