/*
* Problem: https://www.acmicpc.net/problem/1006
* 분류: 동적계획법
* 
* 시도1) 함께 감시할 수 있는 구역들이 간선으로 연결된 그래프를 만든 뒤 이분 매칭을 사용해 최대한 많은 인접 구역 쌍을 구해 풀려 했으나,
* N이 홀수인 경우 그래프를 이분 그래프로 만들지 못해 실패함.
* 
* 시도2) 타일링 문제와 유사하다는 점을 이용해 동적 계획법을 사용함. 풀이 시 참고한 글: https://kibbomi.tistory.com/128
*/
#include <iostream>
#include <vector>
#include <memory.h>

enum State
{
	UP = 0,
	DOWN,
	BOTH,
	NONE
};

using namespace std;

const int INF = 987654321;
int N, W, memo[10000][4], enemies[10000][2];

inline int RoundCol(int num)
{
	return (num + N) % N;
}

int MinBlocksCovering(int col, State state)
{
	int& ret = memo[col][state];
	if (ret != -1) return ret;

	int prevCol = RoundCol(col - 1);

	bool upPaired = enemies[prevCol][UP] + enemies[col][UP] <= W;
	bool downPaired = enemies[prevCol][DOWN] + enemies[col][DOWN] <= W;

	if (state == BOTH)
	{
		ret = MinBlocksCovering(prevCol, BOTH) + (enemies[col][UP] + enemies[col][DOWN] <= W ? 1 : 2);
		ret = min(ret, MinBlocksCovering(prevCol, UP) + (downPaired ? 2 : 3));
		ret = min(ret, MinBlocksCovering(prevCol, DOWN) + (upPaired ? 2 : 3));
		ret = min(ret, MinBlocksCovering(prevCol, NONE) + (upPaired && downPaired ? 2 : (upPaired || downPaired ? 3 : 4)));
	}
	else if (state == UP)
	{
		ret = MinBlocksCovering(prevCol, BOTH) + 1;
		ret = min(ret, MinBlocksCovering(prevCol, UP) + 2);
		ret = min(ret, MinBlocksCovering(prevCol, DOWN) + (upPaired ? 1 : 2));
		ret = min(ret, MinBlocksCovering(prevCol, NONE) + (upPaired ? 2 : 3));
	}
	else if (state == DOWN)
	{
		ret = MinBlocksCovering(prevCol, BOTH) + 1;
		ret = min(ret, MinBlocksCovering(prevCol, DOWN) + 2);
		ret = min(ret, MinBlocksCovering(prevCol, UP) + (downPaired ? 1 : 2));
		ret = min(ret, MinBlocksCovering(prevCol, NONE) + (downPaired ? 2 : 3));
	}
	else // state == NONE
	{
		ret = MinBlocksCovering(prevCol, BOTH);
	}
	return ret;
}

int GetAnswer()
{
	if (N == 1)
		return enemies[0][UP] + enemies[0][DOWN] <= W ? 1 : 2;

	int answer = INF;
	bool upPaired = enemies[0][UP] + enemies[N - 1][UP] <= W;
	bool downPaired = enemies[0][DOWN] + enemies[N - 1][DOWN] <= W;

	// None
	{
		memset(memo, -1, sizeof(memo));
		memo[0][UP] = 1;
		memo[0][DOWN] = 1;
		memo[0][BOTH] = enemies[0][UP] + enemies[0][DOWN] <= W ? 1 : 2;
		memo[0][NONE] = 0;
		answer = MinBlocksCovering(N - 1, BOTH);
	}

	// Up Placed
	{
		memset(memo, -1, sizeof(memo));
		memo[0][UP] = upPaired ? 1 : 2;
		memo[0][DOWN] = INF;
		memo[0][BOTH] = upPaired ? 2 : 3;
		memo[0][NONE] = INF;

		int curUpEnemies = enemies[0][UP], prevUpEnemies = enemies[N - 1][UP];
		enemies[0][UP] = INF;
		enemies[N - 1][UP] = INF;

		answer = min(answer, MinBlocksCovering(N - 1, BOTH) - 1);

		enemies[0][UP] = curUpEnemies;
		enemies[N - 1][UP] = prevUpEnemies;
	}

	// Down Placed
	{
		memset(memo, -1, sizeof(memo));
		memo[0][UP] = INF;
		memo[0][DOWN] = downPaired ? 1 : 2;
		memo[0][BOTH] = downPaired ? 2 : 3;
		memo[0][NONE] = INF;

		int curDownEnemies = enemies[0][DOWN], prevDownEnemies = enemies[N - 1][DOWN];
		enemies[0][DOWN] = INF;
		enemies[N - 1][DOWN] = INF;

		answer = min(answer, MinBlocksCovering(N - 1, BOTH) - 1);

		enemies[0][DOWN] = curDownEnemies;
		enemies[N - 1][DOWN] = prevDownEnemies;
	}

	// Both Placed
	{
		memset(memo, -1, sizeof(memo));
		memo[0][UP] = INF;
		memo[0][DOWN] = INF;
		memo[0][BOTH] = upPaired && downPaired ? 2 : (upPaired || downPaired ? 3 : 4);
		memo[0][NONE] = INF;

		enemies[0][UP] = INF;
		enemies[0][DOWN] = INF;
		enemies[N - 1][UP] = INF;
		enemies[N - 1][DOWN] = INF;

		answer = min(answer, MinBlocksCovering(N - 1, BOTH) - (enemies[N - 1][UP] + enemies[N - 1][DOWN] <= W ? 1 : 2));
	}

	return answer;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int T;
	cin >> T;
	while (T--)
	{
		cin >> N >> W;
		for (int i = 0; i < N; ++i)
			cin >> enemies[i][DOWN];
		for (int i = 0; i < N; ++i)
			cin >> enemies[i][UP];

		cout << GetAnswer() << "\n";
	}

	return 0;
}