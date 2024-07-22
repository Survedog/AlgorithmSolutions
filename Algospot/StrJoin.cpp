/*
* Problem: https://algospot.com/judge/problem/read/STRJOIN
* 최적해를 구하기 위해 작은 문자열부터 더해나가는 방법을 생각해냈다. 이것이 최적해를 만들 수 있다는 것을 증명하고자 함.
* 문자열 a, b, c을 합친다고 할 때, (a + b) + c의 순서로 합친다면 반복문의 수행 횟수는 2a + 2b + c이다.
* 탐욕적 선택 속성 증명(귀류법): 만약 위 문자열들의 길이가 a > c라고 하면, 2a + 2b + c는 a + (b + c)의 반복 횟수 a + 2b + 2c보다 크고, b > c라 하면 (a + c) + b의 반복 횟수 2a + b + 2c보다 크다.
* - 따라서 더 작은 문자열을 먼저 합치면 무조건 반복 횟수를 줄일 수 있다.
* 최적 부분 구조 증명: 매 합치기 연산의 반복 횟수를 최소화하지 않으면서 전체 반복 횟수를 줄이는 방법은 없으므로 모든 단계에서 전체 반복 횟수를 최소화 해야한다.
* - 따라서 최적 부분 구조가 성립한다.
* 위 증명들로 인해 모든 문자열을 작은 것부터 합쳐 나가면 반복문을 최소한으로 수행하며 문자열을 모두 합칠 수 있다는 것을 알 수 있다.
*/

#include <iostream>
#include <queue>

using namespace std;

int n;
priority_queue<int, vector<int>, greater<int>> pq;

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> n;
		pq = priority_queue<int, vector<int>, greater<int>>();
		for (int i = 0; i < n; ++i)
		{
			int len;
			cin >> len;
			pq.push(len);
		}

		int ret = 0;
		while (pq.size() > 1)
		{
			int a = pq.top(); pq.pop();
			int b = pq.top(); pq.pop();
			pq.push(a + b);
			ret += a + b;
		}

		cout << ret << "\n";
	}
	return 0;
}