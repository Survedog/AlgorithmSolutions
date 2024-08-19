/*
* Problem: https://algospot.com/judge/problem/read/EDITORWARS
* 상호 배타적 집합 (유니온 파인드) 구조체에 enemy 배열을 추가해 각 집합마다 자신과 반대되는 집합을 저장하도록 했다.
* 그 후 적의 적은 동료라는 논리를 바탕으로 각 댓글의 내용에 맞게 집합들을 합쳐나갔다. 이때 같은 집합 내에서 상호 비방을 하거나, 서로 적대적인 집합끼리 상호 인정을 하는 경우를 모순으로 간주했다.
* 그럼 최종적으론 모든 집합이 최대 하나의 적 집합을 갖게 된다.
* 이렇게 만든 두 개의 서로 적대하는 집합 쌍들에 대해, 서로 다른 쌍에 속하는 집합들은 서로 적대하지 않는다. (이전 단계에서 한 집합에 두 개 이상의 적 집합이 있는 경우 그 적들을 모두 합쳐버렸으므로)
* 따라서 모든 쌍들을 순회하며 둘 중 크기가 더 큰 집합들의 크기를 모두 더하면 댓글의 내용을 만족하며 만들 수 있는 집합의 최대 크기가 나온다.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory.h>

using namespace std;

int N, M;
bool isChecked[10000];

struct DisjointSet
{
	vector<int> parent, rank, size, enemy;

	DisjointSet(int n) : parent(n), rank(n, 1), size(n, 1), enemy(n, -1)
	{
		for (int i = 0; i < n; ++i)
			parent[i] = i;
	}

	int Union(int u, int v)
	{
		if (u == -1 || v == -1) return max(u, v);

		u = Find(u);
		v = Find(v);
		if (u == v) return u;

		if (rank[u] < rank[v])
			swap(u, v);

		parent[v] = u;
		size[u] += size[v];
		if (rank[u] == rank[v]) rank[u]++;
		return u;
	}

	int Find(int u)
	{
		if (u == -1) return -1;
		if (parent[u] == u) return u;
		return parent[u] = Find(parent[u]);
	}

	bool Dis(int u, int v)
	{
		u = Find(u);
		v = Find(v);
		if (u == v) return false;

		if (enemy[u] != -1)
			v = Union(v, enemy[u]);
		if (enemy[v] != -1)
			u = Union(u, enemy[v]);
		enemy[u] = v;
		enemy[v] = u;
		return true;
	}

	bool Ack(int u, int v)
	{
		u = Find(u);
		v = Find(v);
		if (enemy[u] == v || enemy[v] == u)
			return false;

		int merged = Union(u, v);
		if (enemy[u] != enemy[v])
			enemy[merged] = Union(enemy[u], enemy[v]);
		return true;
	}
};

int CalcMaxSize(DisjointSet& dSet)
{
	memset(isChecked, false, N);

	int ret = 0;
	for (int i = 0; i < N; ++i)
	{
		int u = dSet.Find(i);
		int v = dSet.Find(dSet.enemy[u]);

		if (isChecked[u]) continue;

		if (v == -1)
		{
			ret += dSet.size[u];
			isChecked[u] = true;
		}
		else
		{
			ret += max(dSet.size[u], dSet.size[v]);
			isChecked[u] = isChecked[v] = true;
		}
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
		DisjointSet dSet(N);
		int maxSize = -1, maxSizeSet = -1, errorPoint;
		bool isValid = true;

		for (int i = 1; i <= M; ++i)
		{
			int a, b;
			string type;
			cin >> type >> a >> b;

			if (isValid)
			{
				if (type == "ACK")
					isValid = dSet.Ack(a, b);
				else
					isValid = dSet.Dis(a, b);

				if (!isValid)
					errorPoint = i;
			}
		}

		if (isValid)
			cout << "MAX PARTY SIZE IS " << CalcMaxSize(dSet) << "\n";
		else
			cout << "CONTRADICTION AT " << errorPoint << "\n";
	}
	return 0;
}