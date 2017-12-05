#include<iostream>
#include<string>
#include<vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;
void fun(string &str)
{
	for(size_t idx=0; idx<str.size(); ++idx)
	{
		for(int i=7; i>=0; --i)
		{
			if(str[idx] & 1<<i)cout<<"1";
			else cout<<"0";
		}
		cout<<" ";
	}
	cout<<endl;
}
int Mysplit(string &str, vector<string> &character)
{
	size_t cur=0;
	while(cur<str.size())
	{
		int length=0;
		for(int i=7; i>=0; --i)
		{
			if(str[cur] & 1<<i)++length;
			else break;
		}
		if(0==length)++length;
		character.push_back(str.substr(cur, length));
		cur=cur+length;
	}
	return 0;
}
int main()
{
	string str1="中a国b人";
	string str2="abc";
	cout<<"str1's size is "<<str1.size()<<endl;
	cout<<"str2's size is "<<str2.size()<<endl;
	fun(str1);
	fun(str2);
	vector<string> character;
	Mysplit(str1, character);
	for(size_t idx=0; idx<character.size(); ++idx)cout<<character[idx]<<endl;
	return 0;
}
