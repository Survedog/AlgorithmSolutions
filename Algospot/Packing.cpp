// Problem: https://algospot.com/judge/problem/read/PACKING

#include <iostream>
#include <memory.h>
#include <algorithm>
#include <vector>

using namespace std;

typedef struct Item {
	char name[21];
	int volume;
	int necessity;
} Item;

int n, w;
Item items[100];
int memo[101][1001];
int bestChoice[101][1001];

int GetMaxNecessity(int start, int wLeft)
{
	if (wLeft == 0 || start == n - 1)
		return 0;

	int& ret = memo[start + 1][wLeft];
	if (ret != -1)
		return ret;

	ret = 0;
	for (int next = start + 1; next < n; ++next)
	{
		int newW = wLeft - items[next].volume;
		if (newW >= 0)
		{
			int necessitySum = GetMaxNecessity(next, newW) + items[next].necessity;
			if (ret < necessitySum)
			{
				ret = necessitySum;
				bestChoice[start + 1][wLeft] = next;
			}
		}
	}
	return ret;
}

void ReconstructResult(int start, int wLeft, vector<int>& chosenItems)
{
	int next = bestChoice[start + 1][wLeft];
	if (next != -1)
	{
		chosenItems.push_back(next);
		ReconstructResult(next, wLeft - items[next].volume, chosenItems);
		return;
	}
}

int main()
{
	int C;
	cin >> C;
	while (C--)
	{
		cin >> n >> w;
		memset(memo, -1, sizeof(memo));
		memset(bestChoice, -1, sizeof(bestChoice));

		for (int i = 0; i < n; i++)
		{
			cin >> items[i].name;
			cin >> items[i].volume;
			cin >> items[i].necessity;
		}

		int maxNecessity = GetMaxNecessity(-1, w);
		vector<int> resultItems;
		ReconstructResult(-1, w, resultItems);

		cout << maxNecessity << " " << resultItems.size() << "\n";
		for (int itemIdx : resultItems)
			cout << items[itemIdx].name << "\n";
	}
	return 0;
}
