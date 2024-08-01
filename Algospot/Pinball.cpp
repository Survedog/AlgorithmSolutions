/*
* Problem: https://algospot.com/judge/problem/read/POTION
* 직접 벡터 구조체와 연산 함수를 구현해 풀었다.
* 이동하는 원(공)과 원(장애물)의 교차점을 구하는 것이 복잡하기 때문에, 각 장애물의 반지름을 1씩 올리는 대신 이동하는 공을 직선으로 간주하기로 했다. (공의 반지름이 1이므로)
* 이후 내적을 이용하여 교차점을 알아내는데 성공했고, 예제는 통과했으나 실제 채점에선 오답이 발생했다.
*/
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

bool NearlyEqual(double a, double b);

typedef struct Vector2 {
	double x, y;

	Vector2() : x(0), y(0) {}
	explicit Vector2(double inX, double inY) : x(inX), y(inY) {}

	Vector2 operator+(const Vector2& rhs) const
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}

	Vector2 operator-(const Vector2& rhs) const
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}

	Vector2 operator*(double val) const
	{
		return Vector2(x * val, y * val);
	}

	Vector2 operator/(double val) const
	{
		return Vector2(x / val, y / val);
	}

	bool operator==(const Vector2& rhs) const
	{
		return NearlyEqual(x, rhs.x) && NearlyEqual(y, rhs.y);
	}

	double Size() const
	{
		return hypot(x, y);
	}

	Vector2 Normalize() const
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

const double SMALL_NUM = 1e-8, INF = 987654321;
int N, obsR[50];
Vector2 obstacles[50];

bool NearlyEqual(double a, double b)
{
	return abs(a - b) < SMALL_NUM;
}

bool Intersect(int obs, Vector2 ball, Vector2 direction, Vector2& outIntersectPoint)
{
	Vector2 obstacle = obstacles[obs];
	Vector2 toCenter = obstacle - ball;
	direction = direction.Normalize();

	double ballToPointLen = toCenter.Dot(direction);
	Vector2 point = ball + direction * ballToPointLen;

	Vector2 centerToPoint = point - obstacle;
	double centerToPointLen = centerToPoint.Size();
	if (NearlyEqual(centerToPointLen, obsR[obs]))
	{
		outIntersectPoint = point;
		return true;
	}
	else if (centerToPointLen < obsR[obs])
	{
		double interToPointLen = NearlyEqual(centerToPointLen, 0) ? obsR[obs] : sqrt(obsR[obs] * obsR[obs] - centerToPointLen * centerToPointLen);
		outIntersectPoint = ball + direction * (ballToPointLen - interToPointLen);
		return true;
	}
	else if (centerToPointLen > obsR[obs])
		return false;
}

int Roll(Vector2 start, Vector2 direction, Vector2& outEnd, Vector2& outNewDirection, int lastObstacle)
{
	int firstObstacle = -1;
	double distToFirst = INF;
	for (int obs = 0; obs < N; ++obs)
	{
		if (obs == lastObstacle)
			continue;

		const Vector2& obstacle = obstacles[obs];
		Vector2 intersectPoint;
		if (Intersect(obs, start, direction, intersectPoint))
		{
			bool isWrongDirection = direction.Dot(intersectPoint - start) < 0;
			if (!isWrongDirection)
			{
				double distToIntersect = (outEnd - start).Size();
				if (distToIntersect < distToFirst)
				{
					outEnd = intersectPoint;
					distToFirst = distToIntersect;
					firstObstacle = obs;
				}
			}
		}
	}

	if (firstObstacle >= 0)
	{
		const Vector2& obstacle = obstacles[firstObstacle];
		Vector2 normalizedCenterToInter = (outEnd - obstacle).Normalize();
		double centerToFootLen = (start - obstacle).Dot(normalizedCenterToInter);
		Vector2 foot = obstacle + (outEnd - obstacle).Normalize() * centerToFootLen;
		Vector2 newDirectionPoint = start + (foot - start) * 2;
		outNewDirection = (newDirectionPoint - outEnd).Normalize();
	}
	return firstObstacle;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		int x, y, dx, dy;
		cin >> x >> y >> dx >> dy >> N;

		for (int i = 0; i < N; ++i)
		{
			cin >> obstacles[i].x >> obstacles[i].y >> obsR[i];
			obsR[i]++;
		}

		Vector2 start(x, y), direction(x, y);
		int lastObstacle = -1;
		for (int iter = 0; iter < 100; ++iter)
		{
			lastObstacle = Roll(start, direction, start, direction, lastObstacle);
			if (lastObstacle != -1)
				cout << lastObstacle << " ";
			else
				break;
		}
	}
	return 0;
}