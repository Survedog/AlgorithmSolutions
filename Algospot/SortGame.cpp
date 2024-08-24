/*
* Problem: https://algospot.com/judge/problem/read/SORTGAME
* 시도) 8개의 숫자를 나열할 수 있는 가짓수는 40320개이므로, 각각의 경우를 노드로 만들고 한 번의 뒤집기 연산으로 서로로 변할 수 있는 노드들끼리 간선으로 이어 그래프로 만든다.
* 그 후 정렬된 배열로부터 너비 우선 탐색을 적용해 입력된 배열이 나올 때까지 계속하면 그 배열을 정렬하기 위한 최소 뒤집기 횟수를 구할 수 있다.
* 이때 이미 지나간 배열(노드)를 파악하는 방법을 다음과 같이 시도해보았다.
* 1) 각 숫자에 자신의 순서의 팩토리얼을 곱한 것을 모두 더해 고유한 인덱스를 얻어낸다. 이때 오버플로우를 막기 위해 입력된 배열의 원소들을 크기가 작은 순으로 0~n-1의 값으로 재할당한다. (상대적 순서만 같으면 답은 같으므로 상관없음)
* -> 이 방법으론 고유한 인덱스가 얻어지지 않아 실패.
* 2) set을 사용해 지금까지 나온 배열들을 저장한다. -> set이 커질수록 필요한 시간이 늘어나 결국 시간 초과가 발생한다.
* 
* 책 해설) 1~8까지의 크기의 모든 배열(이때 원소는 0 ~ n-1)에 대해 미리 너비우선탐색을 적용해 모든 나열에 대한 답을 구해낸다.
* 그 후 모든 테스트케이스에서 미리 구해둔 값을 그대로 사용만 하면 된다.
*/
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <map>

using namespace std;

const int NODE_COUNT = 16777216;

int N;
map<vector<int>, int> dist[8];
vector<int> target;
queue<pair<vector<int>, int>> q;

int BFS(int n)
{
	vector<int> here(n, -1);
	for (int i = 0; i < n; ++i) here[i] = i;

	while (!q.empty())
		q.pop();

	dist[n - 1][here] = 0;
	q.emplace(here, 0);

	while (!q.empty())
	{
		here = q.front().first;
		int distance = q.front().second;
		q.pop();

		for (int i = 0; i < n; ++i)
			for (int j = i + 2; j <= n; ++j)
			{
				reverse(here.begin() + i, here.begin() + j);
				if (dist[n - 1].count(here) == 0)
				{
					dist[n - 1][here] = distance + 1;
					q.emplace(here, distance + 1);
				}
				reverse(here.begin() + i, here.begin() + j);
			}
	}
	return -1;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	for (int n = 1; n <= 8; ++n)
		BFS(n);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> N;

		target.clear();
		vector<pair<int, int>> order;
		for (int i = 0; i < N; ++i)
		{
			int num;
			cin >> num;
			target.push_back(num);
			order.emplace_back(num, i);
		}

		sort(order.begin(), order.end());
		for (int i = 0; i < N; ++i)
			target[order[i].second] = i;

		cout << dist[N - 1][target] << "\n";
	}
	return 0;
}