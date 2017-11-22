#include<iostream>
#include<string>
using std::string;
using std::cout;
using std::endl;
using std::cin;
void printMatrix(int **p, int row, int col)//for debug
{
	for(int i=0; i<row; ++i)
	{
		for(int j=0; j<col; ++j)
		{
			cout<<p[i][j]<<" ";
		}
		cout<<endl;
	}
}
int minInThreeNum(int a, int b, int c)
{
	int tmp=(a<b)?a:b;
	return (tmp<c)?tmp:c;
}
//求由word2变为word1的最小编辑距离
int minEditDistance(const char *word1, int length1, const char *word2, int length2)
{
	int edit[50][50];
	//初始化矩阵
	//edit[0][0]=0
	//edit[0][j]=j
	//edit[i][0]=i
	for(int i=0; i<length2+1; ++i)
	{
		edit[i][0]=i;
	}
	for(int j=0; j<length1+1; ++j)
	{
		edit[0][j]=j;
	}
	//printMatrix(edit, length2+1, length1+1);
	//cout<<endl;
	//填矩阵
	for(int i=1; i<length2+1; ++i)
	{
		for(int j=1; j<length1+1; ++j)
		{
			if(word2[i-1]==word1[j-1])edit[i][j]=edit[i-1][j-1];
			else edit[i][j]=minInThreeNum(edit[i-1][j]+1, edit[i][j-1]+1, edit[i-1][j-1]+1);
		}
	}
	//printMatrix(edit, length2+1, length1+1);
	return edit[length2][length1];
}
int main()
{
	string word1;
	string word2;
	cout<<"please input word1: ";
	cin>>word1;
	cout<<"please input word2: ";
	cin>>word2;
	cout<<minEditDistance(word1.c_str(), word1.size(), word2.c_str(), word2.size())<<endl;
	return 0;
}
