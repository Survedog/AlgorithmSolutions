/*
* Problem: https://algospot.com/judge/problem/read/KLIS
* 수열의 특정 위치부터 시작하는 lis의 길이와 개수를 계산하는 함수를 구현한 뒤, lis를 재구성하는 함수에서 이 위치별 lis 개수 정보를 사용해 k - 1(skip 값)만큼의 lis를 생략하였다.
* 이때 lis들을 오름차순으로 생성하기 위해 lis의 다음 원소가 될 수 있는 후보들을 오름차순으로 정렬한 후 이들을 순회하였음.
*
* 문제 출력에 lis의 길이를 넣지 않아 틀림. 문제 내용을 더 세심하게 읽을 것.
*/

#include <iostream>
#include <memory.h>
#include <algorithm>
#include <vector>

using namespace std;

int n, k, series[500], memo[501], lisCount[501];
const int Max = 2000000000 + 1;

int GetLISLength(int start)
{
	int& ret = memo[start + 1];
	if (ret != -1) return ret;

	ret = 1;
	for (int next = start + 1; next < n; next++)
	{
		if (start == -1 || series[next] > series[start])
		{
			int candiLen = GetLISLength(next) + 1;
			if (ret < candiLen)
			{
				ret = candiLen;
				lisCount[start + 1] = lisCount[next + 1];
			}
			else if (ret == candiLen)
			{
				lisCount[start + 1] = min<long long>(Max, (long long)lisCount[start + 1] + lisCount[next + 1]);
			}
		}
	}

	return ret;
}

void Reconstruct(int start, int skip, vector<int>& lis)
{
	if (start != -1)
		lis.push_back(series[start]);

	vector<pair<int, int>> candidates;
	for (int next = start + 1; next < n; ++next)
		if ((start == -1 || series[next] > series[start]) && GetLISLength(next) + 1 == GetLISLength(start))
			candidates.push_back({ series[next], next });

	sort(candidates.begin(), candidates.end());
	for (const pair<int, int>& candi : candidates)
	{
		int next = candi.second;
		if (skip >= lisCount[next + 1])
			skip -= lisCount[next + 1];
		else
		{
			Reconstruct(next, skip, lis);
			break;
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int c;
	cin >> c;
	while (c--)
	{
		cin >> n >> k;

		for (int i = 0; i < n; ++i)
		{
			cin >> series[i];
		}

		memset(memo, -1, sizeof(memo));
		fill_n(lisCount, 501, 1);

		GetLISLength(-1);

		vector<int> kthLis;
		Reconstruct(-1, k - 1, kthLis);

		cout << GetLISLength(-1) - 1 << "\n";
		for (int i : kthLis)
			cout << i << " ";
		cout << "\n";
	}

	return 0;
}