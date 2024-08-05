/*
* Problem: https://algospot.com/judge/problem/read/GRADUATION
* 시도1) semester번째 학기 전까지 taken 과목들을 미리 수강했고, 앞으로 semesterLeft만큼의 학기를 더 신청할 수 있을 때 수강할 수 있는 최대 과목 수를 구하는 함수를 만들었다.
* 초기 semesterLeft를 0부터 10까지 올리며 이 함수를 실행했고, 만약 수강 가능한 최대 과목 수가 K보다 크거나 같게 되면 그 semesterLeft를 답으로 출력하도록 했다.
* 이때 과목들의 집합은 모두 int형 변수에 비트마스크로 나타낸 덕분에 taken에 선행 과목이 모두 포함되는지 확인하는 과정을 상수 시간에 수행할 수 있었다.
* 또한 현재 학기에 수강할 수 있는 과목들의 부분집합을 구할 때는 교재 p.585에서 소개한 비트마스크의 부분집합 순회 방법을 사용했다.
* 예제는 맞았으나 실제 채점 시 오답이 발생했다.
* 
* 책 해설) semester번째 학기 전까지 taken 과목들을 미리 수강했을 때 앞으로 몇 학기를 더 신청해야 K 이상의 과목을 수강할 수 있는지 구하는 함수를 사용한다.
* 이때 현재 학기에서 수강할 수 있는 과목들의 부분집합을 세기 전, 이미 들었던 과목과 선수과목을 수강하지 않은 과목을 제외해 부분 집합 수를 줄인다.
* 해당 알고리즘을 사용하니 오답이 발생하지 않았다.
* 
* 개선점) 최소 학기 수를 동적계획법으로 바로 구할 수 있는데, 그 방법을 깊게 고민하지 않고 불가능하다 여겨 최대 과목 수를 구하는 간접적인 방법을 대신 사용하였다.
* 결과적으로 코드가 복잡해지고 알 수 없는 오답에 시달리게 되었다. 앞으론 더 직관적인 알고리즘을 사용할 수 있을지 더 고민해봐야겠다.
*/
#include <iostream>
#include <memory.h>
#include <algorithm>

using namespace std;

const int INF = 987654321;
int N, K, M, L, prereq[12], open[10];
int memo[10][1 << 12];

inline int GetSubjectCount(int subjects)
{
	return __builtin_popcount(subjects);
}

inline int GetLSBSubject(int subjects)
{
	return __builtin_ctz(subjects);
}

int GetMinSemesters(int semester, int taken, int takenCount)
{
	if (takenCount == K) return 0;
	if (semester == M) return INF;

	int& ret = memo[semester][taken];
	if (ret != -1) return ret;

	int available = open[semester] & (~taken);
	for (int subject = 0; subject < N; ++subject)
	{
		if ((available & (1 << subject)) && (taken & prereq[subject]) != prereq[subject])
			available ^= 1 << subject;
	}

	ret = INF;
	for (int subset = available; subset; subset = ((subset - 1) & available))
	{
		int newSubjectCount = GetSubjectCount(subset);
		if (newSubjectCount <= L)
			ret = min(ret, GetMinSemesters(semester + 1, taken | subset, takenCount + newSubjectCount) + 1);
	}
	ret = min(ret, GetMinSemesters(semester + 1, taken, takenCount));

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
		cin >> N >> K >> M >> L;
		for (int i = 0; i < N; ++i)
		{
			int preCount, pre = 0;
			cin >> preCount;

			for (int j = 0; j < preCount; ++j)
			{
				int subject;
				cin >> subject;
				pre |= (1 << subject);
			}
			prereq[i] = pre;
		}

		for (int i = 0; i < M; ++i)
		{
			int subjectCount, subjects = 0;
			cin >> subjectCount;

			for (int j = 0; j < subjectCount; ++j)
			{
				int subject;
				cin >> subject;
				subjects |= (1 << subject);
			}
			open[i] = subjects;
		}

		memset(memo, -1, sizeof(memo));
		int result = GetMinSemesters(0, 0, 0);
		if (result < INF) cout << result << "\n";
		else cout << "IMPOSSIBLE" << "\n";
	}
	return 0;
}