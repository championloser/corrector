#include"../include/minEditDistance.h"
#include<iostream>
#include<string>
using std::cout;
using std::endl;
using std::string;
using jjx::minEditDistance;
int main()
{
	string str1="姜家贤";
	string str2="abc姜家贤";
	cout<<minEditDistance(str1, str2)<<endl;
	return 0;
}
