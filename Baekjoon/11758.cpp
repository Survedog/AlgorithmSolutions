/*
* Problem: https://www.acmicpc.net/problem/11758
* 분류: 기하학(CCW)
* CCW를 그대로 사용하는 문제.
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

using namespace std;

int pos[3][2];

int CCW()
{
	int x1 = pos[1][0] - pos[0][0];
	int y1 = pos[1][1] - pos[0][1];

	int x2 = pos[2][0] - pos[0][0];
	int y2 = pos[2][1] - pos[0][1];

	int ret = x1 * y2 - x2 * y1;
	if (ret != 0) ret /= abs(ret);
	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 2; ++j)
			cin >> pos[i][j];

	cout << CCW() << "\n";
	return 0;
}