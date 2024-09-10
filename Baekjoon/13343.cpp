/*
* Problem: https://www.acmicpc.net/problem/13343
* 분류: 유클리드 호제법
* 
* 게임에서 a > b인 (a, b)가 주어졌을 때, 이후 게임에서 두 플레이어 중 하나는 반드시 (b, a % b)를 만나게 된다.
* 이때 a >= k * b라면 (k는 2 이상의 정수) 플레이어는 상대방에게 (a % b + b, b), (b, a % b) 중 하나를 주어 자신이 이후 게임에서 (b, a % b)를 만날 지 말지를 선택할 수 있다.
* 따라서 게임의 결과는 현재 플레이어가 (b, a % b)를 만나는 경우와 안 만나는 경우로 나눌 수 있다.
* 현재 알고리즘에선 플레이어에게 a, b가 주어졌을 때, (b, a % b)와 (a % b + b, b)(a >= 2*b인 경우만)의 두 가지 경우를 만들어서 재귀적으로 CanWin을 호출하고,
* 두 경우 중 하나라도 상대가 질 수 밖에 없다면 현재 플레이어의 승리로 간주하였다.
* -> 1028ms의 시간이 소요되었다.
*/

#include <iostream>
#include <algorithm>

using namespace std;

long long N, M;

bool CanWin(long long a, long long b)
{
	if (b == 0) return false;

	bool ret = false;
	if (a >= 2 * b) ret = !CanWin(a % b + b, b);
	ret |= !CanWin(b, a % b);
	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> M;
	if (N < M) swap(N, M);
	cout << (CanWin(N, M) ? "win" : "lose") << "\n";

	return 0;
}