/*
* Problem: https://algospot.com/judge/problem/read/HANOI4
* 원반들이 배치된 상태를 하나의 노드로 하는 그래프를 만들어 풀기로 했다.
* 이때 다음을 고려해서 노드를 표현하기 적합한 자료구조를 구상했다.
* 1) 상태 사이의 동등성을 간단히 비교할 수 있음
* 2) 인접한 상태를 쉽게 알아낼 수 있음
* 3) 적은 메모리를 사용해야 함.
*
* 각 기둥에 있는 원반의 순서는 정해져있으므로 벡터에 각 원반이 배치된 기둥의 인덱스를 저장하면 최대 12개의 정수로 하나의 상태를 표현할 수 있다.
* 더 나아가 기둥의 인덱스(0~3)는 2비트만으로 표현할 수 있으므로, 비트마스크를 사용하면 12 * 2비트 = 24비트로 상태를 표현할 수가 있다.
* 따라서 32비트 정수로 상태를 표현하면 상태끼리의 비교를 O(1)에 수행할 수 있고, 큐에 저장할 때도 메모리를 적게 사용한다.
* 인접한 상태를 알아내는 것이 문제인데, 비트 AND연산을 통해 각 원반의 위치를 알아내면 O(N)시간에 인접한 상태를 구할 수 있다.
* 
* 원반들이 배치될 수 있는 상태의 총 개수는 4^12 = 약 1600만으로, BFS 시 시간 초과가 발생할 우려가 있으나, 시도해볼 가치가 있다.
* 시도1) 너비우선 탐색을 그대로 적용했음. 시간 초과가 발생함. (이전 커밋)
* 시도2) 양방향 탐색을 실시함. 오답이 발생했는데, start와 finish가 같은 경우를 예외처리하지 않은 것과 디버그 시 가독성을 위해 답에 Answer 문구를 추가한 것을 지우지 않은게 문제였다.
* 이것들을 해결하니 무사히 정답을 맞출 수 있었다. (현재 커밋)
* 
* 배운 점)
* 1. 처음엔 State 구조체에 상태를 표현하는 정수 값을 넣어 썼으나 int형을 그대로 쓰는 것에 비해 코드만 더 길어지고 복잡해짐.
* 2. GetEachTop 시 min 연산을 할 필요 없이, 큰 원반부터 차례대로 순회하며 자신이 있는 기둥에 원반 번호를 기록하면 기둥 별로 가장 작은 원반이 저장된다.
*/
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <memory.h>
#include <cmath>

using namespace std;

const int MAX_DISK = 12;
int start, finish, N, dist[1 << 24];
queue<int> q;

int Vec2Int(const vector<int>& v)
{
	int ret = 0, loc = 0;
	for (int i = 0; i < v.size(); ++i)
		ret |= v[i] << (2 * i);
	return ret;
}

inline int Sign(int x)
{
	if (x == 0) return 0;
	return x > 0 ? 1 : -1;
}

inline int Incr(int x)
{
	return x >= 0 ? x + 1 : x - 1;
}

int MoveDisk(int state, int disk, int toPillar)
{
	state &= ~(3 << (2 * disk));
	state |= toPillar << (2 * disk);
	return state;
}

void GetEachTop(int state, vector<int>& outTops)
{
	outTops.clear();
	outTops.resize(4, MAX_DISK);
	for (int disk = N - 1; disk >= 0; --disk)
	{
		int pillar = 3 & (state >> (2 * disk));
		outTops[pillar] = disk;
	}
}

int BFS()
{
	if (start == finish) return 0;

	while (!q.empty()) q.pop();
	q.emplace(start); dist[start] = 1;
	q.emplace(finish); dist[finish] = -1;

	while (!q.empty())
	{
		int here = q.front();
		q.pop();

		vector<int> tops;
		GetEachTop(here, tops);

		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				if (i != j && tops[i] < tops[j])
				{
					int next = MoveDisk(here, tops[i], j);
					if (dist[next] == 0)
					{
						int nextDist = Incr(dist[here]);
						dist[next] = nextDist;
						q.emplace(next);
					}
					else if (Sign(dist[here]) != Sign(dist[next]))
						return abs(dist[here]) + abs(dist[next]) - 1;
				}
	}
	return -1;
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

		memset(dist, 0, sizeof(dist));

		vector<int> finishVec(N, 3);
		vector<int> startVec(N, -1);
		for (int pillar = 0; pillar < 4; ++pillar)
		{
			int count;
			cin >> count;
			for (int i = 0; i < count; ++i)
			{
				int disk;
				cin >> disk;
				startVec[disk - 1] = pillar;
			}
		}

		finish = Vec2Int(finishVec);
		start = Vec2Int(startVec);
		cout << BFS() << "\n";
	}
	return 0;
}
