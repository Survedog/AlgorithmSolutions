/*
* Problem: https://algospot.com/judge/problem/read/NERDS
* 책에서의 
* 구상) 먼저 신발 사이즈와 타이핑 속도를 각 차원으로 하는 2차원 평면을 만들고, 각 사람을 평면 상의 점으로 놓은 뒤 두 그룹을 나누는 방법을 찾기로 했다.
* 처음으로 고안한 방법: 한 그룹 내의 임의의 두 점의 쌍을 선택해 직선을 만들고, 그 직선이 두 그룹을 해당 직선으로 나눌 수 있는지 CCW를 통해 확인한다. 만약 직선들중 하나라도 나누는 게 가능하다면 두 그룹을 나눌 수 있다.
* 위 방법은 모든 점의 쌍을 순회하는 데 O(n^2), 두 점의 직선에서 다른 모든 점들에 대해 CCW를 수행하는 데 O(n)으로, 총 O(n^3)의 시간 복잡도가 필요하기 때문에 시간 초과가 발생할 것이라 생각하여 포기함.
* 
* 책의 해설을 참고하여 각 그룹의 Convex Hull을 만든 뒤 두 껍질이 서로 겹치는지 확인하는 방식을 시도해보기로 했다.
* 
* 시도1) 그룹의 점 중 맨 오른쪽에 있는 점(무조건 Convex Hull에 들어감)부터 시작하여, 벡터를 양의 x축으로부터의 각도로 표현하는 Polar 함수를 통해 다음 껍질 상 점을 알아냈다.
* (처음엔 CCW에 '현재 점 - 오른쪽 점(x+1) - 새로운 점'을 입력한 결과값을 사용해 찾으려 했으나, ccw는 sin 함수에 기반하므로 사이각이 90도를 넘어가면 다시 감소해버리기 때문에 가장 왼쪽에 있는 벡터를 구할 수 없다고 판단해 Polar를 사용했다)
* 이후 두 Convex Hull이 겹치는지 확인하기 위해, CCW를 사용해 두 그룹의 Convex Hull의 각 변들 중 교차하는 변이 있는지 확인했다.
* 예제는 무사히 통과했지만 실제 채점 시 오답이 발생했다.
* 
* 책해설)
* CCW를 '현재 점-임의의 점-새로운 점'과 같이 사용했을 때, Convex Hull의 특성 상 껍질 위 점에서 다른 점으로의 벡터는 180도 이상의 편차가 생기지 않으므로, 가장 왼쪽 벡터를 정의할 수 있음. (p.553 그림 15.16 참고)
* 따라서 CCW를 사용해 어느 것이 가장 왼쪽에 있는지 비교하는 것이 가능하다.
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>

using namespace std;

const double PI = acos(-1), INF = 987654321;
int n;
vector<pair<int, int>> nerd, normal;

inline double Polar(int x, int y)
{
	return fmod(atan2(y, x) + 2 * PI, 2 * PI);
}

int Cross(pair<int, int> v1, pair<int, int> v2)
{
	return v1.first * v2.second - v2.first * v1.second;
}

int CCW(pair<int, int> a, pair<int, int> b, pair<int, int> c)
{
	pair<int, int> ab = make_pair(b.first - a.first, b.second - a.second);
	pair<int, int> ac = make_pair(c.first - a.first, c.second - a.second);
	int cross = Cross(ab, ac);

	if (cross > 0) return 1;
	else if (cross < 0) return -1;
	else return 0;
}

bool Intersects(pair<int, int> a1, pair<int, int> a2, pair<int, int> b1, pair<int, int> b2)
{
	int aToB = CCW(a1, a2, b1) * CCW(a1, a2, b2);
	int bToA = CCW(b1, b2, a1) * CCW(b1, b2, a2);

	if (aToB == 0 && bToA == 0)
	{
		if (a2 < a1) swap(a1, a2);
		if (b2 < b1) swap(b1, b2);
		return !(b2 < a1 || a2 < b1);
	}

	return aToB <= 0 && bToA <= 0;
}

void MakeConvexHull(const vector<pair<int, int>>& points, vector<pair<int, int>>& outConvexHull)
{
	int rightCorner = 0;
	for (int i = 0; i < points.size(); ++i)
		if (points[i] > points[rightCorner])
			rightCorner = i;

	vector<bool> isContained(points.size(), false);
	outConvexHull.push_back(points[rightCorner]);
	isContained[rightCorner] = true;

	double lastPolar = 0;
	int cur = rightCorner;
	while (true)
	{
		int next = -1;
		double minPolar = INF;
		for (int i = 0; i < points.size(); ++i)
		{
			if (i == cur) continue;

			pair<int, int> relativePos = make_pair(points[i].first - points[cur].first, points[i].second - points[cur].second);
			double polar = Polar(relativePos.first, relativePos.second);
			if (polar >= lastPolar && polar < minPolar)
			{
				minPolar = polar;
				next = i;
			}
		}

		if (next == -1 || isContained[next])
			break;

		outConvexHull.push_back(points[next]);
		isContained[next] = true;
		lastPolar = minPolar;
		cur = next;
	}
}

bool CheckTheory()
{
	vector<pair<int, int>> nerdHull, normalHull;
	MakeConvexHull(nerd, nerdHull);
	MakeConvexHull(normal, normalHull);

	for (int i = 0; i < nerdHull.size(); ++i)
	{
		int j = (i + 1) % nerdHull.size();
		for (int p = 0; p < normalHull.size(); ++p)
		{
			int q = (p + 1) % normalHull.size();
			if (Intersects(nerdHull[i], nerdHull[j], normalHull[p], normalHull[q]))
				return false;
		}
	}
	return true;
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

		nerd.clear();
		normal.clear();
		for (int i = 0; i < n; ++i)
		{
			bool isNerd;
			pair<int, int> info;

			cin >> isNerd >> info.first >> info.second;
			if (isNerd)
				nerd.push_back(info);
			else
				normal.push_back(info);
		}

		cout << (CheckTheory() ? "THEORY HOLDS" : "THEORY IS INVALID") << "\n";
	}
	return 0;
}