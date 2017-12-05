#include<cppjieba/Jieba.hpp>
#include<iostream>
#include<vector>
#include<string>
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

const char* const DICT_PATH="../data/dict/jieba.dict.utf8";
const char* const HMM_PATH="../data/dict/hmm_model.utf8";
const char* const USER_DICT_PATH="../data/dict/user.dict.utf8";
const char* const IDF_PATH="../data/dict/idf.utf8";
const char* const STOP_WORD_PATH="../data/dict/stop_words.utf8";

int main()
{
	cppjieba::Jieba jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH);
	vector<string> words;
	string s;
	while(cin>>s)
	{
		words.clear();
		jieba.Cut(s, words, true);
		for(size_t idx=0; idx<words.size(); ++idx)
		{
			cout<<words[idx]<<"/";
		}
		cout<<endl;
	}
	return 0;
}
