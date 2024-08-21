/*
* Problem: https://algospot.com/judge/problem/read/DICTIONARY
* 문자열(단어)들을 서로 비교하며 개별 문자 간 순서를 알아내 adjacent에 저장하고, 이를 이용해 위상정렬로 알파벳을 순서에 맞게 정렬했다.
* 이때 위상 정렬은 DFS를 사용했으며(교재 831페이지 증명 참고), 마지막에 정렬된 순서를 뒤집는 수고를 덜기 위해 처음부터 문자 간 순서를 반대로 저장했다.
* 또한 모순이 발생하는지 찾기 위해 결과 문자열을 순회하며 현재 문자의 adjacent가 자신의 앞에 등장하는지 살펴보며 개별 순서가 모두 만족되는지 확인하였다.
* 
* 보충) 책 해설을 읽은 결과, 단어들을 비교할 때 모든 단어 쌍에 대해 할 필요 없이 인접한 단어끼리만 비교해도 필요한 모든 정보를 얻을 수 있다는 것을 알 수 있었다.
* 이를 적용한 결과 수행 시간이 기존의 172ms에서 4ms로 크게 단축되었다.
*/
#include <iostream>
#include <string>
#include <vector>
#include <memory.h>

using namespace std;

int n;
string words[1000];
vector<char> answer, adjacent[26];
bool visited[26];

inline int ToNumber(char ch) { return ch - 'a'; }

void CheckAdjacent(const string& a, const string& b)
{
	int idxA = 0, idxB = 0;
	while (idxA < a.size() && idxB < b.size() && a[idxA] == b[idxB])
	{
		idxA++; idxB++;
	}

	if (idxA < a.size() && idxB < b.size())
		adjacent[ToNumber(b[idxB])].push_back(a[idxA]);
}

void DFS(char ch)
{
	int node = ToNumber(ch);
	visited[node] = true;

	for (char adj : adjacent[node])
	{
		if (!visited[ToNumber(adj)])
			DFS(adj);
	}

	answer.push_back(ch);
}

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
			cin >> words[i];

		for (int i = 0; i < 26; ++i)
			adjacent[i].clear();

		for (int i = 0; i < n - 1; ++i)
		{
			int j = i + 1;
			CheckAdjacent(words[i], words[j]);
		}

		answer.clear();
		answer.reserve(26);
		memset(visited, 0, sizeof(visited));

		for (char ch = 'a'; ch <= 'z'; ++ch)
			if (!visited[ToNumber(ch)])
				DFS(ch);

		memset(visited, 0, sizeof(visited));
		bool isValid = true;
		for (char ch : answer)
		{
			visited[ToNumber(ch)] = true;
			for (char adj : adjacent[ToNumber(ch)])
				if (!visited[ToNumber(adj)])
					isValid = false;
		}

		if (isValid)
		{
			for (char ch : answer)
				cout << ch;
			cout << "\n";
		}
		else
			cout << "INVALID HYPOTHESIS" << "\n";
	}
	return 0;
}