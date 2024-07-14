#include <stdio.h>
#include <iostream>
#include <memory.h>
#include <cassert>

using namespace std;

int n, d, p;
int path[50][50];
double singleProbability[50][50];
double finalProbability[50][101];

double GetProbability(int town, int daysPassed)
{
	double& ret = finalProbability[town][daysPassed];
	if (ret >= 0.0)
		return ret;

	ret = 0.0;
	for (int i = 0; i < n; ++i)
	{
		if (path[i][town] > 0)
			ret += GetProbability(i, daysPassed - 1) * singleProbability[i][town];
	}
	return ret;
}

int main()
{
	int C;
	cin >> C;
	while (C--)
	{
		cin >> n >> d >> p;

		memset(path, 0, sizeof(path));
		fill_n(&finalProbability[0][0], 50 * 101, -1.0);
		for (int i = 0; i < n; ++i)
		{
			int pathCount = 0;
			for (int j = 0; j < n; ++j)
			{
				cin >> path[i][j];
				pathCount += path[i][j];
			}

			for (int j = 0; j < n; ++j)
			{
				singleProbability[i][j] = pathCount > 0 ? ((double)path[i][j] / pathCount) : 0.0;
			}
		}

		for (int i = 0; i < n; ++i)
			finalProbability[i][1] = singleProbability[p][i];

		int t;
		cin >> t;
		while (t--)
		{
			int target;
			cin >> target;
			printf("%.8lf\n", GetProbability(target, d));
		}
	}
	return 0;
}
