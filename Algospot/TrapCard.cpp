/*
* Problem: https://algospot.com/judge/problem/read/TRAPCARD
* 분류: 이분 매칭
*/
#include <iostream>
#include <vector>
#include <memory.h>
#include <string>

using namespace std;

const int MAX_N = 200, MAX_M = 200;
const int dx[] = { 1, -1, 0, 0 }, dy[] = { 0, 0, 1, -1 };

int H, W, n, m;
vector<string> board;
vector<vector<bool>> visited;
vector<vector<pair<int, int>>> aMatch, bMatch;

inline bool IsInRange(int y, int x)
{
	return y >= 0 && y < H && x >= 0 && x < W;
}

bool DFS(int y, int x)
{
	if (visited[y][x]) return false;
	visited[y][x] = true;

	for (int i = 0; i < 4; ++i)
	{
		int ny = y + dy[i];
		int nx = x + dx[i];

		if (!IsInRange(ny, nx)) continue;

		int cy = bMatch[ny][nx].first;
		int cx = bMatch[ny][nx].second;
		if (aMatch[ny][nx] == make_pair(-1, -1) || DFS(cy, cx))
		{
			aMatch[ny][nx] = make_pair(y, x);
			bMatch[]
		}
	}
}

int BipartiteMatch()
{
	aMatch = bMatch = vector<vector<pair<int, int>>>(H, vector<pair<int, int>>(W, make_pair(-1, -1)));

	int match = 0;
	for (int i = 0; i < H; ++i)
		for (int j = 0; j < W; ++j)
		{
			visited = vector<vector<bool>>(H, vector<bool>(W, false));
			if (DFS(i, j)) match++;
		}
	return match;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> H >> W;
		board = vector<string>(H);
		for (int i = 0; i < H; ++i)
			cin >> board[i];
	}
	return 0;
}
