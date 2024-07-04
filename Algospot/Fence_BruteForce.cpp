// Problem: https://github.com/Survedog/AlgorithmSolutions/tree/main/Algospot

#include <iostream>

using namespace std;

#define MAX_H 10000
#define MAX_N 20000

int C, N;
int BoardHeights[MAX_N];

int main()
{
	cin >> C;
	for (int Case = 0; Case < C; ++Case)
	{
		cin >> N;
		int MaxSize = 0;

		for (int Board = 0; Board < N; ++Board)
		{
			cin >> BoardHeights[Board];
		}

		for (int Height = 1; Height <= MAX_H; Height++)
		{
			int UsableCount = 0;
			for (int Board = 0; Board < N; ++Board)
			{
				if (Height <= BoardHeights[Board])
				{
					UsableCount++;
					MaxSize = max(MaxSize, UsableCount * Height);
				}
				else
				{
					UsableCount = 0;
				}
			}
		}

		cout << MaxSize << "\n";
	}

	return 0;
}