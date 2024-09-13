/*
* Problem: https://www.acmicpc.net/problem/5052
* 분류: 트라이
* 
* 모든 전화번호를 트라이에 넣은 후, 각 전화번호를 트라이에서 찾으면서 자신의 접두사가 되는 전화번호가 있는지 확인하였다.
*/
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <memory.h>

using namespace std;

int N;
vector<string> numbers;

inline int ToIdx(char ch) { return ch - '0'; }

struct TrieNode
{
	bool isTerminal;
	TrieNode* children[10];

	TrieNode() : isTerminal(false)
	{
		fill(children, children + 10, nullptr);
	}

	~TrieNode()
	{
		for (int i = 0; i < 10; ++i)
			if (children[i])
				delete children[i];
	}

	void Insert(const char* pNumChar)
	{
		if (*pNumChar == 0)
		{
			isTerminal = true;
			return;
		}

		int next = ToIdx(*pNumChar);
		if (children[next] == nullptr) children[next] = new TrieNode();
		return children[next]->Insert(pNumChar + 1);
	}

	bool HasContradiction(const char* pNumChar)
	{
		if (*pNumChar == 0) return false;
		if (isTerminal) return true;

		int next = ToIdx(*pNumChar);
		if (children[next])
			return children[next]->HasContradiction(pNumChar + 1);
		return false;
	}
};

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int T;
	cin >> T;
	while (T--)
	{
		cin >> N;
		numbers = vector<string>(N);
		TrieNode* trieRoot = new TrieNode();
		for (int i = 0; i < N; ++i)
		{
			cin >> numbers[i];
			trieRoot->Insert(numbers[i].c_str());
		}

		bool isValid = true;
		for (int i = 0; i < N; ++i)
			if (trieRoot->HasContradiction(numbers[i].c_str()))
			{
				isValid = false;
				break;
			}

		cout << (isValid ? "YES" : "NO") << "\n";
		delete trieRoot;
	}

	return 0;
}