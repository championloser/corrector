#include<iostream>
#include<fstream>
using std::cout;
using std::endl;
using std::ifstream;
int main()
{
	ifstream ifs("jiangjiaxian");
	if(ifs.is_open())
	{
		cout<<"true"<<endl;
	}
	else cout<<"false"<<endl;
	ifs.close();
	cout<<"close()"<<endl;
	return 0;
}
