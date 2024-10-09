/*
* Problem: https://www.acmicpc.net/problem/1086
* 분류: 동적계획법
*/
#include <iostream>
#include <string>
#include <memory.h>
#include <cassert>

using namespace std;

const int MAX_DIGITS = 17;
int N, K, wholeLen, len[15], series[15], tenSquareModded[751];
long long TenSquares[18], memo[1 << 15][100];
string seriesStr[15];

void CalcTenSquares()
{
	long long num = 1;
	for (int i = 0; i <= 17; ++i)
	{
		TenSquares[i] = num;
		num *= 10;
	}
}

int CalcModdedNSquaredTen(int n)
{
	assert(n >= 0);
	int& ret = tenSquareModded[n];
	if (ret != -1) return ret;

	if (n > MAX_DIGITS)
		return ret = (CalcModdedNSquaredTen(n - MAX_DIGITS) * CalcModdedNSquaredTen(MAX_DIGITS)) % K;
	else
		return ret = TenSquares[n] % K;
}

int CalcModdedNum(const string& numStr)
{
	int ret = 0, offset = 0;
	while (offset < numStr.size())
	{
		const int pieceSize = min(MAX_DIGITS, (int)numStr.size() - offset);
		const string pieceStr = numStr.substr(offset, pieceSize);

		long long moddedPiece = stoll(pieceStr) % K;
		ret += (moddedPiece * tenSquareModded[numStr.size() - pieceSize - offset]) % K;
		ret %= K;

		offset += pieceSize;
	}
	return ret;
}

long long DP(int modded, int lenLeft, int state)
{
	assert(lenLeft >= 0);
	long long& ret = memo[state][modded];
	if (ret != -1) return ret;

	if (state == (1 << N) - 1)
	{
		assert(lenLeft == 0);
		return ret = (modded == 0 ? 1 : 0);
	}

	ret = 0;
	for (int next = 0; next < N; ++next)
	{
		if ((state & 1 << next) == 0)
		{
			int newLenLeft = lenLeft - len[next];
			int newModded = (modded + series[next] * tenSquareModded[newLenLeft]) % K;
			int newState = state | (1 << next);
			ret += DP(newModded, newLenLeft, newState);
		}
	}
	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	memset(memo, -1, sizeof(memo));
	memset(tenSquareModded, -1, sizeof(tenSquareModded));

	cin >> N;
	wholeLen = 0;
	for (int i = 0; i < N; ++i)
	{
		cin >> seriesStr[i];
		len[i] = seriesStr[i].length();
		wholeLen += len[i];
	}
	cin >> K;

	CalcTenSquares();
	for (int i = 0; i < 751; ++i)
		CalcModdedNSquaredTen(i);

	for (int i = 0; i < N; ++i)
	{
		series[i] = CalcModdedNum(seriesStr[i]);
	}

	long long numerator = 0;
	for (int i = 0; i < N; ++i)
	{
		int lenLeft = wholeLen - len[i];
		int modded = (series[i] * tenSquareModded[lenLeft]) % K;
		int state = 1 << i;
		numerator += DP(modded, lenLeft, state);
	}

	long long denominator = 1;
	for (int i = 2; i <= N; ++i)
	{
		if (numerator % i == 0)
			numerator /= i;
		else
			denominator *= i;
	}

	cout << numerator << "/" << denominator << "\n";
	return 0;
}