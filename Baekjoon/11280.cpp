/*
* Problem: https://www.acmicpc.net/problem/11280
* 분류: 그래프, DFS, SCC 구하기 (타잔 알고리즘)
*
* Algospot/MeetingRoom과 거의 같은 방식으로 해결함.
* 각 변수마다 참/거짓임을 나타내는 노드를 하나씩 만들고, 긱 절마다 그 내용이 참이 되기 위한 변수 간 관계를 간선으로 만들어 함의 그래프를 만들었다.
* ex) 절이 (x or y) 라면 x가 false일때 y는 반드시 true여야 한다. 이 경우 x의 거짓 노드에서 y의 참 노드로 간선을 잇는다.
* 이 그래프에서 각 노드의 내용이 맞는지 아닌지를 논리값 T/F로 나타낸다면, 그래프에 T인 노드에서 F인 노드로 이어지는 간선이 없다면 전체 식을 True로 만들기 위한 조합을 찾을 수 있다.
* 그래프를 SCC(강결합 컴포넌트)로 나눴을 때, 한 SCC 안의 노드들은 전부 T거나 전부 F여야 한다. (그렇지 않으면 T->F 간선이 발생함)
* 또한 한 변수의 참/거짓 노드는 서로 반대의 값을 가져야 한다는 것은 자명하다. (참 노드가 T면 거짓 노드가 F)
* 따라서 타잔 알고리즘을 써서 SCC를 분리한 후, 한 변수의 참/거짓 노드가 모두 들어있는 SCC가 있는지 확인하면 된다.
* 만약 그러한 SCC가 없다면 전체 그래프 내에 T->F 간선이 없다는 것을 귀류법을 통해 증명할 수 있다.
*
* 증명) 다음의 알고리즘으로 각 SCC마다 올바른 논리값(T/F)을 구한다고 하자.
* - 입력 간선이 없는 SCC부터 조회하며, 만약 논리 값이 정해지지 않았다면 F로 정하고, 반대 노드를 T로 정한 뒤 노드를 그래프에서 지운다.
*   (그래프를 SCC로 분리하면 DAG가 되므로 입력 간선이 없는 SCC가 반드시 존재함)
*   만약 논리 값이 이미 T로 정해져 있다면 그냥 지운다. (방문하지 않은 노드 중 논리 값이 이미 정해진 것은 T밖에 없음)
*   이 방법을 반복하면서 그래프의 모든 노드를 지우면 전체 식이 참이 되는 논리 값 조합을 찾을 수 있다.
*
* 위 알고리즘에 따르면 한 변수에 대해 F인 노드는 T인 노드보다 무조건 먼저 지워진다.
* 간선을 만드는 과정을 생각해보면, 간선 x->y가 있다면 !y->!x 간선이 있고, x->!y가 있다면 y->!x가 있고, !x->y가 있다면 !y->x가 있다.
* 즉, 두 노드의 간선이 있다면 그 노드들의 반대 노드들 사이에도 대응되는 간선이 있다.
* 이를 생각했을 때, 만약 T->F인 간선 X->Y이 있다고 하면, !Y->!X 간선도 있을 것이다. 이때 각 노드들이 지워지는 순서를 생각해보자.
* 알고리즘은 입력 간선이 없는 노드부터 조회하므로 X->Y에서 X가 먼저 지워져야 하는데, X는 T이므로 F인 !X가 먼저 지워져야 한다.
* 그런데 !Y->!X 간선이 있으므로, !Y가 !X보다 먼저 지워져야 하고, !Y는 T이므로 Y가 그보다 먼저 지워져야 한다.
* 즉 지워지는 순서가 Y <- X <- !X <- !Y <- Y로 사이클이 발생해 모순이 생긴다. 따라서 위 알고리즘에선 T->F 간선이 발생할 수 없다.
*/
#include <iostream>
#include <vector>
#include <stack>

using namespace std;

int N, M, correct[20001], sccCounter, vertexCounter;
vector<int> adjacent[20001], discovered, sccId;
stack<int> st;

inline int TrueIdx(int v) { return v; }
inline int FalseIdx(int v) { return v + N; }
inline int OppositeIdx(int v) { return (v <= N) ? v + N : v - N; }

int SCC(int here)
{
	int ret = discovered[here] = vertexCounter++;
	st.push(here);

	for (int adj : adjacent[here])
	{
		if (discovered[adj] == -1)
			ret = min(ret, SCC(adj));
		else if (sccId[adj] == -1)
			ret = min(ret, discovered[adj]);
	}

	if (ret >= discovered[here])
	{
		while (true)
		{
			int t = st.top(); st.pop();
			sccId[t] = sccCounter;
			if (t == here) break;
		}
		sccCounter++;
	}
	return ret;
}

void TarjanSCC()
{
	discovered = sccId = vector<int>(2 * N + 1, -1);
	sccCounter = vertexCounter = 0;
	for (int i = 1; i <= 2 * N; ++i)
		if (discovered[i] == -1)
			SCC(i);
}

int IsPossible()
{
	TarjanSCC();
	for (int i = 1; i <= N; ++i)
		if (sccId[TrueIdx(i)] == sccId[FalseIdx(i)])
			return 0;
	return 1;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> M;

	for (int i = 1; i <= 2 * N; ++i)
	{
		correct[i] = -1;
	}

	for (int i = 0; i < M; ++i)
	{
		int a, b;
		cin >> a >> b;
		if (a > b) swap(a, b);

		if (b < 0) // both negative
		{
			a *= -1; b *= -1;
			adjacent[TrueIdx(a)].push_back(FalseIdx(b));
			adjacent[TrueIdx(b)].push_back(FalseIdx(a));
		}
		else if (a < 0) // only a negative
		{
			a *= -1;
			adjacent[TrueIdx(a)].push_back(TrueIdx(b));
			adjacent[FalseIdx(b)].push_back(FalseIdx(a));
		}
		else
		{
			adjacent[FalseIdx(a)].push_back(TrueIdx(b));
			adjacent[FalseIdx(b)].push_back(TrueIdx(a));
		}
	}

	cout << IsPossible() << "\n";
	return 0;
}