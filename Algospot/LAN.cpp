/*
* Problem: https://algospot.com/judge/problem/read/LAN
* 분류: 크루스칼 or 프림 & 상호 배타적 집합
*
* 각 건물들과 케이블을 그래프로 나타낸 후 최소 스패닝 트리(단, 이미 이어진 케이블은 유지하면서)의 가중치 합을 구하면 될 것이다.
* 크루스칼과 프림 알고리즘을 사용할 수 있는데, 현재 그래프에선 이미 서로 이어진 건물들이 존재하므로 시작 단계에선 연결 그래프가 아닐 수도 있다.
* 따라서 프림 알고리즘을 사용하기 어려워보이므로 크루스칼 알고리즘을 사용하기로 했다. 이때 상호 배제 집합은 Union-Find 알고리즘을 통해 구현했다.
* -> 해결하기는 했으나, 평균 풀이 시간보다 훨씬 긴 시간인 556ms가 나왔다. (이전 커밋)
* 
* 책의 해설을 보니 이미 연결된 건물들 사이의 거리를 0으로 두면 별 다른 처리 없이도 최소 스패닝 트리 알고리즘을 적용할 수 있다는 것을 알 수 있었다.
* 이것을 적용시켜 프림 알고리즘을 적용하였다. 프림 알고리즘은 밀집 배열에서 더 좋은 성능을 보이기 때문에 더 빠른 시간 내에 풀 수 있을 것이라 생각했다.
* -> 프림 알고리즘을 사용해 푸니 112ms가 나왔다. (현재 커밋)
*/
#include <iostream>
#include <vector>
#include <algorithm>

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
	vector<double> minWeight(N, INF);
	vector<bool> added(N, false);

	double ret = 0;
	minWeight[0] = 0;

	for (int iter = 0; iter < N; ++iter)
	{
		int u = -1;
		for (int v = 0; v < N; ++v)
			if (!added[v] && (u == -1 || minWeight[u] > minWeight[v]))
				u = v;

		added[u] = true;
		ret += minWeight[u];

		for (int v = 0; v < N; ++v)
		{
			if (!added[v] && adj[u][v] < minWeight[v])
				minWeight[v] = adj[u][v];
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
