/*
* Problem: https://algospot.com/judge/problem/read/CHILDRENDAY
* 시도) 숫자를 하나씩 붙여나가는 것을 방향 그래프로 표현한 뒤 조건(C % N = M)을 만족하는 수가 나올 때까지 BFS하는 방식을 시도해보았다.
* 두가지 문제가 있었는데,
* 1) 노드의 개수에 제한이 없다는 것
* 2) long long의 범위를 넘어서는 엄청 큰 숫자까지 가기 시작하면 수를 표현할 방법이 없다는 것이었다.
* 
* 그 문제들에 대해 다음의 풀이를 시도했다.
* 1) c에 다음 숫자를 붙인 값은 c * 10 + x이므로, mod 연산의 특징(mod한 것끼리 곱셈/덧셈/뺄셈 mod한 것은 해당 연산을 먼저 하고 mod한 것과 같음)을 활용하면 노드의 개수를 제한할 수 있다 생각했다.
*    하지만 구체적인 방법을 찾지 못했다.
* 2) 각 간선이 뒤에 붙여지는 수를 표현하게 하면 다음의 방식으로 수를 알아낼 수 있다.
*	 최종적으로 찾은 노드에서 시작하여 매 노드에서 자신을 처음으로 발견한 노드로 거슬러 올라가면서 시작 노드까지 이동하고, 그 경로 상의 간선들이 표현하는 숫자를 순서대로 나열한 뒤 뒤집으면 원하던 숫자(정답)가 나온다.
*	 하지만 이러면 BFS 도중에는 현재의 실제 숫자를 알 수 없으므로 현재 숫자가 조건을 만족하는지 알 방법이 없었다.
* 
* 책 해설) 숫자를 붙여가는 과정을 그래프로 표현하는 것은 같으나, 위의 두 풀이의 문제점을 다음의 아이디어를 사용해 해결했다.
* (c * 10 + x) mod N = ((c % N) * 10 + x) mod N이므로, c % N만 기억하고 있으면 c가 조건을 만족하는지 바로 알 수 있다.
* 따라서 각 노드가 현재 숫자 c 대신 c % N을 표현하게 바꾸면 노드의 개수를 N개로 제한할 수 있으며, 각 노드에서 실제 숫자를 몰라도 조건 만족 여부를 알 수 있게 된다.
* 
* 위 방식을 사용해 그래프를 구성하던 중 또다른 문제가 발생했는데, 이는 N + M 미만의 c는 c % N = M이더라도 답이될 수 없다는 것이다.
* 때문에 그런 경우를 걸러낼 방법이 필요한데, 책에선 이를 해결하기 위해 하나의 c % N 값에 대해 실제 숫자가 N 미만인 노드와 N 이상인 노드를 각각 만들었다.
* 이제 BFS는 실제 숫자가 N 이상인 노드에서만 종료하도록 하면 무사히 원하는 답을 구할 수 있다.
*
* 배운점) 기존엔 BFS를 시작하기 전 모든 가능한 숫자를 큐에 직접 넣었는데, 책에 나온 대로 0을 시작점으로 두고 그래프를 만들어나가는 방식을 사용하니
* 시작점을 이미 알고 있어 간선을 거슬러 올라가며 재구성하는 것이 편리해졌다. (재구성 종료 시점을 바로 알 수 있음)
*/
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <memory.h>

using namespace std;

int N, M;
pair<int, int> parent[10000][2];
vector<int> digits;

string Reconstruct(int c)
{
	string ret;
	int p = parent[c][1].first;
	int pOverN = parent[c][1].second;
	int cOverN = 1;

	while (c != 0 || cOverN == 1)
	{
		for (int digit : digits)
		{
			int candi = (p * 10 + digit) % N;
			if (candi == c)
			{
				ret.push_back(digit + '0');
				break;
			}
		}
		c = p;
		cOverN = pOverN;
		p = parent[c][cOverN].first;
		pOverN = parent[c][cOverN].second;
	}

	reverse(ret.begin(), ret.end());
	return ret;
}

int BFS()
{
	queue<pair<int, int>> q;
	q.emplace(0, 0);
	parent[0][0] = make_pair(0, 0);

	while (!q.empty())
	{
		int c = q.front().first;
		int cOverN = q.front().second;
		q.pop();

		if (cOverN != 0 && c == M)
			return c;

		for (int digit : digits)
		{
			int candi = c * 10 + digit;
			int idx = cOverN ? 1 : (candi >= N ? 1 : 0);
			candi %= N;

			if (parent[candi][idx].first == -1)
			{
				parent[candi][idx].first = c;
				parent[candi][idx].second = cOverN;
				q.emplace(candi, idx);
			}
		}
	}
	return -1;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int T;
	cin >> T;
	while (T--)
	{
		string D;
		cin >> D >> N >> M;

		memset(parent, -1, sizeof(parent));
		digits.clear();
		for (char d : D) digits.push_back(d - '0');
		sort(digits.begin(), digits.end());

		int c = BFS();
		if (c == -1) cout << "IMPOSSIBLE" << "\n";
		else cout << Reconstruct(c) << "\n";
	}
	return 0;
}