/*
* Problem: https://www.acmicpc.net/problem/1028
* 분류: 동적계획법
* 
* 동적계획법을 통해 각 위치에서 4개의 대각선 방향으로 연결되는 1의 길이를 구한 후, 매 위치마다 자신을 꼭대기로 하는 다이아몬드의 최대 크기를 계산해 답을 구했다.
*/
#include <iostream>
#include <algorithm>

using namespace std;

int R, C, board[750][750];
pair<int, int> up[750][750], down[750][750];

bool operator==(const pair<int, int>& a, const pair<int, int>& b)
{
	return a.first == b.first && a.second && b.second;
}

pair<int, int> CalcDownLen(int row, int col)
{
	pair<int, int>& ret = down[row][col];
	if (ret.first != -1) return ret;

	ret = make_pair(board[row][col], board[row][col]);
	if (row == R - 1 || board[row][col] == 0) return ret;

	if (col != 0)
		ret.first += CalcDownLen(row + 1, col - 1).first;
	if (col != C - 1)
		ret.second += CalcDownLen(row + 1, col + 1).second;
	return ret;
}

pair<int, int> CalcUpLen(int row, int col)
{
	pair<int, int>& ret = up[row][col];
	if (ret.first != -1) return ret;

	ret = make_pair(board[row][col], board[row][col]);
	if (row == 0 || board[row][col] == 0) return ret;

	if (col != 0)
		ret.first += CalcUpLen(row - 1, col - 1).first;
	if (col != C - 1)
		ret.second += CalcUpLen(row - 1, col + 1).second;
	return ret;
}

bool MakesDiamond(int r1, int r2, int col, int size)
{
	if (r1 > r2) swap(r1, r2);
	return down[r1][col].first >= size && down[r1][col].second >= size && up[r2][col].first >= size && up[r2][col].second >= size;
}

int GetMaxDiamondSize(int row, int col)
{
	if (board[row][col] == 0) return 0;
	int maxSize = min((R - 1) / 2 + 1, (C - 1) / 2 + 1);
	maxSize = min(maxSize, col + 1);
	maxSize = min(maxSize, C - col);
	maxSize = min(maxSize, (R - row) / 2 + 1);

	int ret = 1;
	for (int size = 2; size <= maxSize; ++size)
	{
		int bottom = row + (size - 1) * 2;
		if (MakesDiamond(row, bottom, col, size))
			ret = size;
	}
	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> R >> C;
	for (int i = 0; i < R; ++i)
		for (int j = 0; j < C; ++j)
		{
			char c;
			cin >> c;
			board[i][j] = c - '0';

			up[i][j] = down[i][j] = make_pair(-1, -1);
		}

	for (int i = 0; i < R; ++i)
		for (int j = 0; j < C; ++j)
		{
			CalcDownLen(i, j);
			CalcUpLen(i, j);
		}

	int answer = 0;
	for (int i = 0; i < R; ++i)
		for (int j = 0; j < C; ++j)
			answer = max(answer, GetMaxDiamondSize(i, j));

	cout << answer << "\n";
	return 0;
}