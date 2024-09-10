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
* 
* 중복되는 간선을 처리하는 더 간단한 방법이 있었다. 중복되는 이전 간선 용량에 더해주기만 하면 된다. (현재 커밋)
* 이를 이용해 인접 행렬로 다시 풀어 보았다.
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

const int MAX_IDX = 'z', INF = 987654321;
const int SOURCE = 'A', SINK = 'Z';
int N;
vector<vector<int>> capacity, flow;

int GetMaxFlow(int source)
{
	int ret = 0;
	while (true)
	{
		vector<int> parent(MAX_IDX + 1, -1);
		queue<int> q;
		parent[SOURCE] = SOURCE;
		q.push(source);

		while (!q.empty() && parent[SINK] == -1)
		{
			int here = q.front(); q.pop();

			for (int there = 'A'; there <= 'Z'; ++there)
			{
				int residual = capacity[here][there] - flow[here][there];
				if (parent[there] == -1 && residual > 0)
				{
					q.push(there);
					parent[there] = here;
				}
			}

			for (int there = 'a'; there <= 'z'; ++there)
			{
				int residual = capacity[here][there] - flow[here][there];
				if (parent[there] == -1 && residual > 0)
				{
					q.push(there);
					parent[there] = here;
				}
			}
		}

		if (parent[SINK] == -1) break;

		int here = SINK, minResidual = INF;
		while (here != SOURCE)
		{
			int residual = capacity[parent[here]][here] - flow[parent[here]][here];
			minResidual = min(minResidual, residual);
			here = parent[here];
		}

		here = SINK;
		while (here != SOURCE)
		{
			flow[parent[here]][here] += minResidual;
			flow[here][parent[here]] -= minResidual;
			here = parent[here];
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
	capacity = flow = vector<vector<int>>(MAX_IDX + 1, vector<int>(MAX_IDX + 1, 0));
	for (int i = 0; i < N; ++i)
	{
		int c;
		char start, end;
		cin >> start >> end >> c;

		if (start == end) continue;

		capacity[start][end] += c;
		capacity[end][start] += c;
	}

	cout << GetMaxFlow(SOURCE) << "\n";
	return 0;
}