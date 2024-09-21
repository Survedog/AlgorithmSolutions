/*
* Problem: https://algospot.com/judge/problem/read/MEETINGROOM
* 시도1) 회의가 끝나는 시간을 노드로, 회의를 간선으로 한다면? -> 조합탐색이 된다
* 시도2) 0~43200까지의 모든 시간을 노드로 만들고 회의를 간선으로 한다면? -> 두 회의 중 어느걸 잡아야할 지는 여전히 모르고, 회의를 반드시 연이어하지도 않음.
*
* [책해설]
* 요약) 이 문제를 2-SAT 문제로 바꾸면 다항 시간 안에 해결할 수 있다. (NP-Hard인 SAT 문제와 달리 2-SAT은 다항 시간 안에 풀 수 있음)
* 모든 회의가 열리기 위한 조건을 논리곱 정규형으로 표현한 뒤 그것을 토대로 함의 그래프(논리식에 포함된 변수들의 값에 대한 요구 조건을 표현한 그래프)를 구성한다.
* 그 그래프를 SCC로 압축한 뒤 각 SCC 내 노드들을 조사하면 위 논리곱 정규형이 성립하는(즉, 모든 회의가 열릴 수 있는) 경우가 있는지 알 수 있다.
* 만약 가능하다면, 들어오는 간선이 없는 노드부터 차례대로 참/거짓을 결정해나가면 된다.
*
* 설명) 제약관계를 논리곱 정규형으로 표현하면, 모든 회의가 열릴 수 있으려면 다음의 두 조건이 성립해야 한다.
* 1. 임의의 팀 i가 있을 때, i의 두 회의 A_i, A_i+1 각각의 개최 여부를 true/false로 나타낼 경우 'A_i || A_i+1 = true'이어야 한다.
*	 즉, A_i = false이면 A_i+1 = true여야 한다.
* 2. 임의의 팀 i와 j가 있을 때, 회의 A_i와 A_j의 회의 시간이 겹친다면 '!A_i || !A_j =  true'이어야 한다.
*	 즉, A_i가 true이면 A_j는 false여야 한다.
*
* 각 회의들의 개최 여부가 true인 경우와 false인 경우를 각각 노드로 만들면 위 조건들을 다음의 간선들로 표현할 수 있다.
* 1. (!A_i)	 -> (A_i+1)
* 2. (!A_i+1) -> (A_i)
* 3. (A_i)	 -> (!A_j)
* 4. (A_j)	 -> (!A_i)
*
* 위 내용을 토대로 방향 그래프를 구성해보자.
* 각 노드의 실제 값(아직은 알 수 없음)이 참이라면 참 노드, 거짓이라면 거짓 노드라고 하겠다. ex) A_i가 false라면 A_i는 거짓 노드, !A_i는 참노드
* 위 간선들은 P이면 Q이다의 조건을 간선으로 나타낸 것이므로, 간선(조건)의 시작 노드가 참 노드일 때 끝 노드도 참 노드라면 그 조건을 만족한다고 할 수 있다.
* 따라서 모든 회의가 열리기 위해선 참 노드에서 시작하는 모든 간선이 참 노드에서 끝나도록 회의를 열어야 한다는 것이다.
*
* 만약 임의의 회의 i를 표현하는 A_i, !A_i 노드가 한 SCC에 있다면 그 SCC 내엔 반드시 참노드에서 거짓 노드로 이동하는 간선이 생길 수 밖에 없다.
* 반면 그런 노드들이 존재하지 않다면 반드시 답이 존재한다는 것을 증명할 수 있다.
* 따라서 타잔 SCC 알고리즘으로 모든 SCC를 구하고 각 SCC 내 노드들을 살펴보면 모든 회의가 열릴 수 있는지 확인할 수 있다.
*
* 이때 모든 SCC는 같은 속성(참/거짓)의 노드로만 구성되어 있다는 사실을 알 수 있다.
* 또한, !X => Y인 간선이 있다면 !Y => X인 간선도 있으므로, A_0, !A_2, A3, !A_1가 같은 SCC에 속해있다면 !A_0, A_2, !A_3, A_1도 같은 SCC에 속해있다.
* 따라서 한 SCC에는 대응하는 반대 속성의 SCC가 반드시 하나씩 존재한다는 것을 알 수 있다.
*
* 이제 각 SCC 그래프를 SCC로 압축한 뒤, 각 정점(SCC)가 참인지 거짓인지 정하기만 하면 된다.
* 들어오는 간선이 없는 정점을 찾아, 만약 아직 참/거짓이 정해지지 않았다면 거짓으로 바꾼 뒤, 자신과 반대되는 정점을 참으로 바꾼 후 자신을 삭제한다. (이미 정해져 있다면 바로 지운다)
* (거짓으로 바꾸는 이유: 참으로 바꾸면 주변 정점도 모두 참이 돼야 하지만, 거짓 정점으로 바꾸면 그런 제약이 없어 불이익이 전혀 없기 때문)
* 이 과정을 통해 모든 SCC의 참/거짓을 알아내면, 모든 노드의 참/거짓은 자신이 속한 SCC를 확인하면 알 수 있다.
* 정당성 증명은 교재 p.878페이지 혹은 Baekjoon/11280.cpp 참고.
* 
* 교재의 실제 구현에선 SCC를 직접 압축하지 않았으며, TarjanSCC로 지정된 scc 순서는 위상 정렬의 역순이라는 사실을 이용해 노드들을 순서대로 조회하며 해결했다.
* 이 소스코드는 교재의 구현과 거의 비슷하게 작성했지만, 교재에서 만든 함의 그래프는 한 팀의 두 회의 중 하나만 개최할 수 있다는 제약을 갖고 있지 않아, 두 회의가 모두 가능하다면 그 중 하나만 답으로 내놓을 필요가 있었다.
* 이를 두가지 방식으로 구현해보았다.
* 1) 함의 그래프에 간선으로 (A_i) -> (!A_i+1), (A_i+1) -> (!A_i)를 추가한다. (이전 커밋)
* 2) 팀이 두 회의 중 하나를 수행하면 다른 하나는 답으로 내놓지 않음. (현재 커밋)
*
* 발견한 것) vector.resize(n, x)를 수행하면 벡터에선 오직 새로운 원소에 대해서만 x를 할당한다.
* 즉 아직 남아있는 기존 원소는 동일한 값을 가지기 때문에, 벡터를 재사용하기 위해 초기화하는 데엔 적합하지 않다.
*/
#include <iostream>
#include <vector>
#include <stack>
#include <memory.h>
#include <algorithm>

using namespace std;

int N, sccCounter, vertexCounter;
vector<int> discovered, sccId;
vector<vector<int>> adjacent;
stack<int> st;


inline bool IsDisjoint(const pair<int, int>& a, const pair<int, int>& b)
{
	return a.second <= b.first || b.second <= a.first;
}

inline int GetPositiveNode(int a) { return 2 * a; }
inline int GetNegativeNode(int a) { return 2 * a + 1; }

void MakeGraph(const vector<pair<int, int>>& meetings)
{
	adjacent.clear();
	adjacent.resize(2 * meetings.size(), vector<int>());
	for (int daily = 0; daily < meetings.size(); daily += 2)
	{
		int weekly = daily + 1;
		adjacent[GetNegativeNode(daily)].push_back(GetPositiveNode(weekly));
		adjacent[GetNegativeNode(weekly)].push_back(GetPositiveNode(daily));
	}

	for (int i = 0; i < meetings.size(); ++i)
		for (int j = i + 1; j < meetings.size(); ++j)
			if (!IsDisjoint(meetings[i], meetings[j]))
			{
				adjacent[GetPositiveNode(i)].push_back(GetNegativeNode(j));
				adjacent[GetPositiveNode(j)].push_back(GetNegativeNode(i));
			}
}

int SCC(int here)
{
	st.push(here);
	int ret = discovered[here] = vertexCounter++;

	for (int there : adjacent[here])
	{
		if (discovered[there] == -1)
			ret = min(ret, SCC(there));
		else if (sccId[there] == -1)
			ret = min(ret, discovered[there]);
	}

	if (ret == discovered[here])
	{
		while (true)
		{
			int node = st.top();
			st.pop();
			sccId[node] = sccCounter;
			if (node == here) break;
		}
		sccCounter++;
	}

	return ret;
}

void TarjanSCC(int nodeCount)
{
	discovered.clear();
	discovered.resize(nodeCount, -1);
	sccId.clear();
	sccId.resize(nodeCount, -1);
	sccCounter = vertexCounter = 0;
	for (int i = 0; i < nodeCount; ++i)
		if (discovered[i] == -1)
			SCC(i);
}

void Solve2SAT(vector<int>& outAnswer)
{
	int nodeCount = adjacent.size();
	TarjanSCC(nodeCount);

	for (int i = 0; i < nodeCount; i += 2)
		if (sccId[i] == sccId[i + 1])
		{
			outAnswer.clear();
			return;
		}

	vector<pair<int, int>> order;
	for (int i = 0; i < nodeCount; ++i)
		order.emplace_back(-sccId[i], i);
	sort(order.begin(), order.end());

	outAnswer.resize(nodeCount / 2, -1);
	for (int i = 0; i < order.size(); ++i)
	{
		int node = order[i].second;
		int variable = node / 2;
		bool isPositive = node % 2 == 0;

		if (outAnswer[variable] != -1) continue;
		outAnswer[variable] = !isPositive;
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
		cin >> N;
		vector<pair<int, int>> meetings;
		for (int i = 0; i < N; ++i)
		{
			int a, b, c, d;
			cin >> a >> b >> c >> d;
			meetings.emplace_back(a, b);
			meetings.emplace_back(c, d);
		}

		MakeGraph(meetings);

		vector<int> answer;
		Solve2SAT(answer);
		if (answer.empty())
			cout << "IMPOSSIBLE" << "\n";
		else
		{
			cout << "POSSIBLE" << "\n";
			bool isTeamOver = false;
			for (int i = 0; i < answer.size(); ++i)
			{
				if (isTeamOver)
				{
					isTeamOver = false;
					continue;
				}
				if (answer[i]) cout << meetings[i].first << " " << meetings[i].second << "\n";
				isTeamOver = i % 2 == 0 && answer[i];
			}
		}
	}

	return 0;
}