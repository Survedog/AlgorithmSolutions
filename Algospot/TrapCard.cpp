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
bool adjacent[MAX_N][MAX_M];
vector<string> board;
vector<bool> visited;
vector<int> aMatch, bMatch;
vector<vector<int>> blockID;
vector<pair<int, int>> aID2Pos, bID2Pos;

inline int GetTeam(int y, int x) { return (x + y) % 2; }

int AssignID(int y, int x)
{
	if (GetTeam(y, x) == 0)
	{
		blockID[y][x] = n++;
		aID2Pos[blockID[y][x]] = make_pair(y, x);
	}
	else
	{
		blockID[y][x] = m++;
		bID2Pos[blockID[y][x]] = make_pair(y, x);
	}
	return blockID[y][x];
}

inline bool IsInRange(int y, int x)
{
	return y >= 0 && y < H && x >= 0 && x < W;
}

bool DFS(int a)
{
	if (visited[a]) return false;
	visited[a] = true;

	for (int b = 0; b < m; ++b)
	{
		if (!adjacent[a][b]) continue;

		if (bMatch[b] == -1 || DFS(bMatch[b]))
		{
			aMatch[a] = b;
			bMatch[b] = a;
			return true;
		}
	}
	return false;
}

int BipartiteMatch()
{
	aMatch = vector<int>(n, -1);
	bMatch = vector<int>(m, -1);

	int match = 0;
	for (int i = 0; i < n; ++i)
	{
		visited = vector<bool>(n, false);
		if (DFS(i)) match++;
	}
	return match;
}

void MakeGraph()
{
	n = 0; m = 0;
	blockID = vector<vector<int>>(H, vector<int>(W, -1));
	aID2Pos = bID2Pos = vector<pair<int, int>>(H * W / 2);
	memset(adjacent, false, sizeof(adjacent));

	for (int y = 0; y < H; ++y)
		for (int x = 0; x < W; ++x)
		{
			if (board[y][x] != '.') continue;

			if (blockID[y][x] == -1) blockID[y][x] = AssignID(y, x);
			int id = blockID[y][x];

			for (int dir = 0; dir < 4; ++dir)
			{
				int cy = y + dy[dir];
				int cx = x + dx[dir];

				if (blockID[cy][cx] == -1) blockID[cy][cx] = AssignID(cy, cx);
				int cID = blockID[cy][cx];

				if (IsInRange(cy, cx) && board[cy][cx] == '.')
					adjacent[id][cID] = adjacent[cID][id] = true;
			}
		}
}

int GetAnswer()
{
	MakeGraph();
	BipartiteMatch();

	vector<bool> aSelected(n, true), bSelected(m, false);

	int ret = n;
	for (int i = 0; i < m; ++i)
		if (bMatch[i] == -1)
		{
			bSelected[i] = true;
			ret++;
		}

	for (int a = 0; a < n; ++a)
		for (int b = 0; b < m; ++b)
		{
			if (aMatch[a] != b && adjacent[a][b])
			{
				aSelected[a] = false;
				bSelected[b] = true;
			}

			while ()
			{

			}
		}

	for (int a = 0; a < n; ++a)
		if (aSelected[a])
		{

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
		cin >> H >> W;
		board = vector<string>(H);
		for (int i = 0; i < H; ++i)
			cin >> board[i];

		GetAnswer();
	}
	return 0;
}

