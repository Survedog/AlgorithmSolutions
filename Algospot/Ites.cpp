/*
* Problem: https://algospot.com/judge/problem/read/ITES
* N이 최대 5천만까지 올라가기 때문에 O(N^2)의 시간복잡도를 가지는 완전탐색을 사용할 순 없었다.
* 대신 부분 수열에 속한 수들은 모두 이어져있고 모든 수가 모두 양수임을 이용해, 부분 수열의 맨 앞을 head, 맨 뒤를 tail로 설정한 뒤 tail을 늘려나가다 합이 K를 넘으면 head를 올리는 방식을 사용했다. (이는 선입 선출이므로 부분 수열은 큐에 저장함)
* 이 과정에서 부분 수열에 속한 수의 합이 K가 되는 경우를 셈으로써 합이 K인 부분 수열의 수를 구할 수 있었다.
*
* 이때 수열을 모두 저장하면 메모리 제한인 64MB를 넘어서기 때문에, 수열을 미리 만드는 대신 그때그때 필요할 때마다 계산하는 방식을 사용했다.
* (온라인 알고리즘: 입력이 모두 주어지지 않아도 계산을 시작할 수 있는 알고리즘 (교재 p.637))
* 이러면 부분 수열에 속한 수들만 저장하면 되고, 수들의 최소값은 1임을 생각해보면 부분 수열의 크기는 최대 K(<=5백만)이므로 메모리 제한을 넘지 않는 것을 확인할 수 있다.
*/
#include <iostream>
#include <queue>

using namespace std;
using llong = long long;

int N, K;
queue<int> part;

inline llong MakeA(llong prevA)
{
	return (prevA * 214013 + 2531011) % ((llong)1 << 32);
}

inline int MakeSignal(llong A)
{
	return A % 10000 + 1;
}

int GetKSumCount()
{
	int sum = 0, ret = 0;
	llong A = 1983;
	for (int i = 0; i < N; ++i)
	{
		int signal = MakeSignal(A);
		A = MakeA(A);

		part.push(signal);
		sum += signal;

		while (sum >= K)
		{
			if (sum == K) ret++;
			sum -= part.front();
			part.pop();
		}
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
		cin >> K >> N;
		part = queue<int>();
		cout << GetKSumCount() << "\n";
	}

	return 0;
}