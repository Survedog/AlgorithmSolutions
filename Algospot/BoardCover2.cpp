/*
* Problem: https://algospot.com/judge/problem/read/BOARDCOVER2
* 조합탐색으로 푸는데, 다음 최적화 방식을 적용하였다.
* - 남은 빈 칸의 크기를 블록의 크기로 나눈 값을 구하는 휴리스틱 함수를 구현해, 현재 상태에서 더 놓을 수 있는 블록의 개수의 상한으로 사용하여 가지치기함.
* - 보드의 남은 칸이 DP_THRESHOLD개 만큼 남았다면 해당 위치에서부터 블록을 놓을 수 있는 최대 개수는 동적 계획법을 사용해 구함.
* 그러나 여전히 시간 초과가 발생하였다.
* 
* 블록을 배치/제거하는 과정을 Set이라는 별도의 함수로 분리시키고, 교재의 search 함수를 그대로 가져와 사용해봤으나 수행 시간이 전혀 개선되지 않음.
* set 함수를 최적화할 필요가 있어보임.
*/

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

int H, W, R, C, blockSize, maxBlocks, spaceLeft[10][10], board[10][10];
vector<vector<pair<int, int>>> rotations;

vector<string> RotateBlock(vector<string>& inBlock)
{
	vector<string> ret(inBlock[0].size(), string(inBlock.size(), ' '));
	for (int i = 0; i < inBlock.size(); ++i)
		for (int j = 0; j < inBlock[0].size(); ++j)
			ret[j][inBlock.size() - i - 1] = inBlock[i][j];
	return ret;
}

void PreProcess()
{
	vector<string> block(R);
	for (int i = 0; i < R; ++i)
		cin >> block[i];

	rotations.clear();
	rotations.resize(4);
	for (int rotate = 0; rotate < 4; ++rotate)
	{
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
					rotations[rotate].push_back(make_pair(y - pivotY, x - pivotX));
				}
		block = RotateBlock(block);
	}
	sort(rotations.begin(), rotations.end());
	rotations.erase(unique(rotations.begin(), rotations.end()), rotations.end());
	blockSize = rotations[0].size();

	int spaceCount = 0;
	for (int i = 0; i < H; ++i)
		for (int j = 0; j < W; ++j)
		{
			spaceLeft[i][j] = -spaceCount;
			if (board[i][j] == 0)
				spaceCount++;
		}

	for (int i = 0; i < H; ++i)
		for (int j = 0; j < W; ++j)
			spaceLeft[i][j] += spaceCount;
}

bool Set(int x, int y, vector<pair<int, int>>& blockRotations, int value)
{
	bool placeable = true;
	for (pair<int, int> blockOffset : blockRotations)
	{
		int blockY = y + blockOffset.first;
		int blockX = x + blockOffset.second;

		if (blockX >= W || blockY >= H || blockX < 0 || blockY < 0)
		{
			placeable = false;
			continue;
		}

		if (board[blockY][blockX] != 0)
			placeable = false;
		board[blockY][blockX] += value;
	}
	return placeable;
}

void GetMaxBlocks(int x, int y, int placed)
{
	if (x >= W)
	{
		x = 0;
		y++;
	}
	if (y >= H)
	{
		maxBlocks = max(maxBlocks, placed);
		return;
	}
	if (maxBlocks >= placed + spaceLeft[y][x] / blockSize)
		return;

	if (board[y][x] == 0)
	{
		for (vector<pair<int, int>> rotation : rotations)
		{
			if (Set(x, y, rotation, 1))
				GetMaxBlocks(x + 1, y, placed + 1);
			Set(x, y, rotation, -1);
		}
	}
	GetMaxBlocks(x + 1, y, placed);
}

void search(int placed)
{
	int y = -1, x = -1;
	for (int i = 0; i < H; ++i)
	{
		for (int j = 0; j < W; ++j)
			if (board[i][j] == 0)
			{
				y = i;
				x = j;
				break;
			}
		if (y != -1) break;
	}

	if (y == -1)
	{
		maxBlocks = max(maxBlocks, placed);
		return;
	}

	for (vector<pair<int, int>> rotation : rotations)
	{
		if (Set(x, y, rotation, 1))
			search(placed + 1);
		Set(x, y, rotation, -1);
	}

	board[y][x] = 1;
	search(placed);
	board[y][x] = 0;
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
		memset(board, 0, sizeof(board));

		cin >> H >> W >> R >> C;
		for (int i = 0; i < H; ++i)
		{
			char str[11];
			cin >> str;
			for (int j = 0; j < W; ++j)
				board[i][j] = (str[j] == '#' ? 1 : 0);
		}

		PreProcess();
		GetMaxBlocks(0, 0, 0);
		//search(0);
		cout << maxBlocks << "\n";
	}

	return 0;
}