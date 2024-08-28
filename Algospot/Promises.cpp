/*
* Problem: https://algospot.com/judge/problem/read/PROMISES
* 분류: 플로이드 알고리즘
* 새 도로가 등장할 때마다 플로이드 알고리즘을 돌려주는 것은 시간 초과가 발생할 게 뻔하니, 실제로 변화가 일어나는 부분만 바꾸면 된다.
* 아이디어1) 도시 a-b에 새로운 다리가 놓아졌고, 그것이 현재 a, b 사이의 최단거리보다 짧다면 a-b 루트를 사용하는 경로들에 변경된 만큼의 거리를 더해주면 됨. 그럼 a-b를 지나가는 경로를 어떻게 찾아야 하지?
* 아이디어2) 이미 설치되어 있는 도로들에 대해 플로이드를 먼저 적용한 후, 새로운 도로 a-b가 나오면 a와 b를 S에서 제외시켰다가 다시 추가하는 것은 어떨까?
			-> 기존 도로가 없어지는건 아니므로 굳이 제외할 필요 없이 그냥 바로 갱신해도 상관 없지 않나? 또한 a, b에 대해 각각 갱신하지 말고, 기존 경로와 a-b를 건너는 경로를 비교하는 게 더 빠를듯.
			a-b 길이가 어떻게 바뀐다고 해도 i-a, b-j의 최단 경로에는 영향을 미치지 않으므로 i-a, b-j의 최단 경로는 이미 구해져있음.
			따라서 새로운 i-j의 최단경로는 min(dist[i][j], dist[i][a] + dist[a][b] + dist[b][j])이다.
			-> UpdateNode 함수를 작성해, 위의 방식으로 a-b를 지나갈 경우에 대해서만 최단 경로를 갱신하자.
* 
* 다음의 실수로 오답이 발생함.
* 1) 모든 간선은 양방향으로 이동가능한데도 새로운 고속도로 a-b에 대해 a->b의 경로로 갈 때만 업데이트하고 b->a의 경로로 이동할 때는 고려하지 않음.
* 2) Floyd 함수를 호출하지 않고 UpdateNode 함수만 호출함.
*/
#include <iostream>
#include <vector>
#include <memory.h>

using namespace std;

const int INF = 987654321;
int V, M, N, adjacent[200][200];

void Floyd()
{
	for (int k = 0; k < V; ++k)
		for (int i = 0; i < V; ++i)
			for (int j = 0; j < V; ++j)
				adjacent[i][j] = min(adjacent[i][j], adjacent[i][k] + adjacent[k][j]);
}

void UpdateNode(int a, int b, int newDist)
{
	for (int i = 0; i < V; ++i)
		for (int j = 0; j < V; ++j)
		{
			adjacent[i][j] = min(adjacent[i][j], adjacent[i][a] + newDist + adjacent[b][j]);
			adjacent[i][j] = min(adjacent[i][j], adjacent[i][b] + newDist + adjacent[a][j]);
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
		cin >> V >> M >> N;

		for (int i = 0; i < V; ++i)
			for (int j = 0; j < V; ++j)
			{
				if (i == j) adjacent[i][j] = 0;
				else adjacent[i][j] = INF;
			}

		for (int i = 0; i < M; ++i)
		{
			int a, b, c;
			cin >> a >> b >> c;
			adjacent[a][b] = adjacent[b][a] = min(adjacent[b][a], c);
		}
		Floyd();

		int answer = 0;
		for (int i = 0; i < N; ++i)
		{
			int a, b, c;
			cin >> a >> b >> c;
			if (adjacent[a][b] <= c)
				answer++;
			else
				UpdateNode(a, b, c);
		}
		cout << answer << "\n";
	}
	return 0;
}
