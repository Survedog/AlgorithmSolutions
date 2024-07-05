// Problem: https://algospot.com/judge/problem/read/JUMPGAME

#include <iostream>
#include <string>

#define MAX_GRID_SIZE 100

int n, number[MAX_GRID_SIZE][MAX_GRID_SIZE];
bool memo[MAX_GRID_SIZE][MAX_GRID_SIZE];

void Initialize()
{
	std::cin >> n;

	for (int row = 0; row < n; ++row)
	{
		for (int col = 0; col < n; ++col)
		{
			std::cin >> number[row][col];
		}
	}

	memset(memo, true, sizeof(memo));
}

bool IsArrivableFrom(int row, int col)
{
	if (row < 0 || row >= n || col < 0 || col >= n)
	{
		return false;
	}
	if (row == n - 1 && col == n - 1)
	{
		return true;
	}
	if (memo[row][col] == false)
	{
		return false;
	}

	bool& ret = memo[row][col];
	const int moveCount = number[row][col];
	return ret = IsArrivableFrom(row + moveCount, col) || IsArrivableFrom(row, col + moveCount);
}

int main()
{
	int C = 0;
	std::cin >> C;

	for (int t = 0; t < C; t++)
	{
		Initialize();
		std::cout << (IsArrivableFrom(0, 0) ? "YES" : "NO") << std::endl;
	}

	return 0;
}
