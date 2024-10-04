/*
* Problem: https://www.acmicpc.net/problem/1006
* 분류: 이분 매칭?
* 
* 시도1) 함께 감시할 수 있는 구역들이 간선으로 연결된 그래프를 만든 뒤 이분 매칭을 사용해 최대한 많은 인접 구역 쌍을 구해 풀려 했으나,
* N이 홀수인 경우 그래프를 이분 그래프로 만들지 못해 실패함.
*/
#include <iostream>
#include <vector>

using namespace std;

int N, W, aCnt, bCnt;
vector<int> enemies, aMatch, bMatch, idx2Team;
vector<bool> adjacent[20001];

inline int SwitchTeam(int team)
{
	return team == 0 ? 1 : 0;
}

bool DFS(vector<bool>& visited, int a)
{
	if (visited[a]) return false;
	visited[a] = true;

	for (int b : adjacent[a])
	{
		if (bMatch[b] == -1 || DFS(visited, bMatch[b]))
		{
			aMatch[a] = b;
			bMatch[b] = a;
		}
	}
}

int BipartiteMatch()
{
	aMatch = bMatch = vector<int>(2 * N + 1, -1);

	for (int a = 0; a < N)
	{
		vector<bool visited()
	}
}

void MakeGraph()
{
	for (int i = 1; i <= 2 * N; ++i)
		adjacent[i].clear();
	idx2Team.clear();
	idx2Team.resize(2 * N + 1, -1);

	idx2Team[1] = 0;
	aCnt = bCnt = 0;
	for (int i = 1; i <= N; ++i)
	{
		int outer = i + N;
		int prev = (i + N - 2) % N + 1;
		int next = i % N + 1;

		idx2Team[next] = idx2Team[prev] = idx2Team[outer] = SwitchTeam(idx2Team[i]);

		if (enemies[i] + enemies[outer] <= W)
		{
			adjacent[i].push_back(outer);
			adjacent[outer].push_back(i);
		}

		if (enemies[i] + enemies[prev] <= W)
			adjacent[i].push_back(prev);

		if (enemies[i] + enemies[next] <= W)
			adjacent[i].push_back(next);
	}

	for (int i = N + 1; i <= 2 * N; ++i)
	{
		int prev = (i - 2) % N + N + 1;
		int next = i % N + N + 1;

		if (enemies[i] + enemies[prev] <= W)
			adjacent[i].push_back(prev);

		if (enemies[i] + enemies[next] <= W)
			adjacent[i].push_back(next);
	}
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
		enemies.clear();
		enemies.resize(2 * N + 1, 0);
		for (int i = 1; i <= 2 * N; ++i)
		{
			cin >> enemies[i];
		}

		MakeGraph();
		cout << 2 * N - BipartiteMatch() << "\n";
	}

	return 0;
}