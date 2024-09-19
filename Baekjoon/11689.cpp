/*
* Problem: https://www.acmicpc.net/problem/11689
* 분류: 정수론, 오일러 피 함수
* 
* 오일러 피 함수의 성질에 의해, 자연수 N과 서로소이면서 1 <= M <= N을 만족하는 M의 개수는 N * (N의 소인수 p에 대한)∏(1 - 1/p)이다.
* 이를 실수 연산이나 오버플로우가 발생하지 않도록 N * ∏((p - 1)/p)로 계산해 답을 구했다.
*/
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

long long N;

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N;
	int sqrtN = (int)sqrt(N);

	long long num = N;
	vector<long long> primeDivs;
	for (int i = 2; i <= sqrtN; ++i)
	{
		if (num % i == 0)
		{
			primeDivs.push_back(i);
			while (num % i == 0)
				num /= i;
		}
	}

	if (num != 1)
		primeDivs.push_back(num);

	long long answer = N;
	for (long long primeDiv : primeDivs)
	{
		answer /= primeDiv;
		answer *= primeDiv - 1;
	}

	cout << answer << "\n";
	return 0;
}