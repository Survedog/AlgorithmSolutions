/*
* Problem: https://algospot.com/judge/problem/read/GENIUS
* 처음엔 memo의 값을 k분 30초에 특정 노래가 재생되고 있을 확률로 정의했으나, 이 경우 점화식을 어떻게 세워야할 지 구상해내지 못함.
* 때문에 memo의 값을 k분에 특정 노래가 시작될 확률로 바꿔 정의하고, 점화식 memo[song][minute] = memo[prevSong][minute - L[prevSong]] * T[prevSong][curSong]을 세웠다.
* 이때 k는 최대 1000000까지이므로, 이를 memo의 두번째 차원의 인덱스로 사용하면 메모리 초과가 발생한다.
* 점화식에선 memo의 두 번째 차원에 대해 최소 minute - 4까지만 접근하고, 접근 순서는 고정되어 있으므로 슬라이딩 윈도우 기법을 적용해 메모리 사용량을 줄일 수 있다.
* 두 번째 차원의 크기는 6으로 설정하고, 두 번째 인덱스는 6으로 나눈 나머지로 접근함으로써 슬라이딩 윈도우 기법을 적용시켰다. (필요한 크기 5 + 여유 크기 1)
*/

#include <stdio.h>
#include <memory.h>
#include <algorithm>

using namespace std;

int n, k, m, L[50];
double T[50][50], memo[50][6];

void CalcProbability()
{
	memo[0][0] = 1.0;
	for (int song = 1; song < n; song++)
		memo[song][0] = 0.0;

	for (int minute = 1; minute <= k; minute++)
	{
		for (int curSong = 0; curSong < n; ++curSong)
		{
			double& ret = memo[curSong][minute % 6] = 0.0;
			for (int prevSong = 0; prevSong < n; ++prevSong)
			{
				if (minute >= L[prevSong])
				{
					ret += memo[prevSong][(minute - L[prevSong]) % 6] * T[prevSong][curSong];
				}
			}
		}
	}
}

int main()
{
	int C;
	scanf("%d", &C);
	while (C--)
	{
		fill_n(&memo[0][0], 50 * 6, -1.0);

		scanf("%d%d%d", &n, &k, &m);
		for (int i = 0; i < n; ++i)
			scanf("%d", &L[i]);

		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				scanf("%lf", &T[i][j]);

		CalcProbability();
		for (int i = 0; i < m; ++i)
		{
			int fav;
			scanf("%d", &fav);

			double probability = 0.0;
			for (int playTime = 0; playTime < L[fav] && k >= playTime; playTime++)
				probability += memo[fav][(k - playTime) % 6];
			printf("%.8lf ", probability);
		}
		printf("\n");
	}

	return 0;
}