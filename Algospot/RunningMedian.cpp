/*
* Problem: https://algospot.com/judge/problem/read/RUNNINGMEDIAN
* 숫자들을 크기 순서대로 늘어놓고 중간값을 구한 뒤, 현재 중간 값보다 작은 값과 큰 값의 개수를 알면 다음 값이 들어왔을 때 중간 값이 한 칸 오른쪽으로 이동할지, 왼쪽으로 이동할 지 알 수 있다.
* 하지만 매번 값이 들어올 때마다 숫자들을 정렬하기엔 N이 너무 크다. 또한 중간값은 숫자가 들어올 때마다 좌 혹은 우로 한 칸씩만 이동하므로 작은 수들 중 가장 큰 수와 큰 수들 중 가장 작은 수만 알면 된다.
* 이에 적합한 자료구조는 힙을 사용한 우선순위 큐라고 생각해, 들어오는 숫자들을 절반으로 갈라 작은 절반은 최대 힙에, 큰 절반은 최소 힙에 넣었다.
* 이제 힙의 크기를 비교하기만 하면 현재 중간 값을 바로 구할 수 있다.(크기가 다르면 중간에 있는 수, 크기가 같으면 최대 힙에 있는 수이다) 또한 새 값이 들어올 경우 O(logN)의 시간만에 정렬할 수 있다.
* 이를 사용해 숫자가 추가될 때마다 중간값을 구해나가면 O(NlogN)의 시간만에 중간값의 합을 구할 수 있다.
* 
* 이때 경우의 수를 줄이기 위해 최소 힙(큰 절반)의 크기는 반드시 최대 힙(작은 절반)과 같거나 1만큼 더 크도록 고정했다.
*/
#include <iostream>
#include <queue>

using namespace std;

int N, a, b;
const int DIVIDER = 20090711;
priority_queue<int, vector<int>, greater<int>> bigger;
priority_queue<int, vector<int>, less<int>> smaller;

typedef struct Generator
{
	int seed, a, b;

	Generator(int seed, int a, int b) : seed(seed), a(a), b(b) {}

	int GetNext()
	{
		return seed = ((long long)seed * a + b) % DIVIDER;
	}
};

int FindMedianAdding(int num)
{
	if (bigger.empty() && smaller.empty())
	{
		bigger.push(num);
		return num;
	}

	if (bigger.size() == smaller.size())
	{
		if (num >= smaller.top())
			bigger.push(num);
		else
		{
			smaller.push(num);
			bigger.push(smaller.top());
			smaller.pop();
		}
		return bigger.top();
	}
	else if (bigger.size() > smaller.size())
	{
		if (num >= bigger.top())
		{
			bigger.push(num);
			smaller.push(bigger.top());
			bigger.pop();
		}
		else
			smaller.push(num);
		return smaller.top();
	}
}

int GetMedianSum()
{
	Generator gen(1983, a, b);
	int num = 1983, sum = 0;
	for (int i = 0; i < N; ++i)
	{
		sum = (sum + FindMedianAdding(num)) % DIVIDER;
		num = gen.GetNext();
	}
	return sum;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		while (!bigger.empty()) bigger.pop();
		while (!smaller.empty()) smaller.pop();
		cin >> N >> a >> b;
		cout << GetMedianSum() << "\n";
	}
	return 0;
}