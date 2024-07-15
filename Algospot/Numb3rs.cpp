/*
* Problem: https://algospot.com/judge/problem/read/NUMB3RS
* 확률을 계산하는 문제로, 실수 결과 값의 오차 크기가 일정 이하여야 한다.
* 결과를 출력할 때 소수점 이하 몇 자리까지 표시할 지 지정하지 않아 오차가 허용 범위를 넘어 오답이 발생함.
* 결론: 실수를 출력할 때는 꼭 허용 오차 범위에 맞게 소수점 이하 표시 크기를 조절하자.
*/

#include <stdio.h>
#include <iostream>
#include <memory.h>
#include <cassert>

using namespace std;

int n, d, p;
int path[50][50];
double singleProbability[50][50];
double finalProbability[50][101];

double GetProbability(int town, int daysPassed)
{
	double& ret = finalProbability[town][daysPassed];
	if (ret >= 0.0)
		return ret;

	ret = 0.0;
	for (int i = 0; i < n; ++i)
	{
		if (path[i][town] > 0)
			ret += GetProbability(i, daysPassed - 1) * singleProbability[i][town];
	}
	return ret;
}

int main()
{
	int C;
	cin >> C;
	while (C--)
	{
		cin >> n >> d >> p;

		memset(path, 0, sizeof(path));
		fill_n(&finalProbability[0][0], 50 * 101, -1.0);
		for (int i = 0; i < n; ++i)
		{
			int pathCount = 0;
			for (int j = 0; j < n; ++j)
			{
				cin >> path[i][j];
				pathCount += path[i][j];
			}

			for (int j = 0; j < n; ++j)
			{
				singleProbability[i][j] = pathCount > 0 ? ((double)path[i][j] / pathCount) : 0.0;
			}
		}

		for (int i = 0; i < n; ++i)
			finalProbability[i][1] = singleProbability[p][i];

		int t;
		cin >> t;
		while (t--)
		{
			int target;
			cin >> target;
			printf("%.8lf\n", GetProbability(target, d));
		}
	}
	return 0;
}
