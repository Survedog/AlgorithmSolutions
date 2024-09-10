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
*
* 개선) a, b가 주어졌을 때, (b, a % b)는 만나면 반드시 이길 수 있는 상태라고 가정하자.
* 이때 a >= 2 * b라면 첫 플레이어는 (a % b + b, b)를 만들 수 있고, 이 경우 다음 플레이어는 반드시 (b, a % b)를 만들도록 강제된다. 이러면 첫 플레이어는 반드시 승리할 수 있다.
* 반대로 (b, a % b)가 만나면 반드시 지는 상태라고 가정하면, 첫 플레이어가 (b, a % b)를 만들면 다음 플레이어는 반드시 지게 된다.
* 즉 a >= 2 * b이면 플레이어는 반드시 이길 수 있고, 아니라면 (b, a % b)에 승부 결과가 의존하게 된다.
* 해당 원리를 적용해 a < 2 * b일 때만 재귀하도록 하니, 약 0ms의 시간 안에 문제를 풀 수 있었다.
*/

#include <iostream>
#include <algorithm>

using namespace std;

long long N, M;

bool CanWin(long long a, long long b)
{
	if (b == 0) return false;
	if (a >= 2 * b) return true;
	return !CanWin(b, a % b);
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