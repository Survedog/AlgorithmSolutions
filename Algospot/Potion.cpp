/*
* Problem: https://algospot.com/judge/problem/read/POTION
* 풀이1 (이전 커밋 참고): 모든 재료들은 '현재까지 넣은 양/레시피에서의 양' 비율에 대해 '가장 큰 비율 * 레시피에서의 양' 만큼은 넣어야 한다.
* 따라서 해당 비율이 가장 큰 재료를 찾은 후, 다른 재료들에도 그 비율을 곱해 모두 정수가 되는지 확인했다. 이때 하나라도 정수가 되지 않는다면, 방금의 재료를 1개씩 추가하며 다시 비율을 구해 확인했다.
* 만약 모든 재료가 정수가 되는 비율을 찾았다면, 해당 비율을 모든 재료의 레시피의 양에 곱하여 최종적으로 모든 재료가 얼마만큼 필요한지 계산할 수 있었다.
* 이때 현재 들어간 모든 재료의 양이 레시피의 양보다 적다면, 위 로직을 수행하지 않고 딱 레시피 만큼만 넣도록 했다.
*
* 풀이2 (현재 소스코드/책 풀이를 참고함):  현재까지 넣은 양에 재료들을 추가하여 레시피에서의 비율을 맞췄다 했을 때, 사용된 전체 재료들의 최소 양 = 레시피의 재료 양 * Y로 표현할 수 있다.
* 이때 유리수인 Y는 X = '현재까지 넣은 양/레시피에서의 양'보다 크거나 같을 수 밖에 없다. (즉, Y >= X).
* Y = a/b (a, b는 정수)로 나타내면, 사용된 전체 재료들의 양(=레시피의 재료 양 * a / b)의 각 원소가 모두 정수가 되야 하므로 b는 레시피의 모든 재료들의 양의 최대 공약수 gcd가 될 수 있다.
* (일반 공약수로도 모두 정수가 되도록 나눌 순 있지만, 전체 재료들의 최소 양을 구하기 위해 최대 공약수로 나눈다)
* 따라서 Y >= X는 a >= X * b = X * gcd로 나타낼 수 있으니, X를 구하고 유클리드 알고리즘으로 공약수 gcd를 구하면 해당 조건을 만족하는 최소 a를 알 수 있고, Y를 구할 수 있다.
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

int n, r[200], p[200];

int GetGCD(int a, int b)
{
	if (b == 0) return a;
	return GetGCD(b, a % b);
}

int GetGCD(vector<int> nums)
{
	int gcd = GetGCD(nums[0], nums[1]);
	for (int i = 2; i < nums.size(); ++i)
		gcd = GetGCD(gcd, nums[i]);
	return gcd;
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
			cin >> r[i];
		for (int i = 0; i < n; ++i)
			cin >> p[i];

		double maxRatio = -1;
		for (int i = 0; i < n; ++i)
			maxRatio = max(maxRatio, (double)p[i] / r[i]);

		int gcd = GetGCD(vector<int>(r, &r[n]));
		double Y = ceil(gcd * maxRatio) / gcd;

		for (int i = 0; i < n; ++i)
			cout << int(Y * r[i] - p[i]) << " ";
		cout << "\n";
	}

	return 0;
}