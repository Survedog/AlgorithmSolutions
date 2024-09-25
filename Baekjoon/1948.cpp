/*
* Problem: https://acmicpc.net/problem/1948
* 분류: 그래프, 벨만 포드 알고리즘
* 
* 시작점부터 도착점까지의 가장 긴 경로들의 길이와, 그 경로들에 속하는 도로들의 수를 구하는 문제이다.
* 처음엔 도로들의 거리를 음수로 바꾼 뒤 다익스트라를 시도했으나, 거리 값이 음수이면 이미 방문한 정점에 대해
* 더 짧은 경로를 찾는 경우가 생길 수 있기 때문에 다익스트라의 효율이 급격히 떨어져 사용할 수 없었다.
* 때문에 벨만 포드 알고리즘을 대신 사용해 음수로 반전된 그래프에서 가장 짧은 거리를 구한 뒤, 그 값을 반전해 가장 긴 거리를 찾았다.
* 이때 각 정점으로 가기 위한 최단 경로를 구하기 위해 parent 배열을 만든 뒤, 현재 정점으로의 최장 경로들에서의 이전 점들을 기록했다.
* 이후 도착점으로부터 거슬러 올라가며 최장 경로 상의 정점들을 한 번씩만 방문하여, 각 정점에서 부모들로 이어지는 도로들의 수를 세어 두번째 답(최장 경로들을 이루는 도로들의 수)을 찾았다.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

const int INF = 1000000001;
int N, M, start, dest;
vector<pair<int, pair<int, int>>> roads;
vector<vector<int>> parent;
vector<vector<bool>> discovered;

int BellmanFord()
{
	parent.resize(N + 1);
	vector<int> dist(N + 1, INF);
	dist[start] = 0;

	for (int iter = 0; iter < N - 1; ++iter)
	{
		bool updated = false;
		for (auto road : roads)
		{
			int from = road.second.first;
			int to = road.second.second;
			int cost = road.first;

			int newDist = dist[from] + cost;
			if (newDist <= dist[to])
			{
				if (newDist < dist[to])
				{
					parent[to].clear();
					dist[to] = newDist;
					updated = true;
				}
				parent[to].push_back(from);
			}
		}
		if (!updated) break;
	}

	return -dist[dest];
}

int GetNonStopRoadCount()
{
	int ret = 0;
	queue<int> q;
	discovered.resize(N + 1, vector<bool>(N + 1, false));

	q.push(dest);
	while (!q.empty())
	{
		int here = q.front();
		q.pop();

		for (int there : parent[here])
		{
			if (!discovered[here][there])
			{
				ret++;
				discovered[here][there] = true;
				q.push(there);
			}
		}
	}

	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> M;
	for (int i = 0; i < M; ++i)
	{
		int from, to, cost;
		cin >> from >> to >> cost;
		roads.emplace_back(-cost, make_pair(from, to));
	}

	cin >> start >> dest;
	int maxDist = BellmanFord();
	int nonStopCnt = GetNonStopRoadCount();
	cout << maxDist << " " << nonStopCnt << "\n";
	return 0;
}