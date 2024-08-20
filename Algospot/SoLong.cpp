/*
* Problem: https://algospot.com/judge/problem/read/SOLONG
* 문자열의 접두사를 통해 원하는 문자열을 찾아내야 하므로 트라이를 사용했다.
* 시도) 트라이에 사전 단어들을 저장하는데, 이때 노드마다 출현 빈도와 추천 문자열로의 간선을(추천 문자열 상에서 다음 문자) 저장한다.
* 이후 돌고래가 입력한 문자열로 트라이를 타고 내려가며, 각 노드에서 추천되는 간선으로 간 경우와 추천되지 않는 간선으로 간 경우를 따지며
* typed(이미 입력한 문자 수)와 reserved(자동완성으로 스킵가능한지 아직 모르는 문자의 수)를 조절해 각 문자열 입력에 필요한 최소 타자 수를 계산했다.
* 예제는 맞았으나 실제 채점에서 오답이 발생함.
*/

#include <iostream>
#include <string>
#include <memory.h>

using namespace std;

int N, M;
const int ALPHABETS = 26;

int toNumber(char ch) { return ch - 'A'; }

struct TrieNode
{
	char autoCompNext;
	int freq, autoCompFreq;
	bool isTerminal;
	TrieNode* children[ALPHABETS];

	TrieNode() : isTerminal(false), freq(0), autoCompFreq(0), autoCompNext(0)
	{
		fill(&children[0], &children[ALPHABETS], nullptr);
	}

	~TrieNode()
	{
		for (TrieNode* child : children)
			if (child)
				delete child;
	}

	void Insert(const char* key, int inFreq)
	{
		if (inFreq > autoCompFreq || (inFreq == autoCompFreq && *key <= autoCompNext))
		{
			autoCompFreq = inFreq;
			autoCompNext = *key;
		}

		if (*key == 0)
		{
			freq = inFreq;
			isTerminal = true;
		}
		else
		{
			int nextNum = toNumber(*key);
			TrieNode* next = children[nextNum];
			if (next == nullptr)
				next = children[nextNum] = new TrieNode();
			next->Insert(key + 1, inFreq);
		}
	}

	int FindTypeCount(const char* key, int wordLenLeft, int typed, int reserved)
	{
		if (*key == 0)
		{
			if (reserved == 0) return typed;
			return autoCompNext == 0 ? typed + 1 : typed + reserved;
		}

		if (autoCompNext == *key)
			return children[toNumber(autoCompNext)]->FindTypeCount(key + 1, wordLenLeft - 1, typed, reserved + 1);
		else
		{
			if (children[toNumber(*key)])
				return children[toNumber(*key)]->FindTypeCount(key + 1, wordLenLeft - 1, typed + reserved + 1, 0);
			else
				return wordLenLeft + typed + reserved;
		}
	}
};

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> N >> M;
		TrieNode* trieRoot = new TrieNode();
		trieRoot->autoCompFreq = 11;
		for (int i = 0; i < N; ++i)
		{
			string word;
			int freq;
			cin >> word >> freq;
			trieRoot->Insert(word.c_str(), freq);
		}

		int sum = M - 1;
		for (int i = 0; i < M; ++i)
		{
			string word;
			cin >> word;
			sum += trieRoot->FindTypeCount(word.c_str(), word.length(), 0, 0);
		}
		cout << sum << "\n";
		delete trieRoot;
	}
	return 0;
}