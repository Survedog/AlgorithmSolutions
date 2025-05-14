/*
* Problem: https://codeforces.com/problemset/problem/2091/E
* 1 <= a < b <= N이고 LCM(최소공배수) = 소수 * GCD(최대공약수)인 a, b 쌍의 개수 찾기.
*
* 동적 계획법으로 풀 수 있지 않을까? f(n)이 쌍의 개수일때, f(k - 1)과 f(k)의 관계는?
* f(k-1) = 1 <= a < b <= k - 1인 모든 a, b쌍 중 F(a, b) = LCM / GCD가 소수인 쌍의 개수
* f(k)에서 추가되는 a, b 쌍은 (1, k), (2, k), ..., (k-1, k)가 있다.
* f(k) = f(k-1) + {(1, k), (2, k), ~ (k-1, k) 중 F(a, b)가 소수인 쌍의 개수}
*
* a = a' * GCD, b = b' * GCD 라고 하면 LCM = a' * b' * GCD
* F(a,b) = LCM / GCD = a' * b'이므로, 만약 소수라면 a'는 1, b'는 소수여야 함.
* a' = 1이면 a = GCD이므로, b = a * b' (b'는 소수)의 형태를 가져야 함.
* 정리하면 F(a,b)가 소수인 a, b 쌍에 대해 (a, b) = (b/b', b)이 성립한다. (b'는 b의 소인수)
* 따라서 (1, k), (2, k), ~ (k-1, k) 중 F(a,b)가 소수인 쌍의 개수는 k의 소인수의 개수와 같다.
*
* -> k를 소인수분해한 뒤, f(k) = f(k-1) + (k의 소인수의 개수)라는 식을 사용해 반복적 동적계획법으로 풀 수 있었다.
*/

#include <iostream>
#include <cmath>
#include <memory.h>

using namespace std;

const int MAX_N = 10000000;
int n, maxFactor[MAX_N + 1], factorCnt[MAX_N + 1], ans[MAX_N + 1];

int main()
{
	memset(ans, -1, sizeof(ans));
	memset(factorCnt, 0, sizeof(factorCnt));
	maxFactor[0] = maxFactor[1] = -1;
	for (int i = 2; i <= MAX_N; ++i)
		maxFactor[i] = i;

	const int sqrtN = sqrt(MAX_N);
	for (int i = 2; i <= sqrtN; ++i)
		if (maxFactor[i] == i)
			for (int j = i * i; j <= MAX_N; j += i)
			{
				while (maxFactor[j] % i == 0)
					maxFactor[j] /= i;
				factorCnt[j]++;
			}

	for (int i = 2; i <= MAX_N; ++i)
		if (maxFactor[i] != 1)
			factorCnt[i]++;

	ans[2] = 1;
	for (int i = 3; i <= MAX_N; ++i)
		ans[i] = ans[i - 1] + factorCnt[i];

	int t;
	cin >> t;
	while (t--)
	{
		cin >> n;
		cout << ans[n] << "\n";
	}

	return 0;
}