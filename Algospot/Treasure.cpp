/*
* Problem: https://algospot.com/judge/problem/read/TREASURE
* 두 도형의 교집합이 되는 다각형을 구한 뒤, 각 선분의 끝점 벡터 사이의 외적 값을 모두 더하고 2로 나눠 그 넓이를 구하려고 했다. (p.538 참고)
* 이를 위해 벡터 구조체를 사용해 평면 위의 위치들을 나타내고, 이차 방정식을 풀어 선분과 선분의 교차점을 알아내는 함수를 구현하였다.
* 이후 직사각형의 각 변을 순회하며 다각형과의 교차점을 찾아내며 교집합 도형을 구하려고 했으나, 정확히 교집합 다각형에 포함되는 점들만을 순서대로 구하는 방법을 찾아내지 못했다.
* 
* 책 해설) 사각형의 각 변을 포함하는 직선으로 평면을 분할한 뒤, 왼쪽(혹은 오른쪽) 반평면에 포함되는 부분을 각각 구한 후 그들의 교집합을 구해 원하는 다각형을 구한다.
* 사각형의 한 변에서 시작하여 입력 다각형에서 오른쪽 반평면에 있는 부분을 삭제한 다각형을 구한다. (이때 변의 순회 순서가 시계방향이냐 반시계방향이냐에 따라 어느쪽 평면을 지울 지가 달라진다)
* 이때 ccw를 통해 다각형의 특정 점이 반평면에 포함되는지 구하면, 해당 점을 끝점으로 하는 선분과 분할 직선의 교차 여부를 쉽게 파악할 수 있다. (포함되는 점과 안 포함되는 점이 이루는 선분은 직선과 무조건 교차됨)
* 이 잘린 사각형을 다시 사각형의 다른 변에 대해 같은 방법으로 자르는 것을, 사각형의 모든 변에 대해 반복하면 원하는 다각형을 구할 수 있다.
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

bool NearlyEqual(double a, double b);

typedef struct Vector2 {
	double x, y;

	Vector2() : x(0), y(0) {}
	explicit Vector2(double inX, double inY) : x(inX), y(inY) {}

	const Vector2 operator+(const Vector2& rhs) const
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}

	const Vector2 operator-(const Vector2& rhs) const
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}

	const Vector2 operator*(double val) const
	{
		return Vector2(x * val, y * val);
	}

	const Vector2 operator/(double val) const
	{
		return Vector2(x / val, y / val);
	}

	bool operator<(const Vector2& rhs) const
	{
		return NearlyEqual(x, rhs.x) ? y < rhs.y : x < rhs.x;
	}

	bool operator>(const Vector2& rhs) const
	{
		return NearlyEqual(x, rhs.x) ? y > rhs.y : x > rhs.x;
	}

	bool operator==(const Vector2& rhs) const
	{
		return NearlyEqual(x, rhs.x) && NearlyEqual(y, rhs.y);
	}

	double Size() const
	{
		return hypot(x, y);
	}

	const Vector2 Normalize() const
	{
		return *this / Size();
	}

	double Dot(const Vector2& rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}

	double Cross(const Vector2& rhs) const
	{
		return x * rhs.y - rhs.x * y;
	}

} Vector2;

const double SMALL_NUM = 1e-8;
int N;
vector<Vector2> poly, rect(4), searchArea;

bool NearlyEqual(double a, double b)
{
	return abs(a - b) < SMALL_NUM;
}

bool FindIntersect(Vector2 p1, Vector2 p2, Vector2 q1, Vector2 q2, Vector2& outIntersect)
{
	if (p1 < p2) swap(p1, p2);
	if (q1 < q2) swap(q1, q2);

	double slopeP = (p2.y - p1.y) / (p2.x - p1.x);
	double constantP = p1.y - slopeP * p1.x;
	double slopeQ = (q2.y - q1.y) / (q2.x - q1.x);
	double constantQ = q1.y - slopeQ * q1.x;

	if (NearlyEqual(slopeP, slopeQ))
		return false;

	Vector2 intersect;
	intersect.x = (constantQ - constantP) / (slopeP - slopeQ);
	intersect.y = slopeP * intersect.x + constantP;
	if (intersect < p1 || p2 < intersect || intersect < q1 || q2 < intersect)
		return false;

	outIntersect = intersect;
	return true;
}

bool IsPointInPoly(const Vector2 point, const vector<Vector2>& p) // 책의 구현을 참고함
{
	int crosses = 0;
	for (int i = 0; i < p.size(); ++i)
	{
		int j = (i + 1) % p.size();
		if ((p[i].y < point.y) != (p[j].y < point.y))
		{
			double slope = (p[i].y - p[j].y) / (p[i].x - p[j].x);
			double c = p[i].y - slope * p[i].x;
			double atX = (point.y - c) / slope;
			if (point.x > atX)
				crosses++;
		}
	}
	return crosses % 2 > 0;
}

void MakeSearchArea()
{
	searchArea.clear();

	//for (int rectPoint1 = 0; rectPoint1 < 4; ++rectPoint1)
	//{
	//	int rectPoint2 = (rectPoint1 + 1) % 4;
	//	if (IsPointInPoly(rect[rectPoint1], poly))
	//		searchArea.push_back(rect[rectPoint1]);

	//	vector<Vector2> intersects;
	//	for (int polyPoint1 = 0; polyPoint1 < N; ++polyPoint1)
	//	{
	//		int polyPoint2 = (polyPoint1 + 1) % N;
	//		Vector2 intersect;
	//		if (FindIntersect(rect[rectPoint1], rect[rectPoint2], poly[polyPoint1], poly[polyPoint2], intersect))
	//			intersects.push_back(intersect);
	//	}

	//	if (rectPoint1 < 2) sort(intersects.begin(), intersects.end(), less<Vector2>());
	//	else sort(intersects.begin(), intersects.end(), greater<Vector2>());
	//	searchArea.insert(searchArea.end(), intersects.begin(), intersects.end());
	//}

	int rectIdx = 0, polyIdx = 0;
	bool rectTurn = true;
	while (true)
	{
		if (rectTurn)
		{
			int rectNextIdx = (rectIdx + 1) % 4;

			vector<pair<Vector2, int>> intersects;
			for (int polyPoint1 = 0; polyPoint1 < N; ++polyPoint1)
			{
				int polyPoint2 = (polyPoint1 + 1) % N;
				Vector2 intersect;
				if (FindIntersect(rect[rectIdx], rect[rectNextIdx], poly[polyPoint1], poly[polyPoint2], intersect))
					intersects.emplace_back(intersect, polyPoint2);
			}

			if (rectIdx < 2)
				sort(intersects.begin(), intersects.end(), less<Vector2>());
			else
				sort(intersects.begin(), intersects.end(), greater<Vector2>());

			for (pair<Vector2, int> e : intersects)
			{
				Vector2& intersect = e.first;
				searchArea.push_back(intersect);
			}

			if (IsPointInPoly(rect[rectNextIdx], poly))
			{
				searchArea.push_back(rect[rectNextIdx]);
			}
			else
			{
				polyIdx = intersects[0].second;
				rectTurn = false;
			}

			rectIdx = rectNextIdx;
			if (rectIdx == 0)
				break;
		}
		else
		{

		}
	}
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> rect[0].x >> rect[0].y >> rect[2].x >> rect[2].y >> N;
		rect[1] = Vector2(rect[0].x, rect[2].y);
		rect[3] = Vector2(rect[2].x, rect[0].y);

		poly.clear();
		poly.resize(N);
		for (int i = 0; i < N; ++i)
			cin >> poly[i].x >> poly[i].y;

		MakeSearchArea();
	}

	return 0;
}