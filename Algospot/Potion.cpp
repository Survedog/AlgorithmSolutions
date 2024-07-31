/*
* Problem: https://algospot.com/judge/problem/read/POTION
* 풀이1: 모든 재료들은 '현재까지 넣은 양/레시피에서의 양' 비율에 대해 '가장 큰 비율 * 레시피에서의 양' 만큼은 넣어야 한다.
* 따라서 해당 비율이 가장 큰 재료를 찾은 후, 다른 재료들에도 그 비율을 곱해 모두 정수가 되는지 확인했다. 이때 하나라도 정수가 되지 않는다면, 방금의 재료를 1개씩 추가하며 다시 비율을 구해 확인했다. 
* 만약 모든 재료가 정수가 되는 비율을 찾았다면, 해당 비율을 모든 재료의 레시피의 양에 곱하여 최종적으로 모든 재료가 얼마만큼 필요한지 계산할 수 있었다.
* 이때 현재 들어간 모든 재료의 양이 레시피의 양보다 적다면, 위 로직을 수행하지 않고 딱 레시피 만큼만 넣도록 했다.
*/
#include <iostream>
#include <cmath>

using namespace std;

int n, r[200], p[200];

inline bool IsNearlyEqual(double A, double B)
{
	return abs(A - B) < 1e-8;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> n;
		for (int i = 0; i < n; ++i)
			cin >> r[i];
		for (int i = 0; i < n; ++i)
			cin >> p[i];

		int maxRatioIdx = -1;
		double maxRatio = -1;
		for (int i = 0; i < n; ++i)
		{
			double ratio = (double)p[i] / r[i];
			if (ratio > maxRatio)
			{
				maxRatio = ratio;
				maxRatioIdx = i;
			}
		}

		if (maxRatio <= 1)
		{
			for (int i = 0; i < n; ++i)
				cout << r[i] - p[i] << " ";
			cout << "\n";
		}
		else
		{
			for (int add = 0; add <= 1000; ++add)
			{
				bool allDivisible = true;
				double ratio = (double)(p[maxRatioIdx] + add) / r[maxRatioIdx];
				for (int ingre = 0; ingre < n; ++ingre)
				{
					double totalNeeded = r[ingre] * ratio;
					allDivisible = IsNearlyEqual(totalNeeded, (int)totalNeeded);
					if (!allDivisible) break;
				}

				if (allDivisible)
				{
					for (int ingre = 0; ingre < n; ++ingre)
						cout << int(r[ingre] * ratio) - p[ingre] << " ";
					cout << "\n";
					break;
				}
			}
		}
	}

	return 0;
}