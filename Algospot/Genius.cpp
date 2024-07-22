/*
* Problem: https://algospot.com/judge/problem/read/GENIUS
* 처음엔 memo의 값을 k분 30초에 특정 노래가 재생되고 있을 확률로 정의했으나, 이 경우 점화식을 어떻게 세워야할 지 구상해내지 못함.
* 때문에 memo의 값을 k분에 특정 노래가 시작될 확률로 바꿔 정의하고, 점화식 'memo[song][minute] = 모든 prevSong에 대한 (memo[prevSong][minute - L[prevSong]] * T[prevSong][curSong])의 합'을 세웠다.
* 이때 k는 최대 1000000까지이므로, 이를 memo의 두번째 차원의 인덱스로 사용하면 메모리 초과가 발생한다.
* 점화식에선 memo의 두 번째 차원에 대해 최소 minute - 4까지만 접근하고, 접근 순서는 고정되어 있으므로 슬라이딩 윈도우 기법을 적용해 메모리 사용량을 줄일 수 있다.
* 두 번째 차원의 크기는 6으로 설정하고, 두 번째 인덱스는 6으로 나눈 나머지로 접근함으로써 슬라이딩 윈도우 기법을 적용시켰다. (필요한 크기 5 + 여유 크기 1)
* 그러나 이 알고리즘의 시간 복잡도는 O(k * n^2)로, 현재 문제에선 시간 초과가 발생한다.
* 
* (책 해답) 문제에 반복적 동적계획법을 사용할 수 있고 점화식이 선형 변환의 형태를 띄므로, 점화식을 행렬 곱 형태로 변화한 뒤 행렬의 빠른 제곱을 사용하는 방식으로 확률을 계산하면 시간 복잡도를 줄일 수 있다.
* 4n * 1 크기의 행렬 C_minute에 각 노래가 (minute - 3)에, (minute - 2)에, (minute - 1)에, minute에 실행될 확률을 순서대로 저장하면 C_minute+1 = W * C_minute의 형태로 확률을 구할 수 있다.
* 이를 위해 T를 이용해 W를 계산한 후, 빠른 행렬 제곱 알고리즘을 사용해 W의 k제곱을 구하였다. 이를 C_0에 곱하면 되는데, C_0은 3n번째 원소의 값 1.0을 제외하곤 모든 원소의 값이 0.0이다.
* 따라서 W^k의 i행 3n열의 값은 i번째 노래가 k분부터 재생될 확률을 나타낸다. 이를 사용해 결과를 구할 수 있다.
*/

#include <stdio.h>
#include <memory.h>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;

class SquareMatrix
{
public:
	SquareMatrix(int inSize) : size(inSize), matrix(inSize, vector<double>(inSize, 0.0))
	{
	}

	int size;
	vector<vector<double>> matrix;

	static SquareMatrix Identity(int inSize)
	{
		SquareMatrix mat(inSize);
		for (int i = 0; i < inSize; ++i)
			mat.matrix[i][i] = 1.0;
		return mat;
	}

	SquareMatrix operator*(const SquareMatrix& other) const
	{
		assert(size == other.size);
		SquareMatrix ret(size);
		for (int i = 0; i < size; ++i)
			for (int j = 0; j < size; ++j)
				for (int k = 0; k < size; ++k)
					ret.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
		return ret;
	}

	SquareMatrix Pow(int num)
	{
		if (num == 0) return Identity(size);

		if (num % 2 == 1) return Pow(num - 1) * (*this);
		SquareMatrix half = Pow(num / 2);
		return half * half;
	}
};

int n, k, m, L[50];
double T[50][50], memo[50][6];

vector<double> CalcProbability(const vector<int>& target)
{
	SquareMatrix W(4 * n);
	for (int i = 0; i < 3 * n; ++i)
		W.matrix[i][i + n] = 1.0;

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			W.matrix[3 * n + i][(4 - L[j]) * n + j] = T[j][i];

	SquareMatrix Wk = W.Pow(k);
	vector<double> ret(target.size(), 0.0);
	for (int i = 0; i < target.size(); ++i)
	{
		int song = target[i];
		for (int playTime = 0; playTime < L[song]; playTime++)
			ret[i] += Wk.matrix[(3 - playTime) * n + song][3 * n];
	}

	return ret;
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

		vector<int> target(m);
		for (int i = 0; i < m; ++i)
			scanf("%d", &target[i]);

		for (double prob : CalcProbability(target))
			printf("%.8lf ", prob);
		printf("\n");
	}

	return 0;
}