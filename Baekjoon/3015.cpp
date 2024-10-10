/*
* Problem: https://www.acmicpc.net/problem/3015
* 분류: 스택, 모노토닉 스택 (단조 증가/감소 스택)
*
* 참고한 게시물: https://blog.thecloer.com/121
* 줄에 사람이 추가될 때마다 그 앞에 있는 사람들 중 보이는 사람을 세는 방식을 사용한다.
* 현재 추가된 사람을 x, 바로 앞 사람을 t라고 할 때, x의 앞에 있는 사람 중 t보다 키가 더 크거나 같으면서 그 자신보다 더 큰 사람에 의해 가려지지 않는 사람들의 키들을 스택에 저장하자.
* 그 스택에서 x보다 큰 사람을 만날 때까지 원소들을 뺀다 하면, x는 처음으로 만나는 자신보다 더 큰 사람과 그 이전까지 스택에서 빼온 모든 사람들과 마주 볼 수 있다.
* 이제 스택에서 뺀 사람 중 x와 키가 같은 사람들을 스택에 다시 넣어주고, x를 스택에 넣은 뒤, 다음 사람을 추가하여 이를 반복하면 모든 마주볼 수 있는 사람의 수를 알 수 있다.

* 이 알고리즘에서 최악의 상황은 모든 사람의 키가 같은 경우로, 이때의 시간 복잡도는 O(N^2)이다.
* 스택에 개별 인원의 키 대신 키와 그 키를 가진 사람 수의 쌍을 저장하면 시간을 최적화할 수 있다.
* 이때 스택의 크기는 각 원소를 추가할 때마다 최대 1만큼만 증가할 수 있으므로 시간 복잡도는 O(N)이다.
*/
#include <iostream>
#include <vector>
#include <stack>

using namespace std;

int N;
vector<int> heights;

long long GetAnswer()
{
	long long ret = 0;
	stack<pair<int, int>> st;

	for (int i = 0; i < N; ++i)
	{
		int sameCnt = 1;
		while (!st.empty() && st.top().first <= heights[i])
		{
			ret += st.top().second;
			if (st.top().first == heights[i]) sameCnt += st.top().second;
			st.pop();
		}

		if (!st.empty()) ret++;
		st.emplace(heights[i], sameCnt);
	}

	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N;
	heights.resize(N);
	for (int i = 0; i < N; ++i)
		cin >> heights[i];

	cout << GetAnswer() << "\n";

	return 0;
}