/*
* Problem: https://codeforces.com/problemset/problem/2091/E
* 1 <= a < b <= N�̰� LCM(�ּҰ����) = �Ҽ� * GCD(�ִ�����)�� a, b ���� ���� ã��.
*
* ���� ��ȹ������ Ǯ �� ���� ������? f(n)�� ���� �����϶�, f(k - 1)�� f(k)�� �����?
* f(k-1) = 1 <= a < b <= k - 1�� ��� a, b�� �� F(a, b) = LCM / GCD�� �Ҽ��� ���� ����
* f(k)���� �߰��Ǵ� a, b ���� (1, k), (2, k), ..., (k-1, k)�� �ִ�.
* f(k) = f(k-1) + {(1, k), (2, k), ~ (k-1, k) �� F(a, b)�� �Ҽ��� ���� ����}
*
* a = a' * GCD, b = b' * GCD ��� �ϸ� LCM = a' * b' * GCD
* F(a,b) = LCM / GCD = a' * b'�̹Ƿ�, ���� �Ҽ���� a'�� 1, b'�� �Ҽ����� ��.
* a' = 1�̸� a = GCD�̹Ƿ�, b = a * b' (b'�� �Ҽ�)�� ���¸� ������ ��.
* �����ϸ� F(a,b)�� �Ҽ��� a, b �ֿ� ���� (a, b) = (b/b', b)�� �����Ѵ�. (b'�� b�� ���μ�)
* ���� (1, k), (2, k), ~ (k-1, k) �� F(a,b)�� �Ҽ��� ���� ������ k�� ���μ��� ������ ����.
*
* -> k�� ���μ������� ��, f(k) = f(k-1) + (k�� ���μ��� ����)��� ���� ����� �ݺ��� ������ȹ������ Ǯ �� �־���.
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