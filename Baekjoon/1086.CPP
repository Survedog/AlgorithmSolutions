/*
* Problem: https://www.acmicpc.net/problem/1086 (박성원)
* 분류: 동적계획법
*
* 각 정수들의 자리수가 너무 커 Mod 연산을 적용할 때 오버플로우가 나지 않도록 하는 방법을 찾아야 한다.
* 정수 abc = a * 100 + b * 10 + c이므로, MOD 연산의 성질에 의해 abc % k = ((a % k) * (100 % k)) % k + ((b % k) * (10 % k)) % k + c % k임을 알 수 있다.
* 이를 이용하면 오버플로우가 나지 않게 하면서 정수들 뿐 아니라 그들을 합친 정수까지도 MOD K 연산을 적용할 수 있다. 이제 DP를 사용해 문제를 해결할 수 있다.
* state는 순열에 포함시킨 정수들의 현황, modded는 지금까지의 순열을 MOD K한 값이라하고, memo[state][modded]에 현재 상태에서 만들 수 있는 K로 나누어 떨어지는 모든 순열의 개수를 저장한다.
* 순열에 포함되지 않은 정수들의 집합을 x라고 할 때, x에 속한 모든 정수 x'에 대해 memo[state][modded] += memo[state | (1 << x')][(modded * ((10^(x'의 길이)) % K) + x' % K]) % K]를 수행하면
* memo[state][modded]값을 구할 수 있다. 이제 메모이제이션이 적용된 DFS를 통해 모든 경우를 탐색하면 정답은 memo[0][0]에 저장된다.

* 문제 해결) https://kibbomi.tistory.com/216를 참고함.
* 기약분수를 구할 때 현재의 방법 대신 GCD를 사용하니 무사히 해결할 수 있었다.
* 또한 문자열 정수들을 Mod 연산된 정수로 바꾸는 과정과 10의 pow의 MOD 값을 구하는 과정을 간략화시킬 수 있었다. (소스코드 참고)
*/
#include <iostream>
#include <string>
#include <memory.h>

using namespace std;

int N, K, series[15], tenSquareModded[51];
long long memo[1 << 15][100];
string seriesStr[15];

int CalcModdedNum(const string& numStr)
{
	int ret = 0;
	for (int i = 0; i < numStr.size(); ++i)
	{
		ret *= 10;
		ret += numStr[i] - '0';
		ret %= K;
	}
	return ret;
}

long long GCD(long long a, long long b)
{
	if (a < b) swap(a, b);

	while (b != 0)
	{
		long long remainder = a % b;
		a = b;
		b = remainder;
	}
	return a;
}

long long DP(int modded, int state)
{
	long long& ret = memo[state][modded];
	if (ret != -1) return ret;

	if (state == (1 << N) - 1)
	{
		return ret = (modded == 0 ? 1 : 0);
	}

	ret = 0;
	for (int next = 0; next < N; ++next)
	{
		if ((state & 1 << next) == 0)
		{
			int newModded = (modded * tenSquareModded[seriesStr[next].length()] + series[next]) % K;
			int newState = state | (1 << next);
			ret += DP(newModded, newState);
		}
	}
	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	memset(memo, -1, sizeof(memo));

	cin >> N;
	for (int i = 0; i < N; ++i)
	{
		cin >> seriesStr[i];
	}
	cin >> K;

	tenSquareModded[0] = 1 % K;
	for (int i = 1; i <= 50; ++i)
		tenSquareModded[i] = (tenSquareModded[i - 1] * 10) % K;

	for (int i = 0; i < N; ++i)
		series[i] = CalcModdedNum(seriesStr[i]);

	long long numerator = DP(0, 0), denominator = 1;
	for (int i = 2; i <= N; ++i)
		denominator *= i;

	long long gcd = GCD(numerator, denominator);
	cout << numerator / gcd << "/" << denominator / gcd << "\n";
	return 0;
}