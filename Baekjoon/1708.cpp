/*
* Problem: https://www.acmicpc.net/problem/1708
* 분류: 기하학, CCW, 볼록 껍질(Convex Hull)
* 
* 볼록 껍질 상의 한 점에 대해, 블록 껍질 상의 다음 점은 현재 점에서 최대한 반시계 방향(블록 껍질 상의 이전 점을 기준으로 잡았을 때)에 위치한 점이다.
* 볼록 껍질에 속하는 점에서 블록 껍질 위나 내부의 다른 점들 두 개를 임의로 선택해 선분을 그었을 때, 두 선분이 이루는 각도는 반드시 180도 이하이다.
* 따라서 블록 껍질에 속하는 점 a가 있을 때, 임의의 두 점 b, c을 선택해 a->b와 a->c의 CCW를 구하면 b, c 중 어느 것이 더 반시계 방향에 있는지 알 수 있다.
* 이렇게 상대적인 비교를 반복하면 가장 반시계 방향에 있는 점, 즉 볼록 껍질 상의 다음 점을 구할 수 있다.
* 
* 가장 왼쪽에 있는 점들 중 가장 아래에 있는 점은 반드시 블록 껍질에 속하므로, 이를 찾아 위 과정을 시작하면 된다.
*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using pll = pair<long long, long long>;

const int MAX_POINT = 100000;
int N;
pll pos[MAX_POINT];

pll operator-(const pll& a, const pll& b)
{
	return make_pair(a.first - b.first, a.second - b.second);
}

long long DistSquared(const pll& a, const pll& b)
{
	pll offset = b - a;
	return offset.first * offset.first + offset.second * offset.second;
}

long long CrossProductZ(const pll& p, const pll& q)
{
	return p.first * q.second - p.second * q.first;
}

int CCW(const pll& a, const pll& b, const pll& c)
{
	pll ab = b - a, ac = c - a;
	long long cp = CrossProductZ(ab, ac);
	if (cp < 0) return -1;
	if (cp > 0) return 1;
	return 0;
}

void MakeConvexHull(vector<pll>& outConvexHull)
{
	pll start = *min_element(pos, pos + N);
	pll pivot = start;
	outConvexHull.push_back(start);

	while (true)
	{
		pll next = pos[1];
		long long distS = DistSquared(pivot, next);

		for (int i = 0; i < N; ++i)
		{
			int ccw = CCW(pivot, next, pos[i]);
			long long newDistS = DistSquared(pivot, pos[i]);
			if (ccw < 0 || ccw == 0 && newDistS > distS)
			{
				next = pos[i];
				distS = newDistS;
			}
		}

		if (start == next) break;

		pivot = next;
		outConvexHull.push_back(pivot);
	}
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N;
	for (int i = 0; i < N; ++i)
	{
		cin >> pos[i].first >> pos[i].second;
	}

	vector<pll> convexHull;
	MakeConvexHull(convexHull);
	cout << convexHull.size() << "\n";

	return 0;
}