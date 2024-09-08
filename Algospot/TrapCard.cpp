/*
* Problem: https://algospot.com/judge/problem/read/TRAPCARD
* 분류: 이분 매칭
* 증명을 하는 과정에서 해결 알고리즘이 만들어지는 구성적 증명을 사용함.
* 
* 책 해설) 그래프에서 어떠한 것도 서로 인접하지 않도록 노드들을 최대한 선택한 것을 최대 분리 집합이라고 한다. 이것은 지도를 그래프로 나타냈을 때 함정을 최대한 설치하기 위해 선택해야 하는 칸들의 집합과 같다.
* 기지의 칸들을 상하좌우로 인접한 칸끼리 서로 다른 집합에 속하도록 두 집합으로 분리시킨 뒤 ((x + y) % 2 값을 이용), 각 빈 칸들을 노드로 만들고 지도 상에서 인접한 노드들을 서로 간선으로 잇는다.
* 이제 전체 노드들을 함정을 설치할 장소로 선택한 뒤 두 집합 간의 최대 이분 매칭을 구하여, 이분 매칭에 참여하는 각 노드 쌍에서 특정한 방법으로 하나의 노드씩만 선택에서 제외하면 최대 분리 집합을 얻을 수 있다.
* 이 방법의 정당성을 증명하는 과정에서 문제를 해결하기 위해 필요한 알고리즘을 얻을 수 있다.
* (해당 증명은 교재의 1033페이지를 참고)
*/
#include <iostream>
#include <vector>
#include <memory.h>
#include <string>

using namespace std;

const int MAX_N = 200, MAX_M = 200;
const int dx[] = { 1, -1, 0, 0 }, dy[] = { 0, 0, 1, -1 };

int H, W, n, m;
bool adjacent[MAX_N][MAX_M];
vector<string> board;
vector<bool> visited;
vector<int> aMatch, bMatch;
vector<vector<int>> blockID;
vector<pair<int, int>> aID2Pos, bID2Pos;

inline int GetTeam(int y, int x) { return (x + y) % 2; }

int AssignID(int y, int x)
{
	if (blockID[y][x] == -1)
	{
		if (GetTeam(y, x) == 0)
		{
			blockID[y][x] = n++;
			aID2Pos[blockID[y][x]] = make_pair(y, x);
		}
		else
		{
			blockID[y][x] = m++;
			bID2Pos[blockID[y][x]] = make_pair(y, x);
		}
	}
	return blockID[y][x];
}

inline bool IsInRange(int y, int x)
{
	return y >= 0 && y < H && x >= 0 && x < W;
}

bool DFS(int a)
{
	if (visited[a]) return false;
	visited[a] = true;

	for (int b = 0; b < m; ++b)
	{
		if (!adjacent[a][b]) continue;

		if (bMatch[b] == -1 || DFS(bMatch[b]))
		{
			aMatch[a] = b;
			bMatch[b] = a;
			return true;
		}
	}
	return false;
}

int BipartiteMatch()
{
	aMatch = vector<int>(n, -1);
	bMatch = vector<int>(m, -1);

	int match = 0;
	for (int i = 0; i < n; ++i)
	{
		visited = vector<bool>(n, false);
		if (DFS(i)) match++;
	}
	return match;
}

void MakeGraph()
{
	n = 0; m = 0;
	blockID = vector<vector<int>>(H, vector<int>(W, -1));
	aID2Pos = bID2Pos = vector<pair<int, int>>(H * W / 2);
	memset(adjacent, false, sizeof(adjacent));

	for (int y = 0; y < H; ++y)
		for (int x = 0; x < W; ++x)
		{
			if (board[y][x] != '.') continue;
			int id = AssignID(y, x);

			for (int dir = 0; dir < 4; ++dir)
			{
				int cy = y + dy[dir];
				int cx = x + dx[dir];

				if (IsInRange(cy, cx) && board[cy][cx] == '.')
				{
					int cID = AssignID(cy, cx);
					if (GetTeam(y, x) == 0) adjacent[id][cID] = true;
					else adjacent[cID][id] = true;
				}
			}
		}
}

int GetAnswer()
{
	MakeGraph();
	BipartiteMatch();

	vector<bool> aSelected(n, true), bSelected(m, false);

	int ret = n;
	for (int i = 0; i < m; ++i)
		if (bMatch[i] == -1)
		{
			bSelected[i] = true;
			ret++;
		}

	while (true)
	{
		bool isChanged = false;
		for (int a = 0; a < n; ++a)
			for (int b = 0; b < m; ++b)
			{
				if (aSelected[a] && bSelected[b] && adjacent[a][b])
				{
					aSelected[a] = false;
					bSelected[aMatch[a]] = true;
					isChanged = true;
				}
			}

		if (!isChanged)
			break;
	}

	for (int a = 0; a < n; ++a)
		if (aSelected[a])
		{
			int y = aID2Pos[a].first;
			int x = aID2Pos[a].second;
			board[y][x] = '^';
		}

	for (int b = 0; b < m; ++b)
		if (bSelected[b])
		{
			int y = bID2Pos[b].first;
			int x = bID2Pos[b].second;
			board[y][x] = '^';
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
		cin >> H >> W;
		board = vector<string>(H);
		for (int i = 0; i < H; ++i)
			cin >> board[i];

		cout << GetAnswer() << "\n";
		for (int i = 0; i < board.size(); ++i)
			cout << board[i] << "\n";
	}
	return 0;
}

