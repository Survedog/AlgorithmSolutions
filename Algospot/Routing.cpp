/*
* Problem: https://algospot.com/judge/problem/read/ROUTING
* 다익스트라 알고리즘에 가중치들의 합 대신 곱을 사용해서 풀었다.
* 간선들의 가중치는 모두 1이상의 실수이기 때문에, 곱셈 시 반드시 기존 값 이상의 값이 나온다.
* 따라서 이미 방문한 정점을 나중에 다시 발견해도 가중치 곱이 기존보다 작을 수 없으니, 가중치 곱을 구하기 위해 다익스트라를 사용하는건 문제가 없다.
* 
* 배운 점)
* 1. 가중치의 상한이 정해져있지 않아 INF 값에 대충 987654321를 사용했는데, 가중치 곱이 이를 넘어서는 입력이 존재했는지 오답이 발생했다.
*	 float.h에 정의된 DBL_MAX를 INF에 대신 저장하니 무사히 정답이 나왔다.
* 2. 다익스트라 시 우선순위 큐 내 중복 노드를 처리하기 위해 cost가 현재 dist와 같거나 더 클 경우는 생략했는데,
*	 우선순위 큐에 노드를 넣을 때 dist를 바로 갱신했기 때문에, 해당 노드를 방문하는 시점에선 cost가 dist보다 작을 수 없어 모든 노드를 생략하는 문제가 생겼다.
*	 따라서 cost가 현재 dist보다 클 경우만 생략하도록 변경했다.
* 3. 책 해설을 확인하니, 가중치의 값을 모두 로그 값으로 변환하면 덧셈을 사용하는 일반적인 다익스트라 알고리즘으로도 답을 구할 수 있다고 한다.
*	 여러 수들의 곱을 구할 땐 로그의 합을 사용하는 방식도 고려해야겠다.
*/
#include <iostream>
#include <vector>
#include <queue>
#include <float.h>

using namespace std;

int N, M;
const double INF = DBL_MAX;
vector<pair<int, double>> adjacent[10000];

double Dijkstra(int src)
{
	vector<double> dist(N, INF);
	priority_queue<pair<double, int>> pq;

	dist[0] = 1;
	pq.emplace(-1, src);

	while (!pq.empty())
	{
		double cost = -pq.top().first;
		int here = pq.top().second;
		pq.pop();

		if (cost > dist[here]) continue;
		if (here == N - 1) return cost;

		for (pair<int, double> adj : adjacent[here])
		{
			int there = adj.first;
			double nextDist = cost * adj.second;
			if (nextDist < dist[there])
			{
				dist[there] = nextDist;
				pq.emplace(-nextDist, there);
			}
		}
	}
	return INF;
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
			adjacent[i].clear();

		for (int i = 0; i < M; ++i)
		{
			int a, b;
			double c;
			cin >> a >> b >> c;
			adjacent[a].emplace_back(b, c);
			adjacent[b].emplace_back(a, c);
		}

		cout << Dijkstra(0) << "\n";
	}
	return 0;
}