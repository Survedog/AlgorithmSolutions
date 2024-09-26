/*
* Problem: https://acmicpc.net/problem/2533
* 분류: 트리
* 
* Algospot/Gallery와 거의 비슷한 방식으로 풀었다.
* 트리를 DFS해서 맨 아래로 내려간 뒤 올라오면서 현재 노드가 얼리 어답터인지 아닌지 정하면 된다.
* 만약 자식 중 어느 하나라도 얼리어답터가 아닌 노드가 존재한다면 현재 노드는 반드시 얼리 어답터여야 한다.
* 반면 자식들이 모두 얼리어답터라면 현재 노드는 얼리 어답터가 아니어도 된다.
* 위 로직을 바탕으로 트리를 한번 순회하면 최소 얼리 어답터 수를 알 수 있게 된다.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

int N, cnt;
vector<int> adjacent[1000001];

bool SetEarlyAdaptor(int node, int parent)
{
	bool isEarlyAdaptor = false;
	for (int child : adjacent[node])
	{
		if (child == parent) continue;
		if (!SetEarlyAdaptor(child, node) && !isEarlyAdaptor)
		{
			cnt++;
			isEarlyAdaptor = true;
		}
	}
	return isEarlyAdaptor;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N;
	for (int i = 0; i < N - 1; ++i)
	{
		int u, v;
		cin >> u >> v;
		adjacent[u].push_back(v);
		adjacent[v].push_back(u);
	}

	cnt = 0;
	SetEarlyAdaptor(1, -1);
	cout << cnt << "\n";
}