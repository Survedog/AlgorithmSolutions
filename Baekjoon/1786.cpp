/*
* Problem: https://acmicpc.net/problem/1786
* 분류: 문자열, KMP 알고리즘
*/
#include <iostream>
#include <vector>
#include <string>

using namespace std;

string T, P;

void CalcPartialMatch(vector<int>& outPartialxMatch)
{
	outPartialxMatch.resize(P.length() + 1, 0);
	int begin = 1, matched = 0;
	while (begin + matched < P.length())
	{
		if (P[matched] == P[begin + matched])
		{
			matched++;
			outPartialxMatch[begin + matched] = matched;
		}
		else
		{
			if (matched == 0)
				begin++;
			else
			{
				begin += matched - outPartialxMatch[matched];
				matched = outPartialxMatch[matched];
			}
		}
	}
}

void KMP(vector<int>& outMatchIdx)
{
	vector<int> partialMatch;
	CalcPartialMatch(partialMatch);

	int begin = 0, matched = 0;
	while (begin + matched < T.length())
	{
		if (P[matched] == T[begin + matched])
		{
			matched++;
			if (matched == P.length())
			{
				outMatchIdx.push_back(begin + 1);
				begin += matched - partialMatch[matched];
				matched = partialMatch[matched];
			}
		}
		else
		{
			if (matched == 0)
				begin++;
			else
			{
				begin += matched - partialMatch[matched];
				matched = partialMatch[matched];
			}
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	getline(cin, T);
	getline(cin, P);
	vector<int> matchIdx;
	KMP(matchIdx);

	cout << matchIdx.size() << "\n";
	for (int i = 0; i < matchIdx.size(); ++i)
		cout << matchIdx[i] << " ";
	cout << "\n";
	return 0;
}