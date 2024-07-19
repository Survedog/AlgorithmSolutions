/*
* Problem: https://algospot.com/judge/problem/read/BLOCKGAME
* 모든 블록 배치 방법을 미리 계산하여 32비트 정수 값으로 저장한 뒤, 재귀함수에선 이 값을 게임 판의 상태 값과 비트 AND 연산하여 한 번에 블록이 배치 가능한 지 판단 가능하다.
* 또한 배치 방법 값을 게임 판의 상태 값에 더하면 현재 게임 판에 해당 방법대로 블록을 배치한 상태 값을 바로 구할 수 있다.
*/

#include <iostream>
#include <memory.h>
#include <cassert>
#include <string>
#include <vector>

using namespace std;

int board[5][5], blockPlacements[2600], placementCount;
char memo[1 << 25];
const int BendBlockX[4][3] = { {0, 1, 0}, {0, 0, 1}, {0, -1, 0}, {0, 0, -1} };
const int BendBlockY[4][3] = { {0, 0, -1}, {0, 1, 0}, {0, 0, 1}, {0, -1, 0} };
const int StraightBlockX[4][2] = { {0, 1}, {0, 0}, {0, -1}, {0, 0} };
const int StraightBlockY[4][2] = { {0, 0}, {0, 1}, {0, 0}, {0, -1} };

inline int CoordToIdx(int x, int y)
{
	return x + 5 * y;
}

int BoardToState(const int inBoard[5][5])
{
	int state = 0;
	for (int y = 0; y < 5; y++)
		for (int x = 0; x < 5; x++)
			if (inBoard[y][x] == 1)
				state += (1 << CoordToIdx(x, y));

	return state;
}

void CalcBlockPlacements()
{
	placementCount = 0;

	for (int y = 0; y < 5; y++)
		for (int x = 0; x < 5; x++)
			for (int rotate = 0; rotate < 4; rotate++)
			{
				int placement = 0;
				bool placeable = true;
				for (int block = 0; block < 3; block++)
				{
					int newX = x + BendBlockX[rotate][block];
					int newY = y + BendBlockY[rotate][block];
					if (newX < 0 || newX >= 5 || newY < 0 || newY >= 5)
					{
						placeable = false;
						continue;
					}
					placement += (1 << CoordToIdx(newX, newY));
				}

				if (placeable)
					blockPlacements[placementCount++] = placement;

				placement = 0;
				placeable = true;
				for (int block = 0; block < 2; block++)
				{
					int newX = x + StraightBlockX[rotate][block];
					int newY = y + StraightBlockY[rotate][block];
					if (newX < 0 || newX >= 5 || newY < 0 || newY >= 5)
					{
						placeable = false;
						continue;
					}
					placement += (1 << CoordToIdx(newX, newY));
				}

				if (placeable)
					blockPlacements[placementCount++] = placement;
			}
}

int CanWin(int state)
{
	if (state == (1 << 25) - 1)
		return 0;

	char& ret = memo[state];
	if (ret != -1) return ret;

	ret = 0;
	for (int i = 0; i < placementCount; ++i)
	{
		if ((state & blockPlacements[i]) == 0)
		{
			if (!CanWin(state + blockPlacements[i]))
			{
				ret = 1;
				break;
			}
		}
	}

	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	CalcBlockPlacements();

	int C;
	cin >> C;
	while (C--)
	{
		memset(memo, -1, sizeof(memo));
		for (int i = 0; i < 5; ++i)
		{
			string temp;
			cin >> temp;
			for (int j = 0; j < 5; ++j)
			{
				if (temp[j] == '.')
					board[i][j] = 0;
				else
					board[i][j] = 1;
			}
		}

		cout << (CanWin(BoardToState(board)) ? "WINNING" : "LOSING") << "\n";
	}

	return 0;
}