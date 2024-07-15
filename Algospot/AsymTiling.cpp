// Problem: https://algospot.com/judge/problem/read/ASYMTILING

#include <iostream>
#include <memory.h>
#include <cassert>

using namespace std;

const int divider = 1000000007;
int memo[101];

int GetCaseCount(int width)
{
	assert(width >= 0);
	if (width == 2)
		return 2;
	else if (width <= 1)
		return 1;

	int& ret = memo[width];
	if (ret >= 0)
		return ret;

	ret = (GetCaseCount(width - 1) + GetCaseCount(width - 2)) % divider;
	return ret;
}

int main()
{
	int C;
	cin >> C;
	while (C--)
	{
		memset(memo, -1, sizeof(memo));
		int n;
		cin >> n;
		if (n % 2 == 1)
		{
			int result = GetCaseCount(n) - GetCaseCount((n - 1) / 2);
			result %= divider;
			if (result < 0)
				result += divider;
			cout << result << "\n";
		}
		else
		{
			int result = GetCaseCount(n) - GetCaseCount(n / 2);
			result %= divider;
			result -= GetCaseCount((n - 2) / 2);
			result %= divider;
			if (result < 0)
				result += divider;
			cout << result << "\n";
		}
	}

	return 0;
}
