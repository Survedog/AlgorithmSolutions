/*
* Problem: https://algospot.com/judge/problem/read/ARCTIC
* 해법1) 다익스트라 알고리즘과 유사한 방법을 사용하여 최적화 문제로써 해결하였다. (이전 커밋 참고)
* 0번 기지부터 시작하여 다른 기지들을 가까운 순서대로 순회하며, 해당 기지에서 다른 기지까지의 거리가 indirectDist에 저장된 거리보다 짧다면 indirectDist를 해당 거리로 업데이트하고, 그 기지를 순회 후보에 추가한다.
* 주의할 점은 이때 visited 배열에 순회한 기지를 표시하여 한 기지를 두 번 순회하지 않도록 해야 한다. 이는 새롭게 방문한 기지 B에서 기존에 방문한 기지 A로의 거리가 더 짧다고 indirectDist[A]를 다시 갱신해버리면,
* B까지 가기 위한 거리는 전혀 고려하지 않기 때문에 최종적으론 A의 인접 기지로부터의 최소 거리만 계산되기 때문이다.
*
* 해법2) 최적화 문제를 결정 문제로 바꾼 뒤 이분법으로 근사값을 계산하는 방식으로 해결하였다.
* 즉, 기지들을 모두 연결하는 연락망을 구축 가능한 최소 무전기 반경을 구하는 함수 대신, 특정 반경의 무전기로 연락망을 구축 가능한지 여부를 찾는 함수를 구현하였다.
*
*/
#include <iostream>
#include <cmath>
#include <queue>
#include <memory.h>

using namespace std;

int N;
double xPos[100], yPos[100], distSquared[100][100];

bool Possible(double output)
{
	double indirectDist[100];
	for (int i = 0; i < N; ++i)
		indirectDist[i] = distSquared[0][i];

	bool visited[100];
	memset(visited, false, sizeof(visited));

	queue<int> candi;
	candi.push(0);
	while (!candi.empty())
	{
		int next = candi.front();
		visited[next] = true;
		candi.pop();

		for (int i = 1; i < N; ++i)
		{
			if (!visited[i] && distSquared[next][i] <= output)
			{
				indirectDist[i] = distSquared[next][i];
				candi.push(i);
			}
		}
	}

	for (int i = 0; i < N; ++i)
		if (output < indirectDist[i])
			return false;
	return true;
}

double Optimize(int iter)
{
	double lo = 0, hi = 1000000;
	while (iter--)
	{
		double mid = (lo + hi) / 2.0;
		if (Possible(mid))
			hi = mid;
		else
			lo = mid;
	}
	return sqrt(lo);
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout << fixed;
	cout.precision(2);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> N;
		for (int i = 0; i < N; ++i)
			cin >> xPos[i] >> yPos[i];

		fill_n(&distSquared[0][0], 100 * 100, 0.0);
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
			{
				if (i > j)
					distSquared[i][j] = distSquared[j][i];
				else if (i < j)
					distSquared[i][j] = pow(xPos[i] - xPos[j], 2) + pow(yPos[i] - yPos[j], 2);
			}

		cout << Optimize(100) << "\n";
	}
}