/*
* Problem: https://algospot.com/judge/problem/read/LAN
* 분류: 크루스칼 & 상호 배타적 집합
*
* 각 건물들과 케이블을 그래프로 나타낸 후 최소 스패닝 트리(단, 이미 이어진 케이블은 유지하면서)의 가중치 합을 구하면 될 것이다.
* 크루스칼과 프림 알고리즘을 사용할 수 있는데, 현재 그래프에선 이미 서로 이어진 건물들이 존재하므로 시작 단계에선 연결 그래프가 아닐 수도 있다.
* 따라서 프림 알고리즘을 사용하기 어려워보이므로 크루스칼 알고리즘을 사용하기로 했다. 이때 상호 배제 집합은 Union-Find 알고리즘을 통해 구현했다.
* 
* 크루스칼 알고리즘을 해결하기는 했으나, 평균 풀이 시간보다 훨씬 긴 시간인 556ms가 나왔다.
*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int N, M, x[500], y[500];
vector<pair<double, int>> connected[500];

struct DisjointSet
{
	vector<int> parent, rank;

	DisjointSet(int n) : parent(n, -1), rank(n, 1) {}

	int Find(int u)
	{
		if (parent[u] < 0) return u;
		return parent[u] = Find(parent[u]);
	}

	bool Union(int u, int v)
	{
		u = Find(u); v = Find(v);
		if (u == v) return false;

		if (rank[u] > rank[v])
			parent[v] = u;
		else if (rank[u] < rank[v])
			parent[u] = v;
		else
		{
			parent[v] = u;
			rank[u]++;
		}
		return true;
	}
};

inline double GetDistance(int u, int v)
{
	double delX = x[u] - x[v];
	double delY = y[u] - y[v];
	return hypot(delX, delY);
}

double Kruskal()
{
	DisjointSet dSet(N);
	for (int u = 0; u < N; ++u)
	{
		for (int i = 0; i < connected[u].size(); ++i)
		{
			int v = connected[u][i].second;
			dSet.Union(u, v);
		}
	}

	vector<pair<double, pair<int, int>>> edges;
	for (int u = 0; u < N; ++u)
		for (int v = 0; v < N; ++v)
			if (u != v) edges.emplace_back(GetDistance(u, v), make_pair(u, v));

	sort(edges.begin(), edges.end());

	double ret = 0;
	for (int i = 0; i < edges.size(); ++i)
	{
		int u = edges[i].second.first;
		int v = edges[i].second.second;
		double cost = edges[i].first;

		if (dSet.Union(u, v)) ret += cost;
	}
	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout << fixed;
	cout.precision(10);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> N >> M;
		for (int i = 0; i < N; ++i)
		{
			cin >> x[i];
			connected[i].clear();
		}

		for (int i = 0; i < N; ++i)
			cin >> y[i];

		for (int i = 0; i < M; ++i)
		{
			int a, b;
			cin >> a >> b;
			double distance = GetDistance(a, b);
			connected[a].emplace_back(distance, b);
			connected[b].emplace_back(distance, a);
		}

		cout << Kruskal() << "\n";
	}
	return 0;
}
