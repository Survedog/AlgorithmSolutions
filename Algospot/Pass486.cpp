/*
* Problem: https://algospot.com/judge/problem/read/PASS486
* 에라토스테네스의 체를 응용한 알고리즘을 사용해 풀었다.
* 숫자를 1부터 올려가며 해당 숫자의 배수를 순회하며 divCount(약수의 개수를 저장하는 배열)를 1씩 올려준다.
* 이때 '임의의 숫자 n의 약수의 개수 = sqrt(n) 미만 약수의 개수 * 2 (숫자가 자연수의 제곱이라면 +1)' 임을 이용하여, 모든 숫자의 sqrt 이하 약수의 개수만 구한 뒤 이 공식을 이용해 전체 약수 개수를 구한다.
* 이 방법을 사용하면 hi까지 순회할 필요 없이 sqrt(n)까지만 가도 되기에 시간초과를 방지할 수 있다.
* 이후 divCount에서 약수의 개수가 n인 수의 개수를 세면 문제를 풀 수 있다.
*/
#include <iostream>
#include <memory.h>
#include <cmath>

using namespace std;

const int MAX_HI = 10000000;
int n, lo, hi, divCount[10000001];

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int c;
	cin >> c;

	memset(divCount, 0, sizeof(divCount));
	fill_n(divCount, MAX_HI + 1, 1);

	int sqrtHi = int(sqrt(MAX_HI));
	for (int i = 2; i <= sqrtHi; ++i)
		for (int j = 1; i * j <= MAX_HI; ++j)
			if (i < sqrt(i * j))
				divCount[i * j]++;

	for (int i = 2; i <= MAX_HI; ++i)
	{
		divCount[i] *= 2;
		int sqrtI = (int)sqrt(i);
		if (sqrtI * sqrtI == i)
			divCount[i]++;
	}

	while (c--)
	{
		cin >> n >> lo >> hi;
		int ans = 0;
		for (int i = lo; i <= hi; ++i)
			if (divCount[i] == n)
				ans++;
		cout << ans << "\n";
	}

	return 0;
}