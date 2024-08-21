/*
* Problem: https://algospot.com/judge/problem/read/DICTIONARY
* 문자열들을 서로 비교하며 개별 문자 간 순서를 알아내 adjacent에 저장하고, 이를 이용해 위상정렬로 알파벳을 순서에 맞게 정렬했다.
* 이때 위상 정렬은 DFS를 사용했으며(교재 831페이지 증명 참고), 마지막에 정렬된 순서를 뒤집는 수고를 덜기 위해 처음부터 문자 간 순서를 반대로 저장했다.
*/
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <memory.h>

using namespace std;

int n;
string words[1000];
vector<char> answer;
set<char> adjacent[26];
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
		adjacent[ToNumber(b[idxB])].insert(a[idxA]);
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

		for (int i = 0; i < n; ++i)
			for (int j = i + 1; j < n; ++j)
				CheckAdjacent(words[i], words[j]);

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