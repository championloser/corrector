#ifndef __CORRECTOR_H__
#define __CORRECTOR_H__

#include<string>
#include<memory>
#include<map>
#include<set>
#include<vector>
using std::string;
using std::shared_ptr;
using std::vector;
using std::map;
using std::set;
using std::pair;

namespace jjx
{
class Corrector
{
public:
	int loadDictionary(const string &dictPath);
	int createIndex();
	shared_ptr<string> findWord(const string &word);
	int printVector(); //for debug
	int printMap();	//for debug
private:
	vector<pair<string, int>> _dictionary;
	map<string, set<int>> _indexMap;
};
}//end of namespace jjx

#endif
