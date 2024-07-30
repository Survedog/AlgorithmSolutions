/*
* Problem: https://algospot.com/judge/problem/read/FOSSIL
* 책 해설) 두 볼록 다각형의 겹친 부분은 볼록 다각형이고, 볼록 다각형의 구간 별 높이는 유니모달 함수의 형태를 띈다는 것을 이용해 삼분검색으로 해결하였다.
* 이때 두 볼록 다각형 모두를 윗 껍질과 아랫껍질로 나눠, 윗 껍질 중 특정 x에서 더 낮은 것의 y값이 겹친 부분의 윗 껍질이고, 아랫 껍질 중 더 높은 것이 겹친 부분의 아랫 껍질이라는 사실을 통해 겹친 부분의 각 x에서의 높이를 구할 수 있었다.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

using namespace std;

const double SMALL_NUMBER = 1e-9;

typedef struct Line {
	vector<double> coef;
	pair<double, double> xRange;

	Line(double x1, double y1, double x2, double y2)
	{
		double slope = (y2 - y1) / (x2 - x1);
		double offset = y1 - slope * x1;
		coef.push_back(slope);
		coef.push_back(offset);

		xRange.first = min(x1, x2);
		xRange.second = max(x1, x2);
	}

	bool operator<(const Line& other) const
	{
		return xRange.first < other.xRange.first;
	}
} Line;

vector<Line> upperLines[2], lowerLines[2];

bool IsNearlyZero(double x)
{
	const double threshold = 1e-7;
	return x <= threshold && x >= -threshold;
}

inline Line GetMatchingLine(const vector<Line>& vec, double x)
{
	for (const Line& line : vec)
		if (x >= line.xRange.first - SMALL_NUMBER && x <= line.xRange.second + SMALL_NUMBER)
			return line;

	assert(false);
	return Line(-1, -1, -1, -1);
}

inline double GetY(const Line& line, double x)
{
	assert(x >= line.xRange.first - SMALL_NUMBER && x <= line.xRange.second + SMALL_NUMBER);
	return line.coef[0] * x + line.coef[1];
}

inline double GetY(const vector<Line>& vec, double x)
{
	const Line& line = GetMatchingLine(vec, x);
	return GetY(line, x);
}

double GetMaxHeight(double lo, double hi)
{
	int iter = 100;
	while (iter--)
	{
		double left = (2 * lo + hi) / 3;
		double right = (lo + 2 * hi) / 3;

		double fUpper = min(GetY(upperLines[0], left), GetY(upperLines[1], left));
		double fLower = max(GetY(lowerLines[0], left), GetY(lowerLines[1], left));
		double fLeft = fUpper - fLower;

		fUpper = min(GetY(upperLines[0], right), GetY(upperLines[1], right));
		fLower = max(GetY(lowerLines[0], right), GetY(lowerLines[1], right));
		double fRight = fUpper - fLower;

		if (fLeft > fRight)
			hi = right;
		else if (fLeft < fRight)
			lo = left;
		else
		{
			hi = right;
			lo = left;
		}
	}

	double mid = (lo + hi) / 2;
	double fUpper = min(GetY(upperLines[0], mid), GetY(upperLines[1], mid));
	double fLower = max(GetY(lowerLines[0], mid), GetY(lowerLines[1], mid));
	double ret = fUpper - fLower;
	return ret > 0 ? ret : 0;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout << fixed;
	cout.precision(8);

	int C;
	cin >> C;
	while (C--)
	{
		int pointCount[2];
		cin >> pointCount[0] >> pointCount[1];

		upperLines[0].clear();
		lowerLines[0].clear();
		upperLines[1].clear();
		lowerLines[1].clear();

		for (int group = 0; group < 2; ++group)
		{
			double firstX, firstY, lastX, lastY;
			cin >> firstX >> firstY;
			lastX = firstX; lastY = firstY;

			for (int i = 1; i < pointCount[group]; ++i)
			{
				double x, y;
				cin >> x >> y;
				if (x < lastX)
					upperLines[group].emplace_back(x, y, lastX, lastY);
				else
					lowerLines[group].emplace_back(x, y, lastX, lastY);
				lastX = x;
				lastY = y;
			}

			if (firstX < lastX)
				upperLines[group].emplace_back(lastX, lastY, firstX, firstY);
			else
				lowerLines[group].emplace_back(lastX, lastY, firstX, firstY);
		}

		sort(upperLines[0].begin(), upperLines[0].end());
		sort(upperLines[1].begin(), upperLines[1].end());
		sort(lowerLines[0].begin(), lowerLines[0].end());
		sort(lowerLines[1].begin(), lowerLines[1].end());

		double lo = max(upperLines[0].front().xRange.first, upperLines[1].front().xRange.first);
		double hi = min(upperLines[0].back().xRange.second, upperLines[1].back().xRange.second);
		cout << GetMaxHeight(lo, hi) << "\n";
	}
	return 0;
}