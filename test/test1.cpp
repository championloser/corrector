#include<iostream>
#include<fstream>
#include<string>
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
int main()
{
	ifstream ifs("dictionary_cn.txt");
	string line;
	int num=1;
	while(std::getline(ifs, line) && num<=10)
	{
		cout<<line<<endl;
		++num;
	}
	ifs.close();
	return 0;
}
