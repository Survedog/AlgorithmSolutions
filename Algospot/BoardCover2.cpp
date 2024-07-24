/*
* Problem: https://algospot.com/judge/problem/read/BOARDCOVER2
* 조합탐색으로 푸는데, 다음 최적화 방식을 적용하였다.
* - 남은 빈 칸의 크기를 블록의 크기로 나눈 값을 구하는 휴리스틱 함수를 구현해, 현재 상태에서 더 놓을 수 있는 블록의 개수의 상한으로 사용하여 가지치기함.
* - 보드의 남은 칸이 DP_THRESHOLD개 만큼 남았다면 해당 위치에서부터 블록을 놓을 수 있는 최대 개수는 동적 계획법을 사용해 구함.
* 그러나 여전히 시간 초과가 발생하였다.
*/

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

using namespace std;

const int DP_THRESHOLD = 10;
int H, W, R, C, blockSize, maxBlocks, spaceLeft[10][10];
map<int, int> memo[DP_THRESHOLD];
vector<pair<int, int>> blockRelativePos[4];
vector<int> blockStates;

vector<string> RotateBlock(vector<string>& inBlock)
{
	vector<string> ret(inBlock[0].size(), string(inBlock.size(), ' '));
	for (int i = 0; i < inBlock.size(); ++i)
		for (int j = 0; j < inBlock[0].size(); ++j)
			ret[j][inBlock.size() - i - 1] = inBlock[i][j];
	return ret;
}

void PreProcess(vector<vector<int>>& inBoard)
{
	blockSize = 0;
	vector<string> block(R);
	for (int i = 0; i < R; ++i)
	{
		cin >> block[i];
		for (int j = 0; j < C; ++j)
			if (block[i][j] == '#')
				blockSize++;
	}

	for (int rotate = 0; rotate < 4; ++rotate)
	{
		blockRelativePos[rotate].clear();

		int pivotY = -1, pivotX = -1;
		for (int y = 0; y < block.size(); ++y)
			for (int x = 0; x < block[0].size(); ++x)
				if (block[y][x] == '#')
				{
					if (pivotY == -1)
					{
						pivotY = y;
						pivotX = x;
					}
					blockRelativePos[rotate].push_back(make_pair(y - pivotY, x - pivotX));
				}
		block = RotateBlock(block);
	}

	blockStates.clear();
	for (int square = 0; square < DP_THRESHOLD; ++square)
	{
		for (int rotate = 0; rotate < 4; ++rotate)
		{
			int blockState = 0;
			for (pair<int, int> blockOffset : blockRelativePos[rotate])
			{
				int blockedSquare = square + blockOffset.first * H + blockOffset.second;
				blockState |= (1 << blockedSquare);
			}

			blockStates.push_back(blockState);
		}
	}

	int spaceCount = 0;
	for (int i = 0; i < inBoard.size(); ++i)
		for (int j = 0; j < inBoard[0].size(); ++j)
		{
			spaceLeft[i][j] = -spaceCount;
			if (inBoard[i][j] == 0)
				spaceCount++;
		}

	for (int i = 0; i < inBoard.size(); ++i)
		for (int j = 0; j < inBoard[0].size(); ++j)
			spaceLeft[i][j] += spaceCount;
}

int DP(int boardState, int square)
{
	if (square == DP_THRESHOLD || boardState == (1 << DP_THRESHOLD) - 1) return 0;

	auto findIter = memo[square].find(boardState);
	if (findIter != memo[square].end()) return findIter->second;

	int& ret = memo[square][boardState] = 0;
	for (int rotate = 0; rotate < 4; ++rotate)
	{
		int blockState = 0;
		for (pair<int, int> blockOffset : blockRelativePos[rotate])
		{
			int blockedSquare = square + blockOffset.first * H + blockOffset.second;
			blockState |= (1 << blockedSquare);
		}

		if ((boardState | blockState) == 0)
			ret = max(ret, DP(boardState | blockState, square + 1) + 1);
	}
	ret = max(ret, DP(boardState, square + 1));
	return ret;
}

void GetMaxBlocks(vector<vector<int>>& inBoard, int x, int y, int placed)
{
	if (x >= W)
	{
		x = 0;
		y++;
	}
	if (y >= H)
		return;
	if (maxBlocks > placed + spaceLeft[y][x] / blockSize)
		return;

	if ((H - y) * W - x <= DP_THRESHOLD)
	{
		int square = 0, nx = x, ny = y, boardState = 0;
		while (ny < H)
		{
			if (inBoard[ny][nx] == 1)
				boardState |= (1 << square);

			nx++;
			if (nx >= W)
			{
				nx = 0;
				ny++;
			}
			square++;
		}

		maxBlocks = max(maxBlocks, placed + DP(boardState, 0));
		return;
	}

	if (inBoard[y][x] == 0)
	{
		for (int rotate = 0; rotate < 4; ++rotate)
		{
			bool placeable = true;
			for (pair<int, int> blockOffset : blockRelativePos[rotate])
			{
				int blockY = y + blockOffset.first;
				int blockX = x + blockOffset.second;

				if (blockX >= W || blockY >= H || blockX < 0 || blockY < 0)
				{
					placeable = false;
					continue;
				}
				if (inBoard[blockY][blockX] != 0)
					placeable = false;

				inBoard[blockY][blockX] += 1;
			}

			if (placeable)
			{
				maxBlocks = max(maxBlocks, placed + 1);
				GetMaxBlocks(inBoard, x + 1, y, placed + 1);
			}

			for (pair<int, int> blockOffset : blockRelativePos[rotate])
			{
				int blockY = y + blockOffset.first;
				int blockX = x + blockOffset.second;

				if (blockX < W && blockY < H && blockX >= 0 && blockY >= 0)
					inBoard[blockY][blockX] -= 1;
			}
		}
	}
	GetMaxBlocks(inBoard, x + 1, y, placed);
	return;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int T;
	cin >> T;
	while (T--)
	{
		maxBlocks = 0;

		cin >> H >> W >> R >> C;
		vector<vector<int>> board(H, vector<int>(W, 0));
		for (int i = 0; i < H; ++i)
		{
			char str[11];
			cin >> str;
			for (int j = 0; j < W; ++j)
				board[i][j] = (str[j] == '#' ? 1 : 0);
		}

		PreProcess(board);
		GetMaxBlocks(board, 0, 0, 0);
		cout << maxBlocks << "\n";
	}
}