/*
* Problem: https://algospot.com/judge/problem/read/WORDCHAIN
* 책 해설) 각 단어의 첫 문자와 끝 문자를 노드로 만든 후, 단어를 그 사이의 간선으로 두는 그래프를 구성한뒤, DFS를 이용한 오일러 트레일 찾기 알고리즘을 사용하면 된다.
* 이때 각 노드로 들어오는 간선과 나가는 간선의 수를 세어 노드가 중간점/시작점/끝점 중 어느 것인지 알 수 있다. (나가는 간선이 들어오는 간선보다 1 많으면 시작점 / 들어오는 간선이 나가는 간선보다 1 많으면 끝점)
* 만약 시작점이나 끝점이 2개 이상이거나, 나가는 간선과 끝나는 간선의 개수 차이가 2 이상인 노드가 있다면 그 그래프에는 오일러 트레일이 존재하지 않는다.
* 
* 깨달은 점) 책에선 오일러 트레일의 끝점과 시작점을 연결하면 오일러 경로가 되므로, 오일러 트레일을 찾을 때 오일러 경로 찾기 알고리즘을 사용할 수 있다고 하였다.
* 이걸 보고 난 오일러 트레일에서 끝점과 시작점을 연결하여 오일러 경로를 구한 뒤 경로의 첫 원소를 제거하는 방식을 떠올렸는데,
* 이 문제를 풀면서 위 과정은 필요없고, in/out 간선 수를 비교하여 시작점을 알아낸 뒤 그 시작점부터 오일러 경로 찾기 알고리즘을 사용하면 오일러 트레일이 바로 구해진다는 것을 깨달았다.
* (어차피 간선 수 조건만 만족한다면 DFS는 반드시 끝점에서 끝나기 때문)
*/
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <memory.h>

using namespace std;

int N, inBound[26], outBound[26];
bool used[100];
vector<pair<int, int>> adjacent[26]; // there, wordIdx
vector<string> words;

inline int ToNumber(char ch) { return ch - 'a'; }

void DFS(vector<int>& outPath, int here)
{
	for (auto& adjInfo : adjacent[here])
	{
		int there = adjInfo.first;
		int wordIdx = adjInfo.second;
		if (used[wordIdx]) continue;

		used[wordIdx] = true;
		DFS(outPath, there);
		outPath.push_back(wordIdx);
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

		for (int i = 0; i < 26; ++i)
		{
			inBound[i] = 0;
			outBound[i] = 0;
			adjacent[i].clear();
		}

		words.clear();
		memset(used, false, sizeof(used));
		for (int i = 0; i < N; ++i)
		{
			string word;
			cin >> word;
			words.emplace_back(word);

			int frontIdx = ToNumber(word.front());
			int backIdx = ToNumber(word.back());
			adjacent[frontIdx].emplace_back(backIdx, i);

			outBound[frontIdx]++; inBound[backIdx]++;
		}

		int start = -1, end = -1;
		bool isPossible = true;
		for (int i = 0; i < 26 && isPossible; ++i)
		{
			if ((outBound[i] + inBound[i]) % 2)
			{
				if (outBound[i] > inBound[i])
				{
					if (start == -1) start = i;
					else isPossible = false;
				}
				else
				{
					if (end == -1) end = i;
					else isPossible = false;
				}
			}
			else if (abs(outBound[i] - inBound[i]) > 1)
				isPossible = false;
		}

		if (isPossible)
		{
			if (start == -1)
				start = ToNumber(words[0].front());
			vector<int> path;
			DFS(path, start);

			for (auto rIter = path.rbegin(); rIter != path.rend(); ++rIter)
				cout << words[*rIter] << " ";
			cout << "\n";
		}
		else
			cout << "IMPOSSIBLE" << "\n";
	}
	return 0;
}