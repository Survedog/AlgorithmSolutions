/*
* Problem: https://www.acmicpc.net/problem/1738
* 분류: 벨만 포드 알고리즘
*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 987654321;
int N, M;
vector<vector<int>> weight;

vector<int> BellmanFord()
{
	vector<int> dist(N + 1, INF);
	vector<int> parent(N + 1, -1);
	dist[1] = 0;

	for (int iter = 0; iter < N - 1; ++iter)
	{
		bool updated = false;
		for (int u = 1; u <= N; ++u)
			for (int v = 1; v <= N; ++v)
				if (weight[u][v] != INF && dist[v] > dist[u] + weight[u][v])
				{
					dist[v] = dist[u] + weight[u][v];
					updated = true;
					parent[v] = u;
				}

		if (!updated) break;
	}

	for (int u = 1; u <= N; ++u)
		for (int v = 1; v <= N; ++v)
			if (weight[u][v] != INF && dist[v] > dist[u] + weight[u][v])
				return vector<int>();

	vector<int> path;
	int node = N;
	while (node != -1)
	{
		path.push_back(node);
		node = parent[node];
	}

	if (path.back() != 1)
		return vector<int>();

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