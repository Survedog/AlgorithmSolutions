/*
* Problem: https://www.acmicpc.net/problem/3665
* 분류: 위상정렬
* 팀 간의 상대 순서를 인접 행렬로 표현한 뒤 DFS를 사용한 위상 정렬로 절대 순서를 구하였다.
* 이때 그래프에 사이클이 발생한다면 일관성이 없는 정보로 판단하였다.
* 사이클 존재 유무는 DFS의 각 단계에서 현재 노드와 연결된 노드 중 아직 DFS가 종료되지 않은 노드(조상 노드)가 있는지 확인함으로써 판단했다.
* 
* 실수) DFS를 시작할 최초 노드는 입력 간선 차수가 0인 노드로 설정했는데, 사이클이 발생하는 경우 그런 노드가 없을 수도 있어 오답이 발생했다.
* 때문에 그런 노드가 없을 때는 IMPOSSIBLE을 출력하도록 변경했다. 
*/

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int N, lastRank[501], M;
vector<vector<bool>> adjacent;
vector<bool> visited, isSearching;

bool TopologySort(int here, vector<int>& result)
{
	visited[here] = true;
	isSearching[here] = true;
	for (int there = 1; there <= N; ++there)
		if (adjacent[here][there])
		{
			if (isSearching[there] || (!visited[there] && !TopologySort(there, result)))
				return false;
		}

	result.push_back(here);
	isSearching[here] = false;
	return true;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> N;
		adjacent = vector<vector<bool>>(N + 1, vector<bool>(N + 1, false));
		visited = isSearching = vector<bool>(N + 1, false);
		for (int i = 1; i <= N; ++i)
		{
			int team;
			cin >> team;
			lastRank[team] = i;
		}

		cin >> M;
		vector<int> inDegree(N + 1, 0);
		for (int i = 0; i < M; ++i)
		{
			int a, b;
			cin >> a >> b;
			if (lastRank[a] > lastRank[b])
			{
				adjacent[a][b] = true;
				inDegree[b]++;
			}
			else
			{
				adjacent[b][a] = true;
				inDegree[a]++;
			}
		}

		for (int i = 1; i <= N; ++i)
			for (int j = 1; j <= N; ++j)
				if (!adjacent[j][i] && lastRank[i] < lastRank[j])
				{
					adjacent[i][j] = true;
					inDegree[j]++;
				}

		int start = -1;
		for (int i = 1; i <= N; ++i)
			if (inDegree[i] == 0)
				start = i;

		vector<int> result;
		if (start != -1 && TopologySort(start, result))
		{
			reverse(result.begin(), result.end());
			for (int i : result)
				cout << i << " ";
			cout << "\n";
		}
		else
		{
			cout << "IMPOSSIBLE\n";
		}
	}

	return 0;
}