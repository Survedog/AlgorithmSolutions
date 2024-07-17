/*
* Problem: https://algospot.com/judge/problem/read/DRAGON
* 부분 문자열을 얻는 문제에서 특정 위치의 문자 하나를 얻는 문제로 단순화시킨다. 이 문제를 l번 반복하면 부분 문자열을 얻을 수 있다.
* 알고리즘은 재귀 호출 횟수 n * 함수 내 최대 반복 수 5로 O(n)의 시간 복잡도를 가지기에 l번 반복해도 문제가 없다.
* 재귀할 때 함수에 string 전체를 전달하지 않고, X에선 "X+YF", Y에선 "FX-Y"만 전달한다.
* 이 값만 알고 있어도 특정 문자를 생략할 지, 혹은 타고 내려갈 지를 결정할 수 있고, p번째 문자를 얻을 수 있기 때문이다.
* 
* 세대에 따라 확장되는 길이를 구해 skip 값에서 빼는 아이디어까진 구상했으나, string을 넘기는 것을 고려하지 않아 실제 p번째 문자를 알아낼 방법을 찾지 못함.
* 재귀 함수에 필요한 데이터만을 골라내는 센스가 필요해보임.
*/

#include <iostream>
#include <algorithm>
#include <memory.h>
#include <cmath>
#include <string>

using namespace std;

const int MAX = 1000000000 + 1;
const string EXPAND_X = "X+YF";
const string EXPAND_Y = "FX-Y";

int n, p, l, length[51];

int GetGenLength(int gen)
{
	if (gen == 0)
		return 1;

	int& ret = length[gen];
	if (ret != -1) return ret;

	ret = min(MAX, 2 * GetGenLength(gen - 1) + 2);
	return ret;
}

char Curve(const string& curve, int gen, int skip)
{
	if (gen == 0)
	{
		return curve[skip];
	}

	for (int i = 0; i < curve.size(); ++i)
	{
		if (curve[i] == 'X' || curve[i] == 'Y')
		{
			if (skip >= GetGenLength(gen))
				skip -= GetGenLength(gen);
			else if (curve[i] == 'X')
				return Curve(EXPAND_X, gen - 1, skip);
			else if (curve[i] == 'Y')
				return Curve(EXPAND_Y, gen - 1, skip);
		}
		else if (skip > 0)
			skip--;
		else
			return curve[i];
	}
	return '#';
}

int main()
{
	int C;
	cin >> C;

	while (C--)
	{
		cin >> n >> p >> l;
		memset(length, -1, sizeof(length));

		const string curve = "FX";
		for (int i = 0; i < l; ++i)
		{
			cout << Curve(curve, n, p - 1 + i);
		}
		cout << "\n";
	}
	return 0;
}