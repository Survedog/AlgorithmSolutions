#include <iostream>
#include <vector>
#include <memory.h>

#define NEGINF -2147483649LL

using namespace std;

int n, m;
vector<int> A, B;
int memo[101][101];

int GetJLISLength(int startA, int startB)
{
	int& ret = memo[startA + 1][startB + 1];
	if (ret != -1)
		return ret;

	ret = 0;
	const long long currentANum = startA >= 0 ? A[startA] : NEGINF;
	const long long currentBNum = startB >= 0 ? B[startB] : NEGINF;
	const long long maxElement = max(currentANum, currentBNum);

	for (int nextA = startA + 1; nextA < n; nextA++)
		if (maxElement < A[nextA])
			ret = max(ret, GetJLISLength(nextA, startB) + 1);

	for (int nextB = startB + 1; nextB < m; nextB++)
		if (maxElement < B[nextB])
			ret = max(ret, GetJLISLength(startA, nextB) + 1);

	return ret;
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
	memset(memo, -1, sizeof(memo));
}

int main()
{
	int C = 0;
	cin >> C;
	while (C--)
	{
		Init();
		cout << GetJLISLength(-1, -1) << "\n";
	}
	return 0;
}
