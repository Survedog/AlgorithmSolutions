/*
* Problem: https://algospot.com/judge/problem/read/TPATH
* 분류: 크루스칼(+유니온 파인드), 스위핑 알고리즘
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

const double INF = 1001;
int N, M;
vector<pair<int, pair<int, int>>> edges;

struct DisjointSet
{
	vector<int> parent, rank;

	DisjointSet(int n) : parent(n, -1), rank(n, 1) {}

	int Find(int u)
	{
		if (parent[u] == -1) return u;
		return parent[u] = Find(parent[u]);
	}

	bool Union(int u, int v)
	{
		u = Find(u); v = Find(v);
		if (u == v) return false;

		if (rank[u] < rank[v]) swap(u, v);

		parent[v] = u;
		if (rank[u] == rank[v]) rank[u]++;
		return true;
	}
};

bool CheckConnected(int lo, int hi)
{
	DisjointSet dSet(N);

	for (int e = lo; e <= hi; ++e)
	{
		int u = edges[e].second.first;
		int v = edges[e].second.second;

		if (dSet.Union(u, v))
		{
			if (dSet.Find(0) == dSet.Find(N - 1))
				return true;
		}
	}

	return false;
}

int GetMinMaxDiff()
{
	int ret = INF, hi = 0;
	for (int lo = 0; lo < M; ++lo)
		while (hi < M)
		{
			if (CheckConnected(lo, hi))
			{
				ret = min(ret, edges[hi].first - edges[lo].first);
				break;
			}
			hi++;
		}
	return ret;
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

		edges.clear();
		for (int i = 0; i < M; ++i)
		{
			int a, b, c;
			cin >> a >> b >> c;
			edges.emplace_back(c, make_pair(a, b));
		}
		sort(edges.begin(), edges.end());

		cout << GetMinMaxDiff() << "\n";
	}
	return 0;
}
