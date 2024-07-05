// Problem: https://algospot.com/judge/problem/read/WILDCARD

#include <iostream>
#include <string>
#include <memory.h>
#include <vector>
#include <algorithm>

using namespace std;

int memo[100][100];
vector<string> answer;

bool IsMatching(const string& w, const string& s, int wPos, int sPos)
{
	if (wPos >= w.length())
		return sPos >= s.length();

	int& ret = memo[wPos][sPos];
	if (ret != -1)
	{
		return ret;
	}

	while (wPos < w.length() && sPos < s.length() && (w[wPos] == s[sPos] || w[wPos] == '?'))
	{
		wPos++;
		sPos++;
	}

	if (wPos == w.length())
		return sPos == s.length();

	if (w[wPos] == '*')
	{
		for (int skip = 0; sPos + skip <= s.length(); skip++)
		{
			if (IsMatching(w, s, wPos + 1, sPos + skip))
			{
				return ret = true;
			}
		}
	}
	return ret = false;
}

int main()
{
	int C = 0;
	cin >> C;

	while (C--)
	{
		string W;
		cin >> W;
		int n = 0;
		cin >> n;

		while (n--)
		{
			string S;
			cin >> S;

			memset(memo, -1, sizeof(memo));

			if (IsMatching(W, S, 0, 0))
			{
				answer.push_back(S);
			}
		}

		sort(answer.begin(), answer.end());
		for (const string& s : answer)
		{
			cout << s << "\n";
		}
		answer.clear();
	}

	return 0;
}
