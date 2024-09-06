/*
* Problem: https://algospot.com/judge/problem/read/MATCHFIX
* 분류: 네트워크 유량 (포드-풀커슨, 에드몬드-카프 알고리즘)
* 
* 책 해설) 유량 그래프로 이를 표현하기 위해 경기들과 선수들을 그래프의 노드로 만든다. 이때 간선의 용량은 전달할 수 있는 승 수를 의미한다.
* 소스에선 매 경기에 용량이 1인 간선을 연결하고, 경기에선 승패에 따라 승 수를 얻을 수 있는 선수들에게 승 수를 전달한다.
* 마지막으로 선수들은 싱크에 승 수를 전달하는데, 이때 0번 선수를 우승시키기 위해, 0번을 제외한 선수들은 0번의 승 수 k보다 1 작은 승 수만 전달하도록 용량을 조절한다.
* 이 그래프에 포드-풀커슨 알고리즘을 사용하면 0번 선수를 k 승 수로 우승시키기 위해 진행할 수 있는 최대 매치 개수가 나온다.

* 이제 k를 올려가며, 모든 매치를 진행했을 때 0번 선수가 우승할 수 있는 최소 k를 찾으면 그것이 정답이 된다.
* 이때 k를 올려도 용량이 줄어드는 간선은 없기 때문에(선수->싱크 간선들의 용량이 1씩 늘어나기만 함), 매 k마다 포드-풀커슨 알고리즘을 새로 수행할 필요 없이 기존의 유량에서 그대로 수행하면 시간을 절약할 수 있다.
* 
* 책에 적힌 알고리즘만으론 매치 편성 때문에 0번 선수에게 승을 줄 수 없어 질 수 밖에 없는 경우를 잡아낼 수 없다.
* 이런 경우 flow 배열을 확인하여 0번 선수가 실제로 특정 승 수를 따낼 수 있었는지 확인하면 위의 경우를 처리할 수 있다.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <memory.h>

using namespace std;

const int MAX_N = 12, MAX_M = 100, SOURCE = 0, SINK = 1, INF = 10000;
const int NODES = 2 + MAX_M + MAX_N;
int N, M, wins[MAX_N], match[MAX_M][2], capacity[NODES][NODES], flow[NODES][NODES];
int maxFlow;

inline int GetMatchNode(int match) { return match + 2; }
inline int GetPlayerNode(int player) { return 2 + M + player; }

int GetMaxFlow()
{
	while (true)
	{
		queue<int> q;
		vector<int> parent(NODES, -1);

		parent[SOURCE] = SOURCE;
		q.push(SOURCE);

		while (!q.empty() && parent[SINK] == -1)
		{
			int here = q.front(); q.pop();
			for (int there = 0; there < NODES; ++there)
			{
				if (parent[there] == -1 && capacity[here][there] - flow[here][there] > 0)
				{
					parent[there] = here;
					q.push(there);
				}
			}
		}

		if (parent[SINK] == -1) break;

		int node = SINK, minResidual = INF;
		while (node != SOURCE)
		{
			int residual = capacity[parent[node]][node] - flow[parent[node]][node];
			minResidual = min(minResidual, residual);
			node = parent[node];
		}
		maxFlow += minResidual;

		node = SINK;
		while (node != SOURCE)
		{
			flow[parent[node]][node] += minResidual;
			flow[node][parent[node]] -= minResidual;
			node = parent[node];
		}
	}
	return maxFlow;
}

void InitGraph()
{
	maxFlow = 0;
	memset(flow, 0, sizeof(flow));
	memset(capacity, 0, sizeof(capacity));

	for (int i = 0; i < M; ++i)
	{
		int matchNode = GetMatchNode(i);
		capacity[SOURCE][matchNode] = 1;

		for (int j = 0; j < 2; ++j)
		{
			int playerNode = GetPlayerNode(match[i][j]);
			capacity[matchNode][playerNode] = 1;
		}
	}
}

bool CanWinWith(int totalWins)
{
	if (*max_element(wins + 1, wins + N) >= totalWins)
		return false;

	for (int i = 0; i < N; ++i)
	{
		int maxWin = i == 0 ? totalWins : totalWins - 1;
		int playerIdx = GetPlayerNode(i);
		capacity[playerIdx][SINK] = maxWin - wins[i];
	}

	return GetMaxFlow() == M && flow[GetPlayerNode(0)][SINK] == capacity[GetPlayerNode(0)][SINK];
}

int GetAnswer()
{
	InitGraph();
	for (int i = wins[0]; i <= wins[0] + M; ++i)
		if (CanWinWith(i)) return i;
	return -1;
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
		for (int i = 0; i < N; ++i)
			cin >> wins[i];
		for (int i = 0; i < M; ++i)
		{
			int a, b;
			cin >> a >> b;
			match[i][0] = a;
			match[i][1] = b;
		}

		cout << GetAnswer() << "\n";
	}
	return 0;
}
