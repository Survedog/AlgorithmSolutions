/*
* Problem: https://algospot.com/judge/problem/read/HABIT
* 접미사 배열에서 인접한 두 접미사의 공통 접두사는 두 번 이상 등장하는 부분 문자열임을 이용했다.
* 멘버-마이어스 알고리즘으로 접미사 배열을 구한 후 인접한 접미사의 공통 접두사를 비교하며 해당 부분 문자열이 얼마나 등장하는지 셌다.
* 그 중 K번 이상 등장한 부분 문자열 중 가장 긴 문자열의 길이를 구했다.
* 
* 책 해설) 현재 부분 문자열(접두사)이 K번 나오는지 확인할 필요 없이, 접미사 배열 상 현재 접미사에서 K - 1번째 떨어진 접미사와의 공통 접두사의 길이를 구하면 된다.
* 해당 접두사가 n번 등장한다면 이후 n - 1번째 접미사까지는 해당 접두사를 가질 것이기 때문이다.
*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

char group[4000];
int K;
string script;

struct Comparator
{
	const vector<int>& group;
	int t;
	Comparator(const vector<int>& group, int t) : group(group), t(t) {}

	bool operator() (int a, int b)
	{
		if (group[a] != group[b]) return group[a] < group[b];
		return group[a + t] < group[b + t];
	}
};

// 교재 구현을 참고함
void GetSuffixArray(const string& s, vector<int>& outSuffixArray)
{
	int n = s.size();
	int t = 1;
	vector<int> group(n + 1);
	for (int i = 0; i < n; ++i) group[i] = s[i];
	group[n] = -1;

	if (outSuffixArray.size() < n) outSuffixArray.resize(n);
	for (int i = 0; i < n; ++i) outSuffixArray[i] = i;

	Comparator comp(group, t);
	while (t < n)
	{
		comp.t = t;
		sort(outSuffixArray.begin(), outSuffixArray.end(), comp);

		t *= 2;
		if (t >= n) break;

		vector<int> newGroup(n + 1);
		newGroup[n] = -1;
		newGroup[outSuffixArray[0]] = 0;
		for (int i = 1; i < n; ++i)
			if (comp(outSuffixArray[i - 1], outSuffixArray[i]))
				newGroup[outSuffixArray[i]] = newGroup[outSuffixArray[i - 1]] + 1;
			else
				newGroup[outSuffixArray[i]] = newGroup[outSuffixArray[i - 1]];

		group = newGroup;
	}
}

int GetLongestHabitLength(const vector<int>& suffix)
{
	int n = suffix.size(), ret = 0;
	vector<int> count(script.size() + 1, 0);
	for (int i = 0; i < n; ++i)
	{
		int idx = 0, same = 0;
		if (i > 0)
		{
			while (idx < min(n - suffix[i - 1], n - suffix[i]))
			{
				if (script[suffix[i - 1] + idx] == script[suffix[i] + idx])
				{
					same++;
					idx++;
				}
				else
					break;
			}

			for (int j = same; j < n - suffix[i - 1]; ++j)
				count[j] = 0;
		}

		for (int j = 0; j < n - suffix[i]; ++j)
		{
			count[j]++;
			if (count[j] >= K) ret = max(ret, j + 1);
		}
	}

	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> K >> script;
		vector<int> suffixArray(script.size());
		GetSuffixArray(script, suffixArray);
		cout << GetLongestHabitLength(suffixArray) << "\n";
	}

	return 0;
}