/*
* Problem: https://algospot.com/judge/problem/read/CANADATRIP
* 이 문제는 'K번째 표지판을 만나거나 지나치기 위한 최소 거리를 구하라'라는 최적화 문제로도 해석할 수 있기 때문에, 최적화 문제를 결정 문제로 바꾸는 방법을 응용하여 풀 수 있었다. 
* 도시들을 표지판 시작 위치에 따라 정렬해 벡터 sorted에 저장하고, 이것을 순회하며 표지판의 개수를 세어 K번째 표지판이 특정 위치 이후에 있는 지를 확인하는 함수 Decision을 구현했다.
* 그 후 BinarySearch 함수에서 Decision을 사용한 이분법을 통해 K번째 표지판을 만날 수 있는 최소의 위치를 계산하였다.
* 해당 알고리즘의 시간 복잡도는 BinarySearch의 시간 복잡도가 O(log(8030000)) = O(1)이고, Decision은 O(N)이므로 O(N)이다.
* 
* 개선할 점: sorted를 테스트 케이스마다 초기화 하는 것을 까먹어 오답이 발생했다. 변수를 초기화해야 하는 지를 더욱 세심히 확인해야할 것 같다.
* 생각해볼 점: 이 알고리즘은 Decision이 오답을 반환할 수 없으니 상관 없지만, 실수 계산 오차 등으로 Decision이 잘못된 답을 반환할 여지가 있는 문제에선 이분법 시 lo, hi의 타입을 실수로 변경하고 많은 iteration을 수행해야 한다.
* - lo, hi가 실수이면 Decision이 가끔 오답을 반환하더라도 iteration을 충분히 반복하면 최종적으론 해답의 근사치가 나오기 때문.
*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int STREET_LEN = 8030000;
int N, K, L[5000], M[5000], G[5000], signCount[5000];
vector<pair<int, int>> sorted;

bool Decision(int dist) // K번째 표지판이 도로상에서 dist 이후에 있는가. 즉, (dist, end]에 있는가
{
	int signs = 0;
	for (int i = 0; i < N && signs < K; ++i)
	{
		int signStart = sorted[i].first;
		if (signStart > dist) break;

		int signIdx = sorted[i].second;
		if (L[signIdx] <= dist)
			signs += signCount[signIdx];
		else
			signs += (dist - signStart) / G[signIdx] + 1;
	}

	return signs < K;
}

int BinarySearch()
{
	int lo = 0; int hi = STREET_LEN;
	while (lo < hi)
	{
		int mid = (lo + hi) / 2;
		if (Decision(mid))
			lo = mid + 1;
		else
			hi = mid;
	}
	return lo;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int T;
	cin >> T;
	while (T--)
	{
		cin >> N >> K;
		sorted.clear();
		for (int i = 0; i < N; ++i)
		{
			cin >> L[i] >> M[i] >> G[i];
			signCount[i] = M[i] / G[i] + 1;
			sorted.emplace_back(L[i] - M[i], i);
		}
		sort(sorted.begin(), sorted.end());

		cout << BinarySearch() << "\n";
	}

	return 0;
}