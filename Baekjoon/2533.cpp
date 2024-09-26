/*
* Problem: https://acmicpc.net/problem/2533
* 분류: 트리
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
	for (int child : adjacent[node])
	{
		if (child == parent) continue;
		if (!SetEarlyAdaptor(child, node))
		{
			cnt++;
			return true;
		}
	}
	return false;
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
	SetEarlyAdaptor(0, -1);
	cout << cnt << "\n";
}