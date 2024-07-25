/*
* Problem: http://algospot.com/judge/problem/read/KAKURO2
* 우선 완전탐색을 위한 알고리즘을 고안하기로 했다.
* 처음엔 재귀 함수에 x, y, hUsed, vUsed(가로/세로로 사용했던 수를 저장)와 hint(앞으로 채워야 하는 수의 합)을 넣어, 힌트 블럭에서 시작하여 매 단계마다 오른쪽으로 한 번, 아래로 한 번 재귀하려고 했다.
* (힌트 블럭이 왼쪽이나 위에 있기 때문에 오른쪽과 아래로만 가도 모든 빈 칸을 순회할 수 있음)
* 그러나 이동했을 때 해당 가로 혹은 세로줄에서 이전까지 어떤 블럭을 사용했고(Used), 얼마만큼의 수를 더 채워야 하는지(hint) 알 수 없어 더 진행하는 것이 불가능했다.
* 이를 해결하기 위해 재귀함수가 무조건 왼쪽 위 블럭부터 시작하여 오른쪽으로만 이동하고(우측 끝에선 다음 줄의 첫 칸으로 이동), 매 단계마다 used, hint를 배열에 저장하도록 변경하기로 했다.
* 이렇게 순회 순서를 고정하니 각 단계에선 필요한 used과 hint 값이 미리 계산되어 있기 때문에 끝까지 동작을 이어나갈 수 있었다.
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <memory.h>

using namespace std;

int N, Q, board[20][20], hUsed[20][20], vUsed[20][20];
bool isBlank[21][21];
pair<int, int> hint[20][20]; // first: 세로, second: 가로

bool Search(int y, int x)
{
	if (x > N) { y++; x = 1; }
	if (y >= N) return true;

	int prevVHint = hint[y - 1][x].first;
	int prevHHint = hint[y][x - 1].second;

	if (!isBlank[y][x])
	{
		if (prevVHint > 0 || prevHHint > 0) return false;
		return Search(y, x + 1);
	}

	int& vHint = hint[y][x].first = prevVHint;
	int& hHint = hint[y][x].second = prevHHint;
	assert(vHint != -1 && hHint != -1);

	for (int num = 1; num <= min(9, min(vHint, hHint)); ++num)
	{
		int lastVUsed = vUsed[y - 1][x];
		int lastHUsed = hUsed[y][x - 1];
		if ((lastHUsed & (1 << num)) || lastVUsed & (1 << num)) continue;

		board[y][x] = num;
		vHint -= num;
		hHint -= num;
		int& curVUsed = vUsed[y][x] = vUsed[y - 1][x] | (1 << num);
		int& curHUsed = hUsed[y][x] = hUsed[y][x - 1] | (1 << num);
		bool success = Search(y, x + 1);

		vHint += num;
		hHint += num;
		curVUsed -= (1 << num);
		curHUsed -= (1 << num);

		if (success) return true;
	}
	return false;
}


int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> N;
		fill_n(&isBlank[0][0], 21 * 21, 0);
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
				cin >> isBlank[i][j];

		cin >> Q;
		fill_n(&hint[0][0], 20 * 20, make_pair(-1, -1));
		fill_n(&board[0][0], 20 * 20, 0);
		memset(hUsed, 0, sizeof(hUsed));
		memset(vUsed, 0, sizeof(vUsed));

		for (int i = 0; i < Q; ++i)
		{
			int y, x, direction, sum;
			cin >> y >> x >> direction >> sum;
			y--; x--;
			if (direction == 1)
				hint[y][x].first = sum;
			else
				hint[y][x].second = sum;
		}

		Search(1, 1);
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
				cout << board[i][j] << " ";
			cout << "\n";
		}
	}
	return 0;
}