/*
* Problem: https://algospot.com/judge/problem/read/TIMETRIP
* 분류: 벨만 포드 알고리즘
* 벨만 포드 알고리즘을 한번은 그대로, 다른 한번은 모든 간선 가중치에 -1을 곱한채로 사용해 안드로메다로 갈 때의 최소/최대 거리을 구했다.
* 
* 다음의 이유들로 오답이 발생했다.
* 1) 사이클이 있을 때, 안드로메다가 그 사이클에 영향을 받지 않음에도 INFINITY를 출력해버림.
* 2) 안드로메다가 UNREACHABLE일 때도 사이클에 포함되어 있으면 INFINITY로 출력해버림.
* 3) 안드로메다가 사이클에 포함되지 않더라도, 사이클에 영향을 받는 경우에는 INFINITY로 출력해야 하는데 그러지 못함.
*/
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int INF = 987654321, NEGINF = -987654321;
int V, W;
vector<pair<int, int>> adjacent[100];

void NegateWeights()
{
	for (int i = 0; i < V; ++i)
		for (pair<int, int>& p : adjacent[i])
			p.second *= -1;
}

void BellmanFord(int src, vector<int>& upper)
{
	upper.clear();
	upper.resize(V, INF);
	upper[src] = 0;

	vector<int> cycle;
	bool updated = false;
	for (int iter = 0; iter < V; ++iter)
	{
		for (int here = 0; here < V; ++here)
		{
			for (pair<int, int>& adj : adjacent[here])
			{
				int there = adj.first;
				int nextUpper = upper[here] + adj.second;
				if (nextUpper < upper[there])
				{
					upper[there] = nextUpper;
					updated = true;
					if (iter == V - 1) cycle.push_back(there);
				}
			}
		}
		if (!updated) break;
	}

	if (updated)
	{
		queue<int> q;
		for (int i : cycle)
			if (upper[i] <= 1000000)
				q.push(i);

		while (!q.empty())
		{
			int here = q.front(); q.pop();
			upper[here] = NEGINF;

			for (pair<int, int>& adj : adjacent[here])
			{
				int there = adj.first;
				if (upper[there] != NEGINF)
					q.push(there);
			}
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> V >> W;

		for (int i = 0; i < V; ++i)
			adjacent[i].clear();

		for (int i = 0; i < W; ++i)
		{
			int a, b, d;
			cin >> a >> b >> d;
			adjacent[a].emplace_back(b, d);
		}

		vector<int> dist;
		BellmanFord(0, dist);
		if (dist[1] <= -1000001)
			cout << "INFINITY ";
		else if (dist[1] >= 1000001)
		{
			cout << "UNREACHABLE" << "\n";
			continue;
		}
		else cout << dist[1] << " ";

		NegateWeights();
		BellmanFord(0, dist);
		if (dist[1] <= -1000001)
			cout << "INFINITY" << "\n";
		else if (dist[1] >= 1000001)
			cout << "UNREACHABLE" << "\n";
		else
			cout << -dist[1] << "\n";
	}
	return 0;
}
