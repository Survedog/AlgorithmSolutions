/*
* Problem: https://algospot.com/judge/problem/read/WITHDRAWAL
* 처음엔 과목들을 배열에 넣고, subject번부터 마지막 과목까지 중에서 cancel개 과목을 취소할 수 있을 때 subject부터 끝까지의 최소 CumulativeRank를 구하는 함수를 통해 동적계획법을 사용해 풀려고 했다.
* 하지만 현재 과목을 취소하는 것과 유지하는 것 중 무엇이 cumulativeRank를 더 낮추는지 알기 위해선 이전까지의 cumulativeRank가 필요하기 때문에 메모이제이션을 하기엔 경우의 수가 너무 많았다.
* ex) 1/1인 과목을 유지 후 2/3, 4/6 중 하나를 유지해야 한다면 4/6을 골라야 cumulativeRank가 낮아지나, 만약 1/10을 고른 후라면 2/3을 고르는 게 더 낮아진다.
*
* 교재 풀이) 최적화 문제를 결정 문제로 변환하여 푸는 방식을 이용한다.
* 과목들의 부분집합 중 크기가 k이상인 S에 대해, cumulativeRank = sum(r_j) / sum(c_j) <= x (j는 S에 속하는 원소)인지 확인하는 Decision(x) 함수를 구현한 뒤 이분법을 사용하면 된다.
* 이때 식을 변형하면 0 <= sum(x * c_j - r_j)가 되는데, v_j = x * c_j - r_j라 하면 모든 과목에 대한 v_j를 구해 내림차순으로 정렬하고, 거기서 가장 큰 k개 원소들을 더한 값이 0 이상인지 확인함으로써
* 해당 식이 성립하는 부분집합이 존재하는지 (즉, cumulativeRank가 x이상인 부분 집합이 존재하는지) 확인할 수 있다.
*
* 개선할 점) 해당 문제를 결정 문제로 바꿨을 때, 그 문제를 어떻게 풀어야할 지 생각해내지 못했음.
* 다음에 이와 같은 방식을 사용한다면 위처럼 조건을 수식으로 전개하고 변형해보며 조건이 성립하는지 확인하는 방법을 구상해보는 것이 좋을 듯.
*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int n, k, r[1000], c[1000];

// cumulativeRank가 x 이하가 되도록 과목들을 취소할 수 있는가?
bool Decision(double x)
{
	vector<double> v(n, 0);
	for (int i = 0; i < n; ++i)
		v[i] = x * c[i] - r[i];
	sort(v.begin(), v.end(), greater<double>());

	double vSum = 0;
	for (int i = 0; i < k; ++i)
		vSum += v[i];

	return vSum >= 0;
}

double Optimize(int iter)
{
	double lo = -1e-8, hi = 1.0;
	while (iter--)
	{
		double mid = (lo + hi) / 2;
		if (Decision(mid))
			hi = mid;
		else
			lo = mid;
	}
	return hi;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.precision(8);
	cout << fixed;

	int T;
	cin >> T;
	while (T--)
	{
		cin >> n >> k;
		for (int i = 0; i < n; ++i)
			cin >> r[i] >> c[i];

		cout << Optimize(100) << "\n";
	}

	return 0;
}