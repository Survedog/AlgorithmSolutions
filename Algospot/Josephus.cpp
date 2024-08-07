/*
* Problem: https://algospot.com/judge/problem/read/JOSEPHUS
* 연결리스트는 어디에서 원소를 삭제하든 상수 시간이 걸리는 것을 이용해, 연결 리스트에 사람들을 담고 K번째 사람을 지워가는 방법으로 마지막으로 남은 2명을 구했다.
*/
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

int N, K;
list<int> circle;

void KillUntil2Left()
{
	int manLeft = N;
	while (manLeft > 2)
	{
		circle.pop_front();
		manLeft--;

		for (int j = 0; j < K - 1; ++j)
		{
			circle.push_back(circle.front());
			circle.pop_front();
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
		cin >> N >> K;
		circle.clear();
		for (int i = 1; i <= N; ++i)
			circle.push_back(i);

		KillUntil2Left();
		circle.sort();
		for (int man : circle)
			cout << man << " ";
		cout << "\n";
	}

	return 0;
}