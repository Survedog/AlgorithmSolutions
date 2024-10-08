/*
* Problem: https://algospot.com/judge/problem/read/LAN
* 분류: (크루스칼 & 상호 배타적 집합) or 프림
*
* 각 건물들과 케이블을 그래프로 나타낸 후 최소 스패닝 트리(단, 이미 이어진 케이블은 유지하면서)의 가중치 합을 구하면 될 것이다.
* 크루스칼과 프림 알고리즘을 사용할 수 있는데, 현재 그래프에선 이미 서로 이어진 건물들이 존재하므로 시작 단계에선 연결 그래프가 아닐 수도 있다.
* 따라서 프림 알고리즘을 사용하기 어려워보이므로 크루스칼 알고리즘을 사용하기로 했다. 이때 상호 배제 집합은 Union-Find 알고리즘을 통해 구현했다.
* -> 해결하기는 했으나, 수행 시간은 평균 풀이 시간보다 훨씬 긴 556ms로 나왔다. (전전 커밋)
* 
* 책의 해설을 보니 이미 연결된 건물들 사이의 거리를 0으로 두면 별 다른 처리 없이도 최소 스패닝 트리 알고리즘을 적용할 수 있다는 것을 알 수 있었다.
* 이것을 적용시켜 프림 알고리즘을 적용하였다. 프림 알고리즘은 밀집 배열에서 O(E)에 가까운 성능을 보이기 때문에 더 빠른 시간 내에 풀 수 있을 것이라 생각했다.
* -> 프림 알고리즘을 사용해 푸니 112ms가 나왔다. (이전 커밋)
* 
* 우선순위 큐를 사용한 프림 알고리즘을 사용해보니 보았으나, 124ms로 이전보다 오히려 느린 시간이 나왔다. (현재 커밋)
* 이는 우선 순위 큐를 사용했을 때의 시간 복잡도가 O(ElogV)이기 때문인 것으로 보인다.
* 
* 배운점) 우선순위 큐를 사용한 프림은 다익스트라 알고리즘과 비슷하지만, 우선순위 큐에 시작점부터의 거리가 아닌 부모로부터의 거리를 넣는다.
* 때문에 이미 방문된 정점에 대해 더 가까운 정점이 나중에 발견될 수도 있으므로, weight > minWeight 대신 visited 배열을 사용해 중복 방문을 막아야 한다.
* 그렇지 않으면 사이클이 발생할 수 있고, 결국 트리가 만들어지지 않을 수 있다.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

const double INF = 987654321;
int N, M, x[500], y[500];
double adj[500][500];

inline double GetDistance(int u, int v)
{
	double delX = x[u] - x[v];
	double delY = y[u] - y[v];
	return hypot(delX, delY);
}

double Prim()
{
	priority_queue<pair<double, int>> pq;
	vector<double> minWeight(N, INF);
	vector<bool> added(N, false);

	minWeight[0] = 0;
	pq.emplace(0, 0);

	double ret = 0;
	while (!pq.empty())
	{
		int u = pq.top().second;
		double weight = -pq.top().first;
		pq.pop();

		if (added[u]) continue;
		added[u] = true;
		ret += weight;

		for (int v = 0; v < N; ++v)
		{
			if (u == v || added[v]) continue;
			double newWeight = adj[u][v];
			if (newWeight < minWeight[v])
			{
				minWeight[v] = newWeight;
				pq.emplace(-newWeight, v);
			}
		}
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
			cin >> x[i];
		for (int i = 0; i < N; ++i)
			cin >> y[i];

		for (int u = 0; u < N; ++u)
			for (int v = 0; v < N; ++v)
				if (u != v) adj[u][v] = GetDistance(u, v);

		for (int i = 0; i < M; ++i)
		{
			int a, b;
			cin >> a >> b;
			adj[a][b] = adj[b][a] = 0;
		}

		cout << Prim() << "\n";
	}
	return 0;
}
