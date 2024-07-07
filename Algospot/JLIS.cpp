#include <iostream>
#include <vector>
#include <memory.h>

using namespace std;

int n, m;
vector<int> A, B;
int memo[200];

int GetLISLength(const vector<int>& series, int start)
{
	if (start == series.size() - 1)
	{
		return 1;
	}

	if (start == -1)
	{
		int ret = 1;
		for (int i = 0; i < series.size(); i++)
		{
			ret = max(ret, GetLISLength(series, i));
		}
		return ret;
	}
	else
	{
		int& ret = memo[start];
		if (ret != -1)
		{
			return ret;
		}

		ret = 1;
		for (int i = start + 1; i < series.size(); i++)
		{
			if (series[start] < series[i])
				ret = max(ret, GetLISLength(series, i) + 1);
		}
		return ret;
	}
}

void Init()
{
	cin >> n >> m;

	A.clear(); B.clear();
	{
		int nTemp = n;
		int mTemp = m;

		while (nTemp--)
		{
			int element;
			cin >> element;
			A.push_back(element);
		}

		while (mTemp--)
		{
			int element;
			cin >> element;
			B.push_back(element);
		}
	}
}

int main()
{
	int C = 0;
	cin >> C;
	while (C--)
	{
		Init();

		int result = 0;
		vector<int> joined(A);
		joined.insert(joined.end(), B.begin(), B.end());
		memset(memo, -1, sizeof(memo));
		result = GetLISLength(joined, -1);

		joined = B;
		joined.insert(joined.end(), A.begin(), A.end());
		memset(memo, -1, sizeof(memo));
		result = max(result, GetLISLength(joined, -1));

		cout << result << "\n";
	}

	return 0;
}
