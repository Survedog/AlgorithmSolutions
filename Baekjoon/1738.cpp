/*
* Problem: https://www.acmicpc.net/problem/1738
* 분류: 벨만 포드 알고리즘
* 
* 모든 간선 비용에 -1을 곱해 반전시킨 뒤 벨만 포드 알고리즘으로 최소 비용 경로를 구했다.
* 이때 다음의 사실들로 인해 오답이 발생했다.
* 1) 그래프에서 음의 사이클이 발생하더라도 그것이 도착점까지의 최소 경로에 영향을 주지 않을 수 있다.
* 2) N번 반복해서 구한 최소 경로가 음의 사이클에 속하지 않더라도, 사이클에서 도착점으로 이동할 수 있다면 결국 최소 경로는 음의 무한대가 된다.
* 3) 2번의 상황이 발생하는지를 그래프의 모든 사이클에 대해 확인해야 한다.
*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 987654321;
int N, M;
vector<vector<int>> weight;

bool DFS(int here, int goal, vector<bool>& visited)
{
	if (here == goal) return true;

	visited[here] = true;
	for (int there = 1; there <= N; ++there)
	{
		if (weight[here][there] == INF || visited[there]) continue;
		if (DFS(there, goal, visited)) return true;
	}
	return false;
}

vector<int> BellmanFord()
{
	vector<int> dist(N + 1, INF);
	vector<int> parent(N + 1, -1);
	dist[1] = 0;

	vector<int> cycleFragment;
	for (int iter = 0; iter < N; ++iter)
	{
		bool updated = false;
		for (int u = 1; u <= N; ++u)
			for (int v = 1; v <= N; ++v)
				if (weight[u][v] != INF && dist[v] > dist[u] + weight[u][v])
				{
					dist[v] = dist[u] + weight[u][v];
					updated = true;
					parent[v] = u;

					if (iter == N - 1) cycleFragment.push_back(v);
				}

		if (!updated) break;
	}

	vector<bool> visited(N + 1, false);
	for (int node : cycleFragment)
		if (DFS(node, N, visited)) return vector<int>();

	vector<int> path;
	int node = N;
	while (node != -1)
	{
		path.push_back(node);
		node = parent[node];
	}

	if (path.empty() || path.back() != 1) return vector<int>();

	reverse(path.begin(), path.end());
	return path;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> M;
	weight = vector<vector<int>>(N + 1, vector<int>(N + 1, INF));
	for (int i = 0; i < M; ++i)
	{
		int u, v, w;
		cin >> u >> v >> w;
		weight[u][v] = min(weight[u][v], -w);
	}

	vector<int> Path = BellmanFord();
	if (Path.empty())
		cout << "-1\n";
	else
	{
		for (int node : Path)
			cout << node << " ";
		cout << "\n";
	}
	return 0;
}