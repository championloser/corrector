#include"../include/Corrector.h"
#include"../include/Mylog.h"
#include<fstream>
#include<iostream>
using std::ifstream;
using std::cout;
using std::endl;

namespace jjx
{
int Corrector::loadDictionary(const string &dictPath)
{
	ifstream ifs(dictPath);
	if(!ifs.is_open())
	{
		Mylog::getInstance()->_root.error("Corrector: open file error");
		exit(EXIT_FAILURE);
	}
	Mylog::getInstance()->_root.debug("loadFile '%s'",dictPath.c_str());
	string word;
	int num;
	while(!ifs.eof())
	{
		ifs>>word;
		ifs>>num;
		_dictionary.push_back(std::make_pair(word, num));
	}
	ifs.close();
	return 0;
}
int Corrector::createIndex()
{
	Mylog::getInstance()->_root.debug("Create Index ...");
	string letter;
	for(size_t index=0; index<_dictionary.size(); ++index)
	{
		for(size_t i=0; i<_dictionary[index].first.size(); ++i)
		{
			letter=_dictionary[index].first[i];
			set<int> numbers;
			numbers.insert(index);
			pair<map<string, set<int>>::iterator, bool> ret=
				_indexMap.insert(std::make_pair(letter, numbers));
			if(!ret.second)
			{
				ret.first->second.insert(index);
			}
		}
	}
	return 0;
}
shared_ptr<string> Corrector::findWord(const string &word)
{
	shared_ptr<string> pstr(new string(word)); 
	return pstr;
}
int Corrector::printVector()
{
	for(size_t i=0; i<_dictionary.size(); ++i)
	{
		cout<<"("<<_dictionary[i].first<<","<<_dictionary[i].second<<")";
	}
	return 0;
}
int Corrector::printMap()
{
	map<string, set<int>>::iterator it;
	for(it=_indexMap.begin(); it!=_indexMap.end(); ++it)
	{
		cout<<it->first<<": ";
		set<int>::iterator sit=it->second.begin();
		for(int i=0; i<10 && sit!=it->second.end(); ++i, ++sit)//只打印一部分
		{
			cout<<*sit<<" ";
		}
		cout<<endl;
	}
	return 0;
}
}//end of namespace jjx
