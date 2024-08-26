/*
* Problem: https://algospot.com/judge/problem/read/HANOI4B
*/
#include <iostream>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

int N;

struct State
{
	stack<int> disks[4];

	State()
	{

	}
};

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> N;
		for (int i = 0; i < N; ++i)
		{
			int count;
			cin >> count;
			for (int j = 0; j < count; ++j)
			{
				int disk;
				cin >> disk;

			}
		}

		for (int i = 0; i < N; ++i)
		{
			int count;
			cin >> count;
			for (int j = 0; j < count; ++j)
			{
				int disk;
				cin >> disk;

			}
		}
	}
	return 0;
}