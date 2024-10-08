/*
* Problem: https://www.acmicpc.net/problem/1029
* 분류: 동적계획법
* 
* (그림을 산 사람들의 명단, 현재의 주인, 마지막으로 구매된 가격)의 3가지 값이 있다면, 그 상태에서 그림을 계속 팔았을 때 현재 주인을 포함한 소유자들의 최대 수가 정해진다.
* 따라서 위의 세가지 값에 대한 최대 소유자 수를 메모이제이션하며 최대 소유자 수를 구하면 된다. (그림을 산 사람들의 명단을 인덱스화 시키기 위해 비트마스킹을 사용했다.)
* 이 경우 최대 시간 복잡도는 O(중복 문제의 수 * 매 DP마다의 반복문 실행 수) = O(구매자 명단의 경우의 수 * 주인의 경우의 수 * 가격의 경우의 수 * N) = O(2^N * N * 10 * N)으로,
* 2^N * N * 10 * N은 N의 최댓값 15에 대해 1억을 넘지 않으므로 제한시간 내에 충분히 해결할 수 있다.
* 
* 개선점) 중복 문제의 수를 착각하지 않도록 메모이제이션할 요소들이 모두 갖추어졌는지 꼼꼼히 확인하자.
*/
#include <iostream>
#include <memory.h>
#include <algorithm>

using namespace std;

int N, price[15][15], memo[2 << 15][15][10];

int DP(int owner, int lastPrice, int state)
{
	int& ret = memo[state][owner][lastPrice];
	if (ret != -1) return ret;

	ret = 1;
	if (state == (1 << N) - 1) return ret;

	for (int buyer = 1; buyer < N; ++buyer)
		if ((state & 1 << buyer) == 0 && lastPrice <= price[owner][buyer])
			ret = max(ret, DP(buyer, price[owner][buyer], state | 1 << buyer) + 1);

	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
		{
			char p;
			cin >> p;
			price[i][j] = p - '0';
		}

	memset(memo, -1, sizeof(memo));
	cout << DP(0, 0, 1) << "\n";
	return 0;
}