/*
* Problem: https://www.acmicpc.net/problem/14517
* 분류: 반복적 동적계획법
* 
* https://banwolcode.tistory.com/58 <- 이 블로그 게시물을 참고함.
* memo[i][j]를 문자열 S의 부분 문자열 S[i...j]의 팰린드롬인 부분 수열의 개수로 두고, 반복적 동적계획법을 수행해 전체 문자열에 대한 답을 구했다.
* len = j - i + 1 (i <= j)라 할 때, memo[i][j]는 다음의 점화식을 따른다.
* 1) len = 1일때: memo[i][j] = 1
* 2) len = 2일때: memo[i][j] = S[i]와 S[j]가 같다면 3, 아니면 2
* 3) len >= 3일때: S[i]와 S[j]가 다르다면 memo[i][j] = memo[i+1][j] + memo[i][j-1] - memo[i+1][j-1]
*                 (중복되는 부분을 제거하기 위해 memo[i+1][j-1]을 뺐다.)
*				  S[i]와 S[j]가 같다면 memo[i][j] = memo[i+1][j] + memo[i][j-1] - memo[i+1][j-1] + memo[i+1][j-1] + 1 = memo[i+1][j] + memo[i][j-1] + 1
				  (S[i]와 S[j]가 같을 때, S[i+1...j-1]의 팰린드롬인 부분 수열을 S[i], S[j] 사이에 끼워넣으면 새로운 팰린드롬 부분 수열이 되므로 memo[i+1][j-1] 만큼의 팰린드롬이 더 존재하고,
                   {S[i], S[j]} 또한 팰린드롬인 부분 수열이기 때문에 총 memo[i+1][j-1] + 1개를 더했다.)
* 위 점화식을 토대로 len을 1부터 S의 크기까지 올려가며 반복적 동적계획법을 수행해 답을 구할 수 있었다.
* 
* 배운점) C++의 mod연산은 음수 값에 대해 수행될 경우 음수 나머지를 반환하기 때문에 (예를 들어 -6 % 5 = -1),
* 이 문제와 같이 특정 값에 대한 MOD 값을 구하는 문제에서 만약 MOD 값 사이의 뺄셈이 존재한다면 결과 값이 음수가 되는지 꼭 확인해야 한다.
* 만약 음수가 되어선 안되는 상황이라면 MOD 연산 전에 DIVIDER를 더해 이를 막을 수 있다.
*/
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int DIVIDER = 10007;
vector<vector<int>> memo;
string S;

int CountPalindromes()
{
	memo[0][0] = 1;
	for (int i = 1; i < S.size(); ++i)
	{
		memo[i][i] = 1;
		memo[i - 1][i] = S[i - 1] == S[i] ? 3 : 2;
	}

	for (int len = 3; len <= S.size(); ++len)
	{
		for (int i = 0; i <= S.size() - len; ++i)
		{
			int j = i + len - 1;
			if (S[i] == S[j])
				memo[i][j] = (memo[i + 1][j] + memo[i][j - 1] + 1) % DIVIDER;
			else
			{
				memo[i][j] = (memo[i + 1][j] + memo[i][j - 1] - memo[i + 1][j - 1] + DIVIDER) % DIVIDER;
			}
		}
	}
	return memo[0][S.size() - 1];
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> S;
	memo.resize(S.size(), vector<int>(S.size(), -1));
	cout << CountPalindromes() << "\n";
	return 0;
}