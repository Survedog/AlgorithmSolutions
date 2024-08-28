/*
* Problem: http://algospot.com/judge/problem/read/DRUNKEN
* 분류: 플로이드 알고리즘
* 
* 경유점을 중심으로 하는 문제이며, 같은 그래프에서 여러 시작점/종료점에 대해 최소 비용을 찾는 것을 볼 때 플로이드 알고리즘을 사용할 수 있을거라 추측할 수 있다.
* 시도) 플로이드 알고리즘을 수행하되, 기존 경로와 새 경유점을 지나는 경로를 비교하는 부분에서 두 경로의 길이에 각각의 가장 긴 지연 시간을 더해서 비교한다.
* 이때 가장 긴 지연 시간은 매번 모든 경유점을 조사할 필요 없이, 매 최소 길이 갱신 시 마다 delay[k], worstDelay[i][k], worstDelay[k][j]을 비교해 가장 긴 것을 저장하면 된다.
* -> i->j로 갈 때 최악 예상 시간이 가장 작은 경로가 i->j->x로 갈 때에는 그대로 사용되지 않는 경우도 있기에 오답이 발생했다. (i->j->x로 갈 땐 i->j로 가는 다른 루트를 사용하는게 더 빠를 수 있음)
* 
* 책 해설) u->v로 가는 최단 경로에서 k가 가장 단속 시간이 큰 경유점이라 하면, 그 경로에서의 최악의 예상 시간은 best_k[u][k] + best_k[k][u] + delay[k]이다.
* (best_k는 단속 시간이 k이하인 정점만 지나는 최단 경로의 이동 시간이다)
* 따라서 모든 k에 대한 위 식의 값 중 최솟값이 우리가 원하는 답이란 것을 알 수 있다.
* 
* best_k를 구하기 위해선 플로이드 알고리즘을 쓰는데, 경유점을 S(경유할 수 있는 정점의 집합)에 추가할 때 1->V의 순서대로 추가하는 대신 단속 시간이 가장 작은 것부터 추가한다.
* 그렇게 하면 각 단계에선 현재 경유점과 그보다 단속 시간이 작은 곳만 경유하며 이동할 때의 최단 거리(=best_k)를 알 수 있다.
* 따라서 플로이드에서 S에 매 경유점을 추가할 때마다 모든 시작/끝 정점 쌍에 대해 최악 예상 시간을 갱신하면, 모든 정점 쌍에 대한 최소의 최악 예상시간을 얻을 수 있다.
*/
#include <iostream>
#include <vector>
#include <memory.h>
#include <algorithm>

using namespace std;

const int INF = 1000000;
int V, E, adj[501][501], W[501][501];
int delay[501];
vector<pair<int, int>> order;

void Floyd()
{
	for (pair<int, int> o : order)
	{
		int k = o.second;
		for (int i = 1; i <= V; ++i)
			for (int j = 1; j <= V; ++j)
			{
				adj[i][j] = min(adj[i][j], adj[i][k] + adj[k][j]);
				W[i][j] = min(W[i][j], adj[i][k] + adj[k][j] + delay[k]);
			}
	}
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> V >> E;
	for (int i = 1; i <= V; ++i)
	{
		cin >> delay[i];
		order.emplace_back(delay[i], i);

		for (int j = 1; j <= V; ++j)
			W[i][j] = adj[i][j] = INF;
		W[i][i] = adj[i][i] = 0;
	}
	sort(order.begin(), order.end());

	for (int i = 0; i < E; ++i)
	{
		int a, b, c;
		cin >> a >> b >> c;
		W[a][b] = adj[a][b] = c;
		W[b][a] = adj[b][a] = c;
	}

	Floyd();

	int C;
	cin >> C;
	while (C--)
	{
		int s, t;
		cin >> s >> t;
		cout << W[s][t] << "\n";
	}
	return 0;
}
