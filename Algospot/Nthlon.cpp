/*
* Problem: https://algospot.com/judge/problem/read/NTHLON
* 분류: 다익스트라 알고리즘
*
* 종목들을 어떤 순서로 여는지가 아닌, 각 종목을 몇 번 수행하는지만이 중요하다는 것은 쉽게 알 수 있다.
  diff[i] = i번째 종목에 대한 A의 예상 기록 - B의 예상 기록, count[i] = i번째 종목의 수행 횟수, cost[i] = A[i]라 하면 문제를 아래와 같이 변형할 수 있다.
  count[0] * diff[0] + count[1] * diff[1] + ... + count[n - 1] * diff[n - 1] = 0을 만족하는 count에 대해, count[0] * cost[0] + ... + count[n - 1] * cost[n - 1]의 최솟값을 구하라.
  
* 종목 i를 코스에 추가했을 때 diff[i]의 시간 차가 발생했다면, diff의 합이 -diff[i]이 되도록 다른 종목들을 추가해 0으로 만들어야 한다.
* 우리는 종목들의 diff 합 사이의 관계를 그래프로 표현한 뒤, 다익스트라 알고리즘을 통해 종목들의 diff 합이 m이 되기 위한 가중치 합의 최소치를 구할 것이다.

* -199 <= diff[i] <= 199이므로, [-199, 199] 범위의 모든 정수를 노드로 두고, 모든 노드 here에 대해 here에서 here + diff[i]로 가며 cost[i]의 가중치를 갖는 간선을 만든다.
* 이제 0에서부터 시작하는 다익스트라 알고리즘을 수행하면 모든 노드로 가는데 필요한 가중치 합 dist를 구할 수 있다.
* 이때 dist[m]은 종목들의 diff의 합이 m이 되기 위한 가중치 합의 최솟값을 나타낸다.

* 우리는 코스에 추가한 모든 종목들의 diff의 합이 0이 될 때의 가중치 합의 최솟값을 찾아야 한다. (단, dist[0]은 0을 다익스트라의 시작점으로 쓰기 위해 0으로 고정시켰으므로 답이 되지 않음)
* 종목 i를 포함하는 코스의 diff 합이 0이 되기 위한 가중치 합의 최솟값은 cost[i] + dist[-diff[i]]이란 것을 알 수 있다.
* 모든 종목에 대해 이 값을 구해 그 중 최솟값을 찾으면 해당 값이 답이 된다.
 
* 주의) 실제 구현에선 dist에 음수 인덱스를 적용할 수 없으므로, 인덱스에 199를 더해 사용했음.
* 배운점) 책 구현에선 diff가 0인 노드를 2개를 만들어 하나는 텅 빈 코스를 나타내는 시작점, 하나는 완성된 코스인 종착점으로 활용했다.
		 이렇게 하면 종착점까지의 거리가 바로 답이 되므로 더 간편하다.
*/
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int INF = 987654321;
int M, diff[500], cost[500];
vector<pair<int, int>> adjacent[399];

void Dijkstra(int src, vector<int>& dist)
{
	dist.clear();
	dist.resize(399, INF);
	priority_queue<pair<int, int>> pq;

	dist[src] = 0;
	pq.emplace(0, src);

	while (!pq.empty())
	{
		int d = -pq.top().first;
		int here = pq.top().second;
		pq.pop();

		if (d > dist[here]) continue;

		for (pair<int, int> adj : adjacent[here])
		{
			int there = adj.first;
			int nextD = d + adj.second;
			if (nextD < dist[there])
			{
				dist[there] = nextD;
				pq.emplace(-nextD, there);
			}
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> M;

		for (int i = 0; i <= 398; ++i)
			adjacent[i].clear();

		for (int i = 0; i < M; ++i)
		{
			int a, b;
			cin >> a >> b;
			diff[i] = a - b;
			cost[i] = a;
		}

		for (int here = 0; here <= 398; ++here)
			for (int j = 0; j < M; ++j)
			{
				int there = here + diff[j];
				if (there >= 0 && there <= 398)
					adjacent[here].emplace_back(there, cost[j]);
			}

		vector<int> dist;
		Dijkstra(199, dist);

		int answer = INF;
		for (int i = 0; i < M; ++i)
			answer = min(answer, dist[-diff[i] + 199] + cost[i]);

		if (answer == INF)
			cout << "IMPOSSIBLE" << "\n";
		else
			cout << answer << "\n";
	}
	return 0;
}
