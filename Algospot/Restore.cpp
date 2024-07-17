/*
* (미해결)
* Problem: https://algospot.com/judge/problem/read/RESTORE
* 단어들을 우측으로 이어가며 겹치는 문자 개수가 가장 큰 조합을 찾는다. 외판원 문제와 거의 비슷한 알고리즘으로 풀이가 가능하다.
* 새 부분 문자열이 기존 부분 문자열에, 혹은 기존 부분 문자열이 새 부분 문자열에 완전히 포함되는(inclusion) 경우 포함되는 문자열을 지우고 더 큰(포함하는) 문자열만 남기는 전처리 과정을 수행한다.
* 전처리 과정에서 새 문자열이 기존 문자열을 포함하는 경우 새 문자열로 기존 문자열을 대체해야 하는데 하지 않아 오답이 발생함. 구현의 신뢰성에 좀 더 신경쓰고, 더 다양한 테스트 케이스를 실험하도록 주의해야겠음.
*/

#include <iostream>
#include <memory.h>
#include <string>
#include <algorithm>
#include <cassert>

using namespace std;

int k, memo[16][1 << 15], choice[16][1 << 15], overlapSize[16][16];
string subString[15];

int GetOverlapSize(const string& A, const string& B)
{
	for (int intersect = min(A.size(), B.size()); intersect >= 0; intersect--)
	{
		string subA = A.substr(A.size() - intersect);
		string subB = B.substr(0, intersect);
		if (subA == subB)
		{
			return intersect;
		}
	}
	assert(false);
	return 0;
}

void PreCalcOverlapSize()
{
	for (int i = 0; i < k; ++i)
		for (int j = 0; j < k; ++j)
			overlapSize[i + 1][j + 1] = GetOverlapSize(subString[i], subString[j]);
}

bool CheckInclusion(const string* big, const string* small)
{
	if (big->size() < small->size())
		swap(big, small);

	for (int offset = 0; offset <= big->size() - small->size(); offset++)
	{
		if (big->substr(offset, small->size()) == *small)
			return true;
	}
	return false;
}

int GetMaxOverlapSize(int last, int added)
{
	if (added == (1 << k) - 1)
		return 0;

	int& ret = memo[last + 1][added];
	if (ret != -1) return ret;

	ret = -1;
	for (int next = 0; next < k; ++next)
	{
		if ((added & (1 << next)) == 0)
		{
			int nextAdded = added | (1 << next);
			int candiSize = GetMaxOverlapSize(next, nextAdded) + overlapSize[last + 1][next + 1];

			if (candiSize > ret)
			{
				ret = candiSize;
				choice[last + 1][added] = next;
			}
		}
	}
	return ret;
}

//void Reconstruct(int last, int added, string& s)
//{
//	if (added == (1 << k) - 1)
//		return;
//
//	int chosen = choice[last + 1][added];
//	if (chosen != -1)
//	{
//		int overlap = overlapSize[last + 1][chosen + 1];
//		s.append(subString[chosen].substr(overlap));
//
//		int nextAdded = added | (1 << chosen);
//		Reconstruct(chosen, nextAdded, s);
//	}
//	else
//		assert(false);
//}

string Reconstruct(int last, int added)
{
	if (added == (1 << k) - 1) return "";

	for (int next = 0; next < k; ++next)
	{
		if (next & (1 << next)) continue;

		int ifAdded = GetMaxOverlapSize(next, added + (1 << next)) + overlapSize[last + 1][next + 1];
		if (ifAdded == GetMaxOverlapSize(last, added))
			return (subString[next].substr(overlapSize[last + 1][next + 1]) + Reconstruct(next, added + (1 << next)));
	}
	assert(false);
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> k;
		for (int i = 0; i < k; ++i)
		{
			memset(memo, -1, sizeof(memo));
			memset(choice, -1, sizeof(choice));
			memset(overlapSize, 0, sizeof(overlapSize));

			string newString;
			cin >> newString;

			bool isIncluded = false;
			for (int j = 0; j < i; ++j)
			{
				if (CheckInclusion(&newString, &subString[j]))
				{
					if (newString.size() > subString[j].size())
						swap(newString, subString[j]);
					isIncluded = true;
					break;
				}
			}

			if (isIncluded)
			{
				i--;
				k--;
			}
			else
				subString[i] = newString;
		}

		PreCalcOverlapSize();
		GetMaxOverlapSize(-1, 0);
		//string original;
		//Reconstruct(-1, 0, original);
		cout << Reconstruct(-1, 0) << "\n";
	}

	return 0;
}