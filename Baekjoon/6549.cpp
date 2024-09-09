/*
* Problem: https://www.acmicpc.net/problem/6549
* 분류: 분할 정복
* 
* 직사각형을 절반으로 나눠 왼쪽/오른쪽/가운데를 포함하는 위치 각각에서의 최대 직사각형 크기를 비교했다.
* 이때 가운데를 포함하는 위치의 최대 직사각형은 다음의 원리를 사용해 구했다.
* - 직사각형의 왼쪽과 오른쪽 막대 중 하나를 포함해야 할 때, 더 큰 막대를 포함하면 무조건 더 큰 직사각형을 만들 수 있다.
*/

#include <iostream>
#include <algorithm>

using namespace std;

int N, H[100000];

long long GetMaxSize(int left, int right)
{
	if (left == right) return H[left];

	int mid = (left + right) / 2;
	long long ret = H[mid];
	int lo = mid, hi = mid, height = H[mid];

	while (lo > left || hi < right)
	{
		if (lo == left || (hi != right && H[lo - 1] < H[hi + 1]))
		{
			hi++;
			height = min(height, H[hi]);
		}
		else
		{
			lo--;
			height = min(height, H[lo]);
		}
		ret = max(ret, (long long)(hi - lo + 1) * height);
	}

	ret = max(ret, GetMaxSize(left, mid));
	ret = max(ret, GetMaxSize(mid + 1, right));
	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	while (true)
	{
		cin >> N;
		if (N == 0) break;

		for (int i = 0; i < N; ++i)
			cin >> H[i];

		cout << GetMaxSize(0, N - 1) << "\n";
	}

	return 0;
}