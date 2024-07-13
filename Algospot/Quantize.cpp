#include <iostream>
#include <algorithm>
#include <memory.h>
#include <cmath>

#define INF 987654321

using namespace std;

int n, s, series[100], memo[100][10];

int GetSSE(int start, int end)
{
	int sum = 0;
	for (int i = start; i <= end; ++i)
		sum += series[i];
	int roundedMean = (int)(0.5 + (double)sum / (end - start + 1));

	int SSE = 0;
	for (int i = start; i <= end; ++i)
	{
		int error = series[i] - roundedMean;
		SSE += error * error;
	}

	return SSE;
}

int GetMinSSE(int start, int usedCount)
{
	if (start >= n)
		return 0;
	if (usedCount == s)
		return INF;

	int& ret = memo[start][usedCount];
	if (ret >= 0)
		return ret;

	ret = INF;
	for (int partSize = 1; start + partSize <= n; partSize++)
		ret = min(ret, GetMinSSE(start + partSize, usedCount + 1) + GetSSE(start, start + partSize - 1));

	return ret;
}

int main()
{
	int C;
	cin >> C;

	while (C--)
	{
		memset(series, 0, sizeof(series));
		memset(memo, -1, sizeof(memo));
		cin >> n >> s;
		for (int i = 0; i < n; ++i)
		{
			cin >> series[i];
		}
		sort(series, series + n);
		cout << GetMinSSE(0, 0) << "\n";
	}

	return 0;
}