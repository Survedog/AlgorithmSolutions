/*
* Problem: https://algospot.com/judge/problem/read/JAEHASAFE
* 시도1) KMP 알고리즘을 응용해 한 문자열의 접두사이면서 다른 문자열의 접미사가 되는 가장 긴 문자열의 길이를 구하는 함수를 만들었다.
* 이를 이용해 각 인접한 상태에 대해 겹치는 문자열의 최대 길이를 구하고, 그 값을 통해 다이얼을 얼마나 돌려야 하는지 계산하는 로직을 구현했다. (x만큼 겹칠경우 시계방향으론 'x', 반시계방향으론 '슬롯 수 - x' 만큼 돌리면 됨)
* 예제는 맞았으나 실제 채점땐 오답이 발생했고, 오답이 왜 발생하는지 알아내지 못했다.
* 
* 책해설) 한 상태를 자신과 이어붙인 문자열에서 다음 상태를 찾는다. 발견된 시작 위치가 곧 시계방향으로 돌려야 하는 횟수가 된다.
* 반시계 방향으로 돌려야 하는 경우, 순서를 반대로 하여 다음 상태 문자열을 두번 이어붙인 것에서 이전 상태를 찾으면 된다.
*/
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int N, slotCount;
string cur, state[101];

void GetPartialMatch(const string& str, vector<int>& outPI)
{
	int m = str.size();
	outPI = vector<int>(m, 0);

	int begin = 1, match = 0;
	while (begin + match < m)
	{
		if (str[begin + match] == str[match])
		{
			match++;
			outPI[begin + match - 1] = match;
		}
		else
		{
			if (match == 0)
				begin++;
			else
			{
				begin += match - outPI[match - 1];
				match = outPI[match - 1];
			}
		}
	}
}

int FindFirstMatch(const string& a, const string& b)
{
	int n = a.size(), m = b.size();
	vector<int> pi;
	GetPartialMatch(b, pi);

	int begin = 0, match = 0;
	while (begin < n)
	{
		if (a[begin + match] == b[match])
		{
			match++;
			if (match == m)
				return begin;
		}
		else
		{
			if (match == 0)
				begin++;
			else
			{
				begin += match - pi[match - 1];
				match = pi[match - 1];
			}
		}
	}
	return 0;
}

int GetMinRotation()
{
	int ret = 0;
	bool turn = true;
	for (int i = 0; i < N; ++i)
	{
		if (turn)
			ret += FindFirstMatch(state[i + 1] + state[i + 1], state[i]);
		else
			ret += FindFirstMatch(state[i] + state[i], state[i + 1]);
		turn = !turn;
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
		cin >> N >> state[0];
		for (int i = 1; i <= N; ++i)
			cin >> state[i];

		slotCount = state[0].size();
		cout << GetMinRotation() << "\n";
	}

	return 0;
}