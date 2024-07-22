/*
* Problem: https://algospot.com/judge/problem/read/LUNCHBOX
* 먹는 시간이 긴 도시락부터 데우면 최적해가 나오지 않을까?
* 귀류법: 먹는 시간이 짧은 도시락 A와 먹는 시간이 긴 도시락 B가 있을 때, A 다음 B를 데울 때 식사를 가장 빨리 끝내는 경우(최적해)가 있다고 가정.
* - 모든 도시락을 연속해서 데우므로 어떤 순서로 데우든 모든 도시락이 완전히 데워지는 시각은 동일하다.
* - 따라서 두 도시락을 모두 먹는 시간은 식1: max(A를 데우는 시간 + B를 데우는 시간 + B를 먹는 시간, A를 데우는 시간 + A를 먹는 시간)이다.
* - 이때 A와 B의 순서를 바꾼다면, 두 도시락을 모두 먹는 시간은 식2: max(A를 데우는 시간 + B를 데우는 시간 + A를 먹는 시간, B를 데우는 시간 + B를 먹는 시간)이 된다.
* - 이때 식1의 좌항은 식2의 좌항과 우항보다 무조건 크거나 같다. 따라서 식1의 결과는 식2보다 무조건 크거나 같다고 할 수 있다.
* - 따라서 BA 순서로 먹을 때 식사가 끝나는 시간이 같거나 더 빨라지므로, 먹는 시간이 긴 도시락부터 데우는 게 최적해가 된다는 것을 증명한다.
*/

#include <iostream>
#include <algorithm>

using namespace std;

int n, m[10000], e[10000];
pair<int, int> order[10000];

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
			cin >> m[i];
		for (int i = 0; i < n; ++i)
		{
			cin >> e[i];
			order[i] = make_pair(e[i], i);
		}
		sort(&order[0], &order[n], greater<pair<int, int>>());

		int ret = 0, time = 0;
		for (int i = 0; i < n; ++i)
		{
			int lunch = order[i].second;
			time += m[lunch];
			ret = max(ret, time + e[lunch]);
		}

		cout << ret << "\n";
	}
}