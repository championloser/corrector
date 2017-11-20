#include<iostream>
using std::cout;
using std::endl;
#define A 0
#define B 0
int main()
{
#if A
	cout<<"A=1"<<endl;
#elif B 
	cout<<"B=1"<<endl;
#else
	cout<<"A B=0"<<endl;
#endif
	return 0;
}
