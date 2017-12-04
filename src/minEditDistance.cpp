#include"../include/minEditDistance.h"

namespace jjx
{
int minInThreeNum(int a, int b, int c)
{
	int tmp=(a<b)?a:b;
	return (tmp<c)?tmp:c;
}
//计算由word2变为word1的最小编辑距离
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
	//填矩阵
	for(int i=1; i<length2+1; ++i)
	{
		for(int j=1; j<length1+1; ++j)
		{
			if(word2[i-1]==word1[j-1])edit[i][j]=edit[i-1][j-1];
			else edit[i][j]=minInThreeNum(edit[i-1][j]+1, edit[i][j-1]+1, edit[i-1][j-1]+1);
		}
	}
	return edit[length2][length1];
}
}//end of namespace jjx
