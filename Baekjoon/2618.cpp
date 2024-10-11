/*
* Problem: https://www.acmicpc.net/problem/2618 (경찰차)
* 분류: 동적계획법
* 
* 경찰차의 위치는 반드시 시작점 혹은 사건이 일어난 장소이므로, 메모이제이션 시 행, 열 번호 대신 사건이나 시작점에 대한 인덱스로 경찰차의 위치를 표현하면 메모리 공간을 절약할 수 있다.
* 처음엔 경찰차1, 2의 위치 및 다음 사건의 번호를 메모이제이션 하는 동적계획법을 수행하려 했으나, 세 값의 최대 크기가 너무 커 시간과 메모리 공간이 부족할 것이라 생각했다.
* 이때 매 반복에선 바로 이전 사건까지의 메모만 필요한다는 사실을 이용, 슬라이딩 윈도우 기법을 이용한 반복적 동적계획법으로 최소 거리를 알아내는 것엔 성공했다.
* 하지만 슬라이딩 윈도우 기법을 사용한 뒤 각 단계에서 했던 선택을 알아내는 방법을 찾지 못했다.
* 
* 해결 방법) https://yabmoons.tistory.com/644를 참고함.
* 다음 사건 번호는 각 경찰차가 마지막으로 처리한 사건의 번호만 알아도 구할 수 있으므로, 사실상 두 경찰차의 최근 처리 사건 번호만 가지고도 메모이제이션을 수행할 수 있다.
* 따라서 일반적인 재귀함수식 동적계획법을 사용했고, 메모와 마찬가지로 각 단계에서의 선택 또한 2차원 배열에 저장해 답을 구한 후 각 선택들을 복원할 수 있었다.
* 
* 알게된 것) 슬라이딩 윈도우 방식을 사용할 땐 각 단계에서의 선택을 복원하기 어려우므로, 이러한 경우 최대한 재귀함수를 통한 방법을 생각해보자.
*/
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int N, W;
vector<pair<int, int>> accidents;
vector<vector<int>> memo, choice;

inline int CalcDist(int a, int b)
{
	return abs(accidents[a].first - accidents[b].first) + abs(accidents[a].second - accidents[b].second);
}

int DP(int carA, int carB)
{
	int& ret = memo[carA][carB];
	if (ret != -1) return ret;

	int acc = max(carA, carB) + 1;
	if (acc == W + 2) return ret = 0;

	int aRet = DP(acc, carB) + CalcDist(acc, carA);
	int bRet = DP(carA, acc) + CalcDist(acc, carB);
	if (aRet < bRet)
	{
		ret = aRet;
		choice[carA][carB] = 1;
	}
	else
	{
		ret = bRet;
		choice[carA][carB] = 2;
	}
	return ret;
}

//int DP()
//{
//	memo[0][2] = CalcDist(1, 2);
//	memo[2][1] = CalcDist(0, 2);
//
//	for (int acc = 3; acc < W + 2; ++acc)
//	{
//		for (int carA = 0; carA < acc - 1; ++carA)
//		{
//			if (carA == 1) continue;
//
//			for (int carB = 1; carB < acc; ++carB)
//			{
//				if (carA == acc - 1 || carB == acc - 1)
//				{
//					int aToAcc = CalcDist(carA, acc);
//					int bToAcc = CalcDist(carB, acc);
//					
//					if (memo[acc][carB] > memo[carA][carB] + aToAcc)
//					{
//						memo[acc][carB] = memo[carA][carB] + aToAcc;
//						choice[carA][carB] = make_pair(0, acc);
//					}
//					
//					if (memo[carA][acc] > memo[carA][carB] + bToAcc)
//					{
//						memo[carA][acc] = memo[carA][carB] + bToAcc;
//						choice[carA][carB] = make_pair(1, acc);
//					}
//				}
//				else
//					memo[carA][carB] = INF;
//			}
//		}
//	}
//
//	int ret = INF;
//	for (int carA = 0; carA < W + 1; ++carA)
//		ret = min(ret, memo[carA][W + 1]);
//
//	for (int carB = 1; carB < W + 1; ++carB)
//		ret = min(ret, memo[W + 1][carB]);
//
//	return ret;
//}

void Reconstruct()
{
	int carA = 0, carB = 1;
	while (choice[carA][carB] != -1)
	{
		int acc = max(carA, carB) + 1;
		cout << choice[carA][carB] << "\n";
		if (choice[carA][carB] == 1)
			carA = acc;
		else
			carB = acc;
	}
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> W;
	accidents.emplace_back(0, 0);
	accidents.emplace_back(N - 1, N - 1);
	for (int i = 0; i < W; ++i)
	{
		int r, c;
		cin >> r >> c;
		accidents.emplace_back(r - 1, c - 1);
	}

	memo.resize(W + 2, vector<int>(W + 2, -1));
	choice.resize(W + 2, vector<int>(W + 2, -1));

	cout << DP(0, 1) << "\n";
	Reconstruct();
	return 0;
}