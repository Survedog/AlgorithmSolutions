/*
* Problem: https://www.acmicpc.net/problem/6086
* 분류: 포드-풀커슨 알고리즘
* 
* 전형적인 포드 풀커슨 알고리즘을 사용하면 되는 문제지만, 그래프는 단순 그래프가 아닐 수도 있다는 차별점이 있다. (즉, 두 노드간 중복되는 간선이 존재할 수 있다)
* 노드간 연결 정보를 저장하는 데 2차원 배열 대신 인접 리스트를 사용하여, 중복되는 간선들도 모두 저장하도록 했다. (이전 커밋)
* 
* 실수한 점)
* 1. 증가 경로를 찾은 뒤 경로에 최소 Residual(용량 - 유량)을 더해야 하는데, 최소 용량을 더해 오류가 생겼다.
* 2. 전역 변수에 인접 리스트를 표현하는 vector<Edge> 배열을 선언한 뒤 main에서 emplace_back으로 간선 정보를 넣어줬는데,
*    이렇게 벡터에 넣은 원소도 현재 스택 스코프에서만 유효한 것인지 스코프를 벗어나자 reverse 필드 값이 초기화됐다.
*    인접 리스트를 vector<Edge*>의 배열로 바꾼 후 간선 정보를 new Edge()로 힙에 넣어주니 정상적으로 값이 저장됐다.
*/
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

struct Edge
{
	int start, dest, capacity, flow;
	Edge* reverse;

	Edge(int start, int dest, int capacity) : start(start), dest(dest), capacity(capacity), flow(0), reverse(nullptr) {}

	inline int GetResidual() { return capacity - flow; }
	void Push(int amount)
	{
		flow += amount;
		reverse->flow -= amount;
	}
};

const int MAX_IDX = 'z', INF = 987654321;
const int SOURCE = 'A', SINK = 'Z';
int N;
vector<Edge*> adjacent[MAX_IDX + 1];

int GetMaxFlow(int source)
{
	int ret = 0;
	while (true)
	{
		vector<Edge*> fromParent(MAX_IDX + 1, nullptr);
		queue<int> q;
		q.push(source);

		while (!q.empty() && fromParent[SINK] == nullptr)
		{
			int here = q.front(); q.pop();

			for (Edge* edge : adjacent[here])
			{
				int there = edge->dest;
				int residual = edge->GetResidual();

				if (there != SOURCE && fromParent[there] == nullptr && residual > 0)
				{
					q.push(there);
					fromParent[there] = edge;
				}
			}
		}

		if (fromParent[SINK] == nullptr) break;

		int here = SINK, minResidual = INF;
		while (here != SOURCE)
		{
			minResidual = min(minResidual, fromParent[here]->GetResidual());
			here = fromParent[here]->start;
		}

		here = SINK;
		while (here != SOURCE)
		{
			fromParent[here]->Push(minResidual);
			here = fromParent[here]->start;
		}
		ret += minResidual;
	}
	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N;
	for (int i = 0; i <= MAX_IDX; ++i)
		adjacent[i].clear();

	for (int i = 0; i < N; ++i)
	{
		int c;
		char u, v;
		cin >> u >> v >> c;

		if (u == v) continue;

		Edge* uv = new Edge(u, v, c);
		Edge* vu = new Edge(v, u, c);
		uv->reverse = vu;
		vu->reverse = uv;

		adjacent[u].push_back(uv);
		adjacent[v].push_back(vu);
	}

	cout << GetMaxFlow(SOURCE) << "\n";
	for (int i = 0; i <= MAX_IDX; ++i)
		for (Edge* edge : adjacent[i])
			delete edge;

	return 0;
}