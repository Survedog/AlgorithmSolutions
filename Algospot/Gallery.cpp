/*
* Problem: https://algospot.com/judge/problem/read/GALLERY
* 책해설) 먼저 갤러리를 노드로, 통로를 간선으로 표현하면 이 미술관의 구조를 무방향 그래프로 나타낼 수 있다.
* 미술관은 한 번 관람한 갤러리를 다시 가기 위해서는 이전에 지나왔던 복도를 반드시 한 번 지나야 하는 구조로 설계되어 있다는 설명에서, 이 그래프엔 사이클이 존재하지 않는다는 것을 알 수 있다.
* 따라서 이 미술관은 루트 없는 트리라고 할 수 있다. 이 트리를 리프에서부터 올라가면 현재 노드(갤러리)에 CCTV를 설치해야 할지 말아야 할 지를 쉽게 알 수 있다.
* 현재 노드의 자식 노드들 중 감시되지 않고 있는 노드가 있다면, 현재 노드에 CCTV를 설치하지 않으면 절대 감시할 수 없다.
* 반면 자식들이 모두 감시되고 있다면 자신의 부모에 CCTV를 설치하는 게 더 많은 갤러리를 감시할 수 있으므로 무조건 이득이다.
* 위 원리를 바탕으로 그래프를 DFS로 순회해(이때 그래프는 첫 노드를 루트로 하는 트리로 취급할 수 있다), 리프에서 올라오면서 CCTV 설치 여부를 결정하면 미술관 전체를 감시하기 위한 최소 CCTV 개수를 알 수 있다.
* 이때 그래프는 연결 그래프가 아닐 수 있으므로 모든 방문하지 않은 노드에 대해 DFS를 수행해야 한다.
*/
#include <iostream>
#include <vector>
#include <memory.h>

using namespace std;

int G, H;
bool adjacent[1000][1000], visited[1000], dominated[1000];

int DFS(int node, bool isRoot)
{
	visited[node] = true;

	int ret = 0;
	bool shouldSelect = false;

	for (int adj = 0; adj < G; ++adj)
		if (adjacent[node][adj] && !visited[adj])
		{
			ret += DFS(adj, false);
			if (!dominated[adj]) shouldSelect = true;
		}

	if (shouldSelect)
	{
		for (int adj = 0; adj < G; ++adj)
			if (adjacent[node][adj])
				dominated[adj] = true;

		dominated[node] = true;
		ret++;
	}

	if (isRoot && !dominated[node])
	{
		dominated[node] = true;
		ret++;
	}
	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> G >> H;
		memset(adjacent, false, sizeof(adjacent));
		memset(visited, false, sizeof(visited));
		memset(dominated, false, sizeof(dominated));
		for (int i = 0; i < H; ++i)
		{
			int a, b;
			cin >> a >> b;
			adjacent[a][b] = true;
			adjacent[b][a] = true;
		}

		int sum = 0;
		for (int i = 0; i < G; ++i)
			if (!visited[i])
				sum += DFS(i, true);

		cout << sum << "\n";
	}
	return 0;
}