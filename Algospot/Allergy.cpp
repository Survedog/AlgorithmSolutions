/*
* Problem: https://algospot.com/judge/problem/read/ALLERGY
* 가지치기를 사용한 조합탐색 기법으로 해결하였다. 해를 하나 구해 저장한 후 현재까지 만든 음식이 해당 해보다 많거나 같으면 해당 분기를 바로 종료하는 간단한 방식의 가지치기를 사용하였음.
* 이때 long long형 정수에 비트마스크를 통해 각 사람이 먹을 수 있는 음식을 만들었는지 상태를 저장함으로써, 모든 사람이 음식을 먹을 수 있는지를 한 번의 값 비교만으로 확인할 수 있었다.
* 사람이 최대 50명 올 수 있으므로, 상태를 64비트인 long long형에 저장해야 하는데 이를 int형에 저장하여 오답이 발생했다. 비트 마스크 사용 시 항상 오버플로우에 주의할 것.
*/

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

int n, m, best;
vector<int> eater[50];

void CalcBest(int food, long long eatState, int made)
{
	if (eatState == ((long long)1 << n) - 1)
	{
		best = min(best, made);
		return;
	}
	if (food == m)
		return;
	if (made >= best)
		return;

	long long nextEatState = eatState;
	for (int person : eater[food])
		nextEatState |= ((long long)1 << person);
	CalcBest(food + 1, nextEatState, made + 1);
	CalcBest(food + 1, eatState, made);
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int T;
	cin >> T;
	while (T--)
	{
		cin >> n >> m;
		map<string, int> nameIdxMap;
		for (int i = 0; i < n; ++i)
		{
			string name;
			cin >> name;
			nameIdxMap[name] = i;
		}

		for (int food = 0; food < m; ++food)
		{
			eater[food].clear();

			int eaterCount;
			cin >> eaterCount;
			for (int i = 0; i < eaterCount; ++i)
			{
				string name;
				cin >> name;
				eater[food].push_back(nameIdxMap[name]);
			}
		}

		best = 50;
		CalcBest(0, 0, 0);
		cout << best << "\n";
	}
}