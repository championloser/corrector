#include<iostream>
#include<queue>
#include<string>
#include<vector>
using std::priority_queue;
using std::endl;
using std::cout;
using std::vector;
using std::string;
typedef struct
{
	string _word;
	int _minDistance;
	int _frequency;
}WordItem;
class cmp
{
public:
	bool operator()(WordItem &lhs, WordItem &rhs)
	{
		if(lhs._minDistance>rhs._minDistance)return true;//最小编辑距离越小，优先级越高
		else if(lhs._minDistance<rhs._minDistance)return false;
		else return lhs._frequency<rhs._frequency;//最小编辑距离相同， 词频越高，优先级越高
	}
};
int main()
{
	priority_queue<WordItem, vector<WordItem>, cmp> priQue;
	WordItem wItem;
	for(int i=0; i<10; ++i)
	{
		wItem._word="jiang";
		wItem._minDistance=i;
		wItem._frequency=i;
		priQue.push(wItem);
	}
	for(int i=0; i<10; ++i)
	{
		cout<<priQue.top()._word<<" "<<priQue.top()._minDistance<<" "<<priQue.top()._frequency<<endl;
		priQue.pop();
	}
}
