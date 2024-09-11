/*
* Problem: https://www.acmicpc.net/problem/1014
* 분류: 이분 매칭
* 
* (https://jaeseo0519.tistory.com/126?category=1071535를 참고함.)
* 책상들을 열에 따라 2개의 집합으로 나눈 뒤, 서로 양립할 수 없는(동시에 학생이 앉을 수 없는) 책상들을 간선으로 연결한다. (이때 부서진 책상은 간선으로 연결하지 않음)
* 이렇게 만든 그래프에서 책상들은 오직 좌우 열의 책상과만 연결되므로 이 그래프는 이분 그래프가 된다.
* 이제 이 그래프에서 정점들이 최대한 양립하기 위해선 최소한의 정점들만을 제거하여 간선들을 모두 지우면 된다. 즉, 최소 정점 덮개에 포함되는 정점을 제거하면 된다.
* 쾨닉의 정리에 의해 이분 그래프에서 최대 이분 매칭의 개수는 최소 정점 덮개의 개수와 동일하므로, 포드-풀커슨의 원리를 이용한 이분 매칭 알고리즘을 통해 최소 정점 덮개 개수를 구한다.
* 최소 정점 덮개의 개수를 앉을 수 있는 모든 책상의 개수에서 빼주면 학생을 배정할 수 있는 최대 자리 수를 구할 수 있다.
*/
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

const int dx[] = { -1, -1, -1, 1, 1, 1 }, dy[] = { 1, 0, -1, 1, 0, -1 };
int N, M, A, B, nodeNum[50][50];
char room[10][10];
vector<vector<bool>> adjacent;
vector<int> aMatch, bMatch;
vector<bool> visited;

inline bool IsInRange(int y, int x) { return y >= 0 && y < N && x >= 0 && x < M; }

bool DFS(int a)
{
	if (visited[a]) return false;
	visited[a] = true;

	for (int b = 0; b < B; ++b)
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

int GetMaxBipartiteMatch()
{
	int ret = 0;
	aMatch = vector<int>(A, -1);
	bMatch = vector<int>(B, -1);

	for (int a = 0; a < A; ++a)
	{
		visited = vector<bool>(A, false);
		if (DFS(a)) ret++;
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

		int broken = 0;
		A = B = 0;
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < M; ++j)
			{
				cin >> room[i][j];
				if (room[i][j] == 'x') broken++;
				nodeNum[i][j] = (j % 2 == 0) ? A++ : B++;
			}

		adjacent = vector<vector<bool>>(A, vector<bool>(B, false));
		for (int y = 0; y < N; ++y)
			for (int x = 0; x < M; x += 2)
				if (room[y][x] == '.')
					for (int k = 0; k < 6; ++k)
					{
						int nx = x + dx[k];
						int ny = y + dy[k];

						if (!IsInRange(ny, nx)) continue;

						if (room[ny][nx] == '.')
							adjacent[nodeNum[y][x]][nodeNum[ny][nx]] = true;
					}

		int answer = N * M - broken - GetMaxBipartiteMatch();
		cout << answer << "\n";
	}

	return 0;
}