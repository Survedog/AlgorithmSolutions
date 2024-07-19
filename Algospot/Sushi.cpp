/*
* Problem: https://algospot.com/judge/problem/read/SUSHI
* 배낭싸기와 거의 동일한 문제. 최대 예산(m)이 10억이므로, 현재 예산 값을 인덱스로 쓰는 memo 배열이 메모리 제한을 넘기지 않게 하기 위해 m과 모든 가격을 100으로 나눈 후 사용함.
* memo 접근 시 반드시 현재 인덱스(예산) 미만의 위치에만 접근하기 때문에, 부분 문제 간 계산 순서가 정해져 있으므로 반복적 동적 계획법을 적용할 수 있었음.
* 반복적 동적 계획법을 적용하여 메모리를 절약함과 동시에 재귀 함수 호출 오버헤드를 줄일 수 있었다.
*/

#include <iostream>
#include <memory.h>
#include <cassert>
#include <algorithm>

using namespace std;

int n, m, price[20], like[20], memo[10000000];

int GetMaxLikeSum()
{
	for (int budget = 1; budget <= m; budget += 1)
	{
		int& maxLikeSum = memo[budget];

		maxLikeSum = 0;
		for (int menu = 0; menu < n; ++menu)
		{
			if (price[menu] <= budget)
				maxLikeSum = max(maxLikeSum, memo[budget - price[menu]] + like[menu]);
		}
	}

	return memo[m];
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> n >> m;
		m /= 100;

		for (int i = 0; i < n; ++i)
		{
			cin >> price[i] >> like[i];
			price[i] /= 100;
		}

		memset(memo, 0, sizeof(memo));
		cout << GetMaxLikeSum() << "\n";
	}

	return 0;
}