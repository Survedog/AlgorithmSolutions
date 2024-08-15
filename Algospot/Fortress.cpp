/*
* Problem: https://algospot.com/judge/problem/read/FORTRESS
* 요새 사이의 포함 관계를 트리로 표현한 후 트리의 지름을 구했다.
*/
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

typedef struct Circle {
	int x, y, r;

	Circle() : x(0), y(0), r(0) {}
	Circle(int x, int y, int r) : x(x), y(y), r(r) {}

	bool IsIncluding(const Circle& other) const
	{
		if (r <= other.r) return false;
		return hypot(x - other.x, y - other.y) + other.r < r;
	}

	bool operator<(const Circle& other) const
	{
		return r < other.r;
	}
} Circle;

int N, childMaxHeights[100][2];
Circle circles[100];

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> N;
		for (int i = 0; i < N; ++i)
		{
			int x, y, r;
			cin >> x >> y >> r;
			circles[i] = Circle(x, y, r);

			childMaxHeights[i][0] = 0;
			childMaxHeights[i][1] = 0;
		}

		int answer = 0;
		sort(&circles[0], &circles[N]);
		for (int i = 0; i < N; ++i)
			for (int j = i + 1; j < N; ++j)
				if (circles[j].IsIncluding(circles[i]))
				{
					int height = childMaxHeights[i][0] + 1;
					if (childMaxHeights[j][0] < height)
					{
						swap(childMaxHeights[j][0], childMaxHeights[j][1]);
						childMaxHeights[j][0] = height;
					}
					else
						childMaxHeights[j][1] = max(childMaxHeights[j][1], height);
					answer = max(answer, childMaxHeights[j][0] + childMaxHeights[j][1]);
					break;
				}

		cout << answer << "\n";
	}
	return 0;
}