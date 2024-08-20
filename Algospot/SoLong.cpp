/*
* Problem: https://algospot.com/judge/problem/read/SOLONG
* 문자열의 접두사를 통해 원하는 문자열을 찾아내야 하므로 트라이를 사용했다.
* 시도) 트라이에 사전 단어들을 저장하는데, 이때 노드마다 출현 빈도와 추천 문자열로의 간선을(추천 문자열 상에서 다음 문자) 저장한다.
* 이후 돌고래가 입력한 문자열로 트라이를 타고 내려가며, 각 노드에서 추천되는 간선으로 간 경우와 추천되지 않는 간선으로 간 경우를 따지며
* typed(이미 입력한 문자 수)와 reserved(자동완성으로 스킵가능한지 아직 모르는 문자의 수)를 조절해 각 문자열 입력에 필요한 최소 타자 수를 계산했다.
* 예제는 맞았으나 실제 채점에서 오답이 발생함.
* 
* 책해답) 사전의 단어들을 벡터에 넣은 뒤 추천순으로 정렬하고 순서대로 트라이에 넣는다. 이렇게하면 트라이의 각 노드에선 자신이 루트인 서브트리 트라이에 가장 먼저 들어온 단어를 추천하면 된다.
* 이때 메모리와 단어 비교에 필요한 시간을 단축시키기 위해, 추천 단어를 노드에 문자열 그대로 저장하지 않고 벡터에서의 인덱스를 id로 사용해 저장한다.
* 트라이 노드의 terminal은 int형으로 선언해 현재 노드에서 단어가 끝나면 해당 단어의 id를, 아니면 -1을 저장한다.
* 이제 이 트라이를 활용해 특정 id의 단어를 치기위해 필요한 최소 타자 수를 구하는 Type 함수를 구현하면 된다.
* 이때 돌고래가 입력한 단어가 사전에 없다면 해당 단어의 문자수를 통째로 총 타자 수에 더한다.
*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int N, M;
const int ALPHABETS = 26;
vector<pair<int, string>> words;

int toNumber(char ch) { return ch - 'A'; }

struct TrieNode
{
	int autoComp, terminal;
	TrieNode* children[ALPHABETS];

	TrieNode() : terminal(-1), autoComp(-1)
	{
		fill(&children[0], &children[ALPHABETS], nullptr);
	}

	~TrieNode()
	{
		for (TrieNode* child : children)
			if (child)
				delete child;
	}

	void Insert(const char* key, int id)
	{
		if (autoComp == -1) autoComp = id;

		if (*key == 0)
			terminal = id;
		else
		{
			int nextNum = toNumber(*key);
			TrieNode* next = children[nextNum];
			if (next == nullptr)
				next = children[nextNum] = new TrieNode();
			next->Insert(key + 1, id);
		}
	}

	TrieNode* Find(const char* key)
	{
		if (*key == 0) return this;

		TrieNode* next = children[toNumber(*key)];
		if (next == nullptr) return nullptr;
		return next->Find(key + 1);
	}

	int Type(const char* key, int id)
	{
		if (*key == 0) return 0;
		if (autoComp == id) return 1;
		TrieNode* next = children[toNumber(*key)];
		return next->Type(key + 1, id) + 1;
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

		words.clear();
		words.reserve(N);
		for (int i = 0; i < N; ++i)
		{
			string word;
			int freq;
			cin >> word >> freq;
			words.emplace_back(-freq, word);
		}
		sort(words.begin(), words.end());

		TrieNode* trieRoot = new TrieNode();
		for (int i = 0; i < words.size(); ++i)
			trieRoot->Insert(words[i].second.c_str(), i);
		trieRoot->autoComp = -1;

		int sum = M - 1;
		for (int i = 0; i < M; ++i)
		{
			string word;
			cin >> word;
			TrieNode* node = trieRoot->Find(word.c_str());
			if (node && node->terminal != -1)
				sum += trieRoot->Type(word.c_str(), node->terminal);
			else
				sum += word.size();
		}
		cout << sum << "\n";
		delete trieRoot;
	}
	return 0;
}