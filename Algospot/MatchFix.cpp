/*
* Problem: https://algospot.com/judge/problem/read/MATCHFIX
* 분류: 네트워크 유량 (포드-풀커슨, 에드몬드-카프 알고리즘)
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <memory.h>

using namespace std;

const int MAX_N = 12, MAX_M = 100, SOURCE = 0, SINK = 1, INF = 10000;
const int NODES = 2 + MAX_M + MAX_N;
int N, M, wins[MAX_N], match[MAX_M][2], capacity[NODES][NODES], flow[NODES][NODES];
int maxFlow;

inline int GetMatchNode(int match) { return match + 2; }
inline int GetPlayerNode(int player) { return 2 + M + player; }

int GetMaxFlow()
{
	while (true)
	{
		queue<int> q;
		vector<int> parent(NODES, -1);

		parent[SOURCE] = SOURCE;
		q.push(SOURCE);

		while (!q.empty() && parent[SINK] == -1)
		{
			int here = q.front(); q.pop();
			for (int there = 0; there < NODES; ++there)
			{
				if (parent[there] == -1 && capacity[here][there] - flow[here][there] > 0)
				{
					parent[there] = here;
					q.push(there);
				}
			}
		}

		if (parent[SINK] == -1) break;

		int node = SINK, minResidual = INF;
		while (node != SOURCE)
		{
			int residual = capacity[parent[node]][node] - flow[parent[node]][node];
			minResidual = min(minResidual, residual);
			node = parent[node];
		}
		maxFlow += minResidual;

		node = SINK;
		while (node != SOURCE)
		{
			flow[parent[node]][node] += minResidual;
			flow[node][parent[node]] -= minResidual;
			node = parent[node];
		}
	}
	return maxFlow;
}

void InitGraph()
{
	maxFlow = 0;
	memset(flow, 0, sizeof(flow));
	memset(capacity, 0, sizeof(capacity));

	for (int i = 0; i < M; ++i)
	{
		int matchNode = GetMatchNode(i);
		capacity[SOURCE][matchNode] = 1;

		for (int j = 0; j < 2; ++j)
		{
			int playerNode = GetPlayerNode(match[i][j]);
			capacity[matchNode][playerNode] = 1;
		}
	}
}

bool CanWinWith(int totalWins)
{
	if (*max_element(wins + 1, wins + N) >= totalWins)
		return false;

	for (int i = 0; i < N; ++i)
	{
		int maxWin = i == 0 ? totalWins : totalWins - 1;
		int playerIdx = GetPlayerNode(i);
		capacity[playerIdx][SINK] = maxWin - wins[i];
	}

	return GetMaxFlow() == M && flow[GetPlayerNode(0)][SINK] == capacity[GetPlayerNode(0)][SINK];
}

int GetAnswer()
{
	InitGraph();
	for (int i = wins[0]; i <= wins[0] + M; ++i)
		if (CanWinWith(i)) return i;
	return -1;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> N >> M;
		for (int i = 0; i < N; ++i)
			cin >> wins[i];
		for (int i = 0; i < M; ++i)
		{
			int a, b;
			cin >> a >> b;
			match[i][0] = a;
			match[i][1] = b;
		}

		cout << GetAnswer() << "\n";
	}
	return 0;
}
