/*
* Problem: https://www.acmicpc.net/problem/9202
* 분류: 트라이
* 
* 먼저 사전 속 단어들로 트라이를 만들었다. 이때 각 트라이 노드는 현재 노드에서 단어가 완성된다면 terminal에 단어의 id 값을, 아니라면 -1을 저장한다.
* 이후 보드의 각 위치에서 DFS를 시작해 인접 칸들로 이동하며 그 경로에 맞게 트라이 노드들을 조사했다.
* 만약 특정 노드에서 단어가 완성된다면 found 배열에서 그 단어를 true로 설정하고 가장 긴 단어 정보를 업데이트했다.
* DFS를 완료하면 found 배열을 조사하며 점수 총합과 찾은 단어 개수를 세어 답을 구할 수 있다.
*/
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int W, B, longest;
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
};

void DFS(int y, int x, TrieNode* node)
{
	if (node->terminal != -1)
	{
		int wordID = node->terminal;
		if (!found[wordID])
		{
			found[wordID] = true;
			if (longest == -1 || words[longest].size() < words[wordID].size())
				longest = wordID;
			else if (words[longest].size() == words[wordID].size())
			{
				if (words[wordID] < words[longest])
					longest = wordID;
			}
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
	visited[y][x] = false;
}

void Answer(TrieNode* root)
{
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

	cout << totalScore << " " << words[longest] << " " << count << "\n";
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