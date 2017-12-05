#include<iostream>
#include<regex>
#include<string>
using std::cout;
using std::endl;
using std::regex_replace;
using std::string;
int main()
{
	std::regex reg("\\pP");
	string str="测试<>《》！*(^)$%~!@#$…&%￥—+=、。，；‘’“”：·`文本";
	cout<<str<<endl;
	str=regex_replace(str, reg, "");
	cout<<str<<endl;
	return 0;
}
