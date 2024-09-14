/*
* Problem: https://www.acmicpc.net/problem/10251
* 분류: 동적계획법
* 
* 어떤 경로로 가든 이용하는 도로의 개수는 같으므로, 이동 소요 시간은 오로지 방향 전환의 횟수에 따라 결정된다.
* 동적 계획법을 사용해, 최대 curveLeft만큼만 방향 전환을 할 수 있을 때 도착점까지 가는 데 필요한 최소 연료 소모량을 구하는 함수를 만들었다.
* 이후 이분법으로 최소 연료 소모량 <= L인 curveLeft의 최솟값을 찾아 도착점까지의 최소 소요 시간을 계산했다.
* -> 문제 제한 시간이 2초인데 1508 ms가 걸렸다. 더 빠른 알고리즘이 있을 것으로 생각된다.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory.h>

using namespace std;

typedef enum
{
	UP,
	LEFT,
	RIGHT,
	DOWN
} EDirection;

const int INF = 987654321;
int M, N, L, G, fuel[100][100][4], memo[100][100][2][200]; // [y][x][from][CurveCount]

int DFS(int y, int x, EDirection from, int curveLeft)
{
	if (y == M - 1 && x == N - 1) return 0;

	int& ret = memo[y][x][from][curveLeft];
	if (ret != -1) return ret;

	ret = INF;
	if (y + 1 < M)
	{
		if (curveLeft > 0 || from == UP)
			ret = min(ret, fuel[y][x][DOWN] + DFS(y + 1, x, UP, (from == UP ? curveLeft : curveLeft - 1)));
	}
	if (x + 1 < N)
	{
		if (curveLeft > 0 || from == LEFT)
			ret = min(ret, fuel[y][x][RIGHT] + DFS(y, x + 1, LEFT, (from == LEFT ? curveLeft : curveLeft - 1)));
	}
	return ret;
}

int Optimize()
{
	int lo = 1, hi = 200;

	while (lo != hi)
	{
		int mid = (lo + hi) / 2;
		memset(memo, -1, sizeof(memo));

		bool succeeded = (fuel[0][0][DOWN] + DFS(1, 0, UP, mid)) <= G ||
			(fuel[0][0][RIGHT] + DFS(0, 1, LEFT, mid)) <= G;

		if (succeeded)
			hi = mid;
		else
			lo = mid + 1;
	}

	if (hi == 200) return -1;
	return L * (M + N - 2) + hi;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int T;
	cin >> T;
	while (T--)
	{
		cin >> M >> N >> L >> G;
		fill_n(&fuel[0][0][0], 100 * 100 * 2, INF);

		for (int i = 0; i < M; ++i)
			for (int j = 0; j < N - 1; ++j)
			{
				cin >> fuel[i][j][RIGHT];
				fuel[i][j + 1][LEFT] = fuel[i][j][RIGHT];
			}

		for (int i = 0; i < M - 1; ++i)
			for (int j = 0; j < N; ++j)
			{
				cin >> fuel[i][j][DOWN];
				fuel[i + 1][j][UP] = fuel[i][j][DOWN];
			}

		cout << Optimize() << "\n";
	}

	return 0;
}