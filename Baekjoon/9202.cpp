/*
* Problem: https://www.acmicpc.net/problem/9202
* 분류: 트라이
*/
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int W, B, longest, maxScore;
vector<bool> found;
vector<vector<bool>> visited;
const int points[9] = { 0, 0, 0, 1, 1, 2, 3, 5, 11 };
const int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 }, dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
string board[4];
vector<string> words;

inline bool IsInRange(int y, int x) { return y >= 0 && y < 4 && x >= 0 && x < 4; }
inline int ToIdx(char ch) { return ch - 'A'; }

struct TrieNode
{
	int terminal;
	TrieNode* children[26];

	TrieNode() : terminal(-1)
	{
		fill(children, children + 26, nullptr);
	}

	~TrieNode()
	{
		for (int i = 0; i < 26; ++i)
			if (children[i])
				delete children[i];
	}

	void Insert(const char* pNumChar, int wordId)
	{
		if (*pNumChar == 0)
		{
			terminal = wordId;
			return;
		}

		int next = ToIdx(*pNumChar);
		if (children[next] == nullptr) children[next] = new TrieNode();
		return children[next]->Insert(pNumChar + 1, wordId);
	}

	bool Find(const char* pNumChar)
	{
		if (*pNumChar == 0 && terminal != -1) return true;

		int next = ToIdx(*pNumChar);
		if (children[next])
			return children[next]->Find(pNumChar + 1);
		return false;
	}
};

void DFS(int y, int x, TrieNode* node)
{
	if (node->terminal != -1)
	{
		int wordID = node->terminal;
		found[wordID] = true;

		if (longest == -1 || words[longest].size() < words[wordID].size())
			longest = wordID;
		else if (words[longest].size() == words[wordID].size())
		{
			if (words[wordID] < words[longest])
				longest = wordID;
		}
	}
	visited[y][x] = true;

	for (int i = 0; i < 8; ++i)
	{
		int ny = y + dy[i];
		int nx = x + dx[i];
		if (!IsInRange(ny, nx)) continue;

		int next = ToIdx(board[ny][nx]);
		if (!visited[ny][nx] && node->children[next])
		{
			DFS(ny, nx, node->children[next]);
		}
	}
}

void Answer(TrieNode* root)
{
	maxScore = 0;
	longest = -1;

	found = vector<bool>(W, false);
	for (int y = 0; y < 4; ++y)
		for (int x = 0; x < 4; ++x)
		{
			visited = vector<vector<bool>>(4, vector<bool>(4, false));
			int start = ToIdx(board[y][x]);
			if (root->children[start])
				DFS(y, x, root->children[start]);
		}

	int totalScore = 0, count = 0;
	for (int i = 0; i < W; ++i)
		if (found[i])
		{
			totalScore += points[words[i].size()];
			count++;
		}
	maxScore = max(maxScore, totalScore);

	cout << maxScore << " " << words[longest] << " " << count << "\n";
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);


	cin >> W;
	TrieNode* trieRoot = new TrieNode();
	words = vector<string>(W);
	for (int i = 0; i < W; ++i)
	{
		cin >> words[i];
		trieRoot->Insert(words[i].c_str(), i);
	}

	cin >> B;
	for (int i = 0; i < B; ++i)
	{
		for (int j = 0; j < 4; ++j)
			cin >> board[j];

		Answer(trieRoot);
	}
	delete trieRoot;

	return 0;
}