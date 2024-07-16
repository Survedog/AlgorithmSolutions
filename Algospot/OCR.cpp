/*
* Problem: http://algospot.com/judge/problem/read/OCR
* 원문이 Q, 인식된 문장이 R일 때 베이즈 정리로 인해 P(Q|R) = P(R|Q)*P(Q)/P(R)
* 따라서 P(R|Q)*P(Q)가 최대화되는 Q가 가장 원문일 확률이 높은 문장이 된다.
* 이때 확률을 곱할 시 실수 곱 대신 log 값의 합을 사용해 언더플로우를 방지한다. (값을 log하기 전과 후의 대소 관계는 변하지 않으므로)
* 이 문제와 같이 마르코프 연쇄를 통해 데이터(원문)가 생성되고, 해당 데이터를 직접 보지 못하며 오류가 있는 관찰자(인식된 문장)를 통해서만 관찰할 수 있는 문제를 은닉 마르코프 모델(HMM)이라고 한다.
* 또한 이 풀이는 비터비 알고리즘이라고 하여, HMM의 가장 가능성 높은 실제 상태를 계산하는 알고리즘이다. (출처: 프로그래밍 대회에서 배우는 알고리즘 문제해결 전략 (저자-구종만, 출판-인사이트))
* 
* memo를 negInf로 초기화하고 memo가 negInf + 0.5보다 클 경우 메모이제이션을 적용하니 시간 초과가 발생함. 반면 memo를 1로 초기화하니 시간초과가 발생하지 않음.
* memo negInf보다 작은 값이 메모되었기 때문으로 추정함. 따라서 메모 값을 초기화할 때는 초기화 값이 실제 로직에서 나오지 않는지 반드시 검증해야 함.
*/

#include <stdio.h>
#include <memory.h>
#include <cstring>
#include <algorithm>
#include <cmath>

const double negInf = -1e200;
int m, q, n;
char word[501][11];
double nextProb[501][501], recogProb[501][501], memo[102][502];
int sentence[100], choice[102][502];

double Recognize(int pos, int lastWord)
{
	if (pos == n)
		return 0.0;

	double& ret = memo[pos][lastWord];
	if (ret < 0.9)
		return ret;

	ret = negInf;
	for (int candi = 1; candi <= m; candi++)
	{
		double candiProb = Recognize(pos + 1, candi) + nextProb[lastWord][candi] + recogProb[candi][sentence[pos]];
		if (ret < candiProb)
		{
			ret = candiProb;
			choice[pos][lastWord] = candi;
		}
	}

	return ret;
}

void Reconstruct(int start, int lastWord)
{
	if (start == n)
	{
		printf("\n");
		return;
	}

	int chosen = choice[start][lastWord];
	printf("%s ", word[chosen]);
	Reconstruct(start + 1, chosen);
}

int main()
{
	scanf("%d%d", &m, &q);

	for (int i = 1; i <= m; ++i)
		scanf("%s", word[i]);

	for (int i = 1; i <= m; ++i)
	{
		scanf("%lf", &nextProb[0][i]);
		nextProb[0][i] = std::log(nextProb[0][i]);
	}

	for (int i = 1; i <= m; ++i)
	{
		for (int j = 1; j <= m; ++j)
		{
			scanf("%lf", &nextProb[i][j]);
			nextProb[i][j] = std::log(nextProb[i][j]);
		}
	}

	for (int i = 1; i <= m; ++i)
	{
		for (int j = 1; j <= m; ++j)
		{
			scanf("%lf", &recogProb[i][j]);
			recogProb[i][j] = std::log(recogProb[i][j]);
		}
	}

	for (int i = 0; i < q; ++i)
	{
		scanf("%d", &n);
		for (int j = 0; j < n; ++j)
		{
			char temp[11] = { 0 };
			scanf("%s", temp);

			for (int k = 1; k <= m; ++k)
			{
				if (strcmp(word[k], temp) == 0)
				{
					sentence[j] = k;
					break;
				}
			}
		}

		std::fill_n(&memo[0][0], 102 * 502, 1);
		memset(choice, -1, sizeof(choice));

		Recognize(0, 0);
		Reconstruct(0, 0);
	}
	return 0;
}