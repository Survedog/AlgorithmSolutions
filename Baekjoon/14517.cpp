/*
* Problem: https://www.acmicpc.net/problem/14517
* 분류: 동적계획법
*/
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int DIVIDER = 10007;
vector<int> matches[1000];
vector<vector<int>> memo;
string S;

int CountPalindromes()
{
	memo[0][0] = 1;
	for (int i = 1; i < S.size(); ++i)
	{
		memo[i][i] = 1;
		memo[i - 1][i] = S[i - 1] == S[i] ? 3 : 2;
	}

	for (int len = 3; len <= S.size(); ++len)
	{
		for (int i = 0; i <= S.size() - len; ++i)
		{
			int j = i + len - 1;
			if (S[i] == S[j])
				memo[i][j] = (memo[i + 1][j] + memo[i][j - 1] + 1) % DIVIDER;
			else
				memo[i][j] = (memo[i + 1][j] + memo[i][j - 1] - memo[i + 1][j - 1]) % DIVIDER;
		}
	}
	return memo[0][S.size() - 1];
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> S;
	memo.resize(S.size(), vector<int>(S.size(), -1));
	cout << CountPalindromes() << "\n";
	return 0;
}