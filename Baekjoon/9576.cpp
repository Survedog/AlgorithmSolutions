/*
* Problem: https://www.acmicpc.net/problem/9576
* 분류: 이분 매칭
*/
#include <iostream>
#include <vector>
#include <memory.h>

using namespace std;

int N, M;
vector<pair<int, int>> desire;
vector<int> manMatch, bookMatch;
vector<bool> visited;

bool DFS(int man)
{
	if (visited[man]) return false;
	visited[man] = true;

	int start = desire[man].first;
	int end = desire[man].second;
	for (int book = start; book <= end; ++book)
	{
		if (bookMatch[book] == -1 || DFS(bookMatch[book]))
		{
			bookMatch[book] = man;
			manMatch[man] = book;
			return true;
		}
	}
	return false;
}

int BiparpiteMatch()
{
	manMatch = vector<int>(M, -1);
	bookMatch = vector<int>(N, -1);

	int ret = 0;
	for (int man = 0; man < M; ++man)
	{
		visited = vector<bool>(M, false);
		if (DFS(man)) ret++;
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
		cin >> N >> M;
		desire = vector<pair<int, int>>(M);

		for (int i = 0; i < M; ++i)
		{
			int a, b;
			cin >> a >> b;
			desire[i] = make_pair(a - 1, b - 1);
		}

		cout << BiparpiteMatch() << "\n";
	}
	return 0;
}

