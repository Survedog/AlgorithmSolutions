/*
* Problem: https://algospot.com/judge/problem/read/NUMBERGAME
* 상대의 점수를 최소화하는 동시에 내 점수를 최대화하는 수를 찾아야 한다. 이를 위해 현재 게임판에서 자신이 가질 수 있는 최대 점수 차를 구하는 재귀 함수를 구현한다.
* 재귀함수는 다음 깊이의 반환 값(다음 턴에서 상대방이 가질 수 있는 최대 점수차)를 음수로 반전시킨 값을 기반으로 현재 턴의 최대 점수 차를 구할 수 있기 때문에,
* 현재 플레이어가 누구인지의 정보를 별도로 제공하지 않는다.
*/

#include <iostream>
#include <memory.h>
#include <cassert>

using namespace std;

const int NEGINF = -987654321;
int n, board[50], memo[50][50];

int GetMaxScoreDiff(int start, int end)
{
	if (start == end)
		return board[start];
	if (start > end)
		return 0;

	int& ret = memo[start][end];
	if (ret != NEGINF) return ret;

	ret = board[start] - GetMaxScoreDiff(start + 1, end);
	ret = max(ret, board[end] - GetMaxScoreDiff(start, end - 1));

	if (end - start >= 1)
	{
		ret = max(ret, -GetMaxScoreDiff(start + 2, end));
		ret = max(ret, -GetMaxScoreDiff(start, end - 2));
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
		cin >> n;
		for (int i = 0; i < n; ++i)
			cin >> board[i];

		fill_n(&memo[0][0], 50 * 50, NEGINF);
		cout << GetMaxScoreDiff(0, n - 1) << "\n";
	}

	return 0;
}