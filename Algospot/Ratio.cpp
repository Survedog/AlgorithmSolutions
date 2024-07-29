/*
* Problem: https://algospot.com/judge/problem/read/RATIO
* 수치해석 알고리즘인 이분법을 사용해 해결하였다.
* 개선할 점) 함수 곳곳에서 오버플로우가 발생하여 알 수 없는 오류가 발생함. 특히 int형 매개변수에 2^31 범위를 넘어서는 수를 넣어 오버플로우가 발생한 것을 눈치채기 어려웠다.
* 큰 수를 사용하는 경우 오버플로우가 발생하지 않는지 항상 확인하고, 단정문을 사용해 오버플로우 등이 발생하지 않았는지 확인하는 것도 좋을듯 하다.
*/
#include <iostream>
#include <cmath>

using namespace std;

const int MAX_GAMES = 2e9;

inline int GetWinRate(long long N, long long M)
{
	return M * 100 / N;
}

int GetMinGamesLeft(long long N, long long M)
{
	int Z = GetWinRate(N, M);
	long long lo = 0, hi = MAX_GAMES + 5;
	while (lo + 1 < hi)
	{
		long long mid = (lo + hi) / 2;
		if (GetWinRate(N + mid, M + mid) > Z)
			hi = mid;
		else
			lo = mid;
	}
	return hi <= MAX_GAMES ? hi : -1;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int T;
	cin >> T;
	while (T--)
	{
		int N, M;
		cin >> N >> M;		
		cout << GetMinGamesLeft(N, M) << "\n";
	}
	return 0;
}