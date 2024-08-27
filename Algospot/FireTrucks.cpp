/*
* Problem: https://algospot.com/judge/problem/read/ROUTING
* 분류: 다익스트라 알고리즘
* 
* 처음엔 모든 소방서에서 다익스트라 알고리즘을 수행해 각 화재 현장까지의 거리 중 최소값을 구하는 방식을 생각했다.
* 그러나 E의 최댓값이 약 50만, logV의 최댓값이 거진 10으로, 만약 소방서가 999개 주어진다면 무조건 시간 초과가 발생할 것이라 생각했다.
* 
* 한 번의 다익스트라로 모든 소방서로부터 화재현장까지의 최소 거리를 구할 수 없는지 생각해보았다.
* 모든 소방서들을 다익스트라 알고리즘의 시작점으로 두고, dist 배열에는 소방서의 구분 없이 한 지점에 대한 모든 소방서로부터의 거리 중 가장 가까운 것을 저장한다.
* 이후 우선순위 큐에 미방문 지점에 대한 거리를 넣어가며, 그 거리가 가장 작은 지점부터 방문해나갈것이다.
* 
* 귀류법으로 정당성을 증명하기 위해, 소방서 A에서 경로 p를 따라 x 지점에 최초로 방문했을 때, A나 다른 소방서들로부터 경로 q를 따라 x로 가는 것이 더 가까운 경우가 있다고 가정해보자.
* 현재 경로 p에서의 x 바로 이전의 점을 u, 경로 q에서 아직 방문하지 않은 지점들 중 가장 처음 만나는 것을 v라고 하자.
* 경로 p보다 q가 x에 더 가깝다는 것은 dist[u] + w[u->x] > dist[v] + (경로 상 v->x의 거리)인 것이고, 여기서 dist[u] + w[u->x] > dist[v]임을 알 수 있다.
* 이때 dist[v]가 더 작다는 것은 x보다 v가 먼저 방문되어야 한다는 건데, v는 아직 방문하지 않은 지점이므로 모순이 발생한다.
* 따라서 경로 p는 모든 소방서에서 x로 가는 경로 중 가장 가까운 경로라는 것을 알 수 있고, 이 알고리즘을 따라 모든 지점을 순서대로 방문하면 모든 소방서로부터의 최소 거리를 알 수 있게 된다.
* 
* 위 알고리즘을 통해 문제를 무사히 해결할 수 있었다.
*/
#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

const int FIRE_STATION = 1, FIRE_SITE = 2, OTHER = 0;

int V, E, N, M;
const int INF = INT_MAX;
vector<pair<int, int>> adjacent[1001];
vector<int> type;

void Dijkstra(vector<int>& dist)
{
	dist.clear();
	dist.resize(V + 1, INF);
	priority_queue<pair<int, int>> pq;

	for (int i = 1; i <= V; ++i)
		if (type[i] == FIRE_STATION)
		{
			dist[i] = 0;
			pq.emplace(0, i);
		}

	while (!pq.empty())
	{
		int here = pq.top().second;
		int cost = -pq.top().first;
		pq.pop();

		if (cost > dist[here]) continue;

		for (pair<int, int> adj : adjacent[here])
		{
			int there = adj.first;
			int nextCost = cost + adj.second;
			if (nextCost < dist[there])
			{
				dist[there] = nextCost;
				pq.emplace(-nextCost, there);
			}
		}
	}
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
		cin >> V >> E >> N >> M;

		for (int i = 0; i <= V; ++i)
			adjacent[i].clear();

		for (int i = 0; i < E; ++i)
		{
			int a, b, t;
			cin >> a >> b >> t;
			adjacent[a].emplace_back(b, t);
			adjacent[b].emplace_back(a, t);
		}

		type.clear();
		type.resize(V + 1, OTHER);
		for (int i = 0; i < N; ++i)
		{
			int fireSite;
			cin >> fireSite;
			type[fireSite] = FIRE_SITE;
		}

		for (int i = 0; i < M; ++i)
		{
			int fireStation;
			cin >> fireStation;
			type[fireStation] = FIRE_STATION;
		}

		vector<int> dist;
		Dijkstra(dist);

		int sum = 0;
		for (int i = 1; i <= V; ++i)
			if (type[i] == FIRE_SITE)
				sum += dist[i];

		cout << sum << "\n";
	}
	return 0;
}