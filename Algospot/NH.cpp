/*
* Problem: https://algospot.com/judge/problem/read/NH
* 동적 계획법과 아호-코라식 문자열 검색 알고리즘을 사용해 해결했다.
* 책 해설) 동적계획법을 사용해 직접 감지되지 않는 문자열을 만들어가며 개수를 구할 수 있다.
* 이때 문자열을 만들기 위해선 이전까지의 문자열과 만들어야 하는 남은 문자 개수가 필요한데, 이전까지의 문자열을 메모해봤자 중복 문제가 발생하지 않으므로
* 이를 대신해 아호-코라식 알고리즘을 위해 만들어진 트라이에서의 노드 번호를 사용한다.
* 이전까지의 문자열이 필요한 이유는 다음 문자를 추가했을때 인식 패턴이 발생하지 않는지 알기 위함이므로 트라이 노드 번호로 충분히 대체할 수 있다.
* 
* GetUnbannedCount 함수의 시간 복잡도는 O(N * M * A * L^2)이며 (A는 알파벳 개수), N * M * A * L^2의 최대치는 100 * 100 * 26 * 100 = 2600만이므로 시간 초과가 발생하지 않는다.
* 현재 코드에선 문자를 받았을 때 이동할 다음 노드인 next를 실시간으로 계산했지만, 미리 계산해둔다면 시간 복잡도를 O(N * M * A * L)로 감소시킬 수 있다.
*/
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <memory.h>

using namespace std;

const int ALPHABETS = 26, DIVIDER = 10007;
int N, M, memo[1000][101], nodeCnt = 0;
struct TrieNode* trieRoot;

inline int ToNumber(char ch)
{
	return ch - 'a';
}

struct TrieNode
{
	int terminal, nodeId;
	TrieNode* children[ALPHABETS], * failure;
	vector<int> outputs;

	TrieNode() : terminal(-1), nodeId(nodeCnt++), failure(nullptr)
	{
		fill_n(&children[0], ALPHABETS, nullptr);
	}

	~TrieNode()
	{
		for (int i = 0; i < ALPHABETS; ++i)
			if (children[i])
				delete children[i];
	}

	void Insert(const char* key, int id)
	{
		if (*key == 0)
			terminal = id;
		else
		{
			int next = ToNumber(*key);
			if (!children[next])
				children[next] = new TrieNode();
			children[next]->Insert(key + 1, id);
		}
	}
};

void CalcFailure(TrieNode* root)
{
	queue<TrieNode*> q;
	root->failure = root;
	q.push(root);

	while (!q.empty())
	{
		TrieNode* cur = q.front();
		q.pop();

		for (int i = 0; i < ALPHABETS; ++i)
		{
			TrieNode* child = cur->children[i];
			if (!child) continue;

			if (cur == root)
				child->failure = root;
			else
			{
				TrieNode* node = cur->failure;
				while (node != root && node->children[i] == nullptr)
					node = node->failure;

				if (node->children[i])
					node = node->children[i];

				child->failure = node;
				child->outputs = node->outputs;
			}

			if (child->terminal != -1)
				child->outputs.push_back(child->terminal);

			q.push(child);
		}
	}
}

int GetUnbannedCount(TrieNode* state, int chLeft)
{
	if (!state->outputs.empty()) return 0;
	if (chLeft == 0) return 1;

	int& ret = memo[state->nodeId][chLeft];
	if (ret != -1) return ret;

	ret = 0;
	for (int i = 0; i < ALPHABETS; ++i)
	{
		TrieNode* next = state;
		while (next != trieRoot && next->children[i] == nullptr)
			next = next->failure;

		if (next->children[i]) next = next->children[i];
		ret = (ret + GetUnbannedCount(next, chLeft - 1)) % DIVIDER;
	}
	return ret;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> N >> M;
		memset(memo, -1, sizeof(memo));

		nodeCnt = 0;
		trieRoot = new TrieNode();
		for (int i = 0; i < M; ++i)
		{
			string ban;
			cin >> ban;
			trieRoot->Insert(ban.c_str(), i);
		}
		CalcFailure(trieRoot);
		cout << GetUnbannedCount(trieRoot, N) << "\n";
		delete trieRoot;
	}
	return 0;
}