/*
* Problem: https://algospot.com/judge/problem/read/GRADUATION
* 시도1) semester번째 학기 전까지 taken 과목들을 미리 수강했고, 앞으로 semesterLeft만큼의 학기를 더 신청할 수 있을 때 수강할 수 있는 최대 과목 수를 구하는 함수를 만들었다.
* 초기 semesterLeft를 0부터 10까지 올리며 이 함수를 실행했고, 만약 수강 가능한 최대 과목 수가 K보다 크거나 같게 되면 그 semesterLeft를 답으로 출력하도록 했다.
* 이때 과목들의 집합은 모두 int형 변수에 비트마스크로 나타낸 덕분에 taken에 선행 과목이 모두 포함되는지 확인하는 과정을 상수 시간에 수행할 수 있었다.
* 또한 현재 학기에 수강할 수 있는 과목들의 부분집합을 구할 때는 교재 p.585에서 소개한 비트마스크의 부분집합 순회 방법을 사용했다.
* 예제는 맞았으나 실제 채점 시 오답이 발생했다.
* 
*/
#include <iostream>
#include <memory.h>
#include <algorithm>

using namespace std;

int N, K, M, L, prereq[12], open[10];
int memo[10][1 << 20], prereqSet[1 << 20];

inline int GetSubjectCount(int subjects)
{
	return __builtin_popcount(subjects);
}

inline int GetLSBSubject(int subjects)
{
	return __builtin_ctz(subjects);
}

int GetPrerequisiteSet(int subjects)
{
	int& ret = prereqSet[subjects];
	if (ret != -1) return ret;

	if (subjects == 0) return ret = 0;

	int lsbSubject = GetLSBSubject(subjects);
	return ret = GetPrerequisiteSet(subjects & (~(1 << lsbSubject))) | prereq[lsbSubject];
}

int GetMaxSubjects(int semester, int taken, int semesterLeft)
{
	if (taken == (1 << N) - 1 || semesterLeft == 0 || semester == M) return 0;

	int& ret = memo[semester][taken];
	if (ret != -1) return ret;

	int available = open[semester] & (~taken);
	for (int subset = available; subset; subset = ((subset - 1) & available))
	{
		int newSubjectCount = GetSubjectCount(subset);
		if (newSubjectCount > 0 && newSubjectCount <= L && ((prereqSet[subset] & (~taken)) == 0))
			ret = max(ret, GetMaxSubjects(semester + 1, taken | subset, semesterLeft - 1) + newSubjectCount);
	}
	ret = max(ret, GetMaxSubjects(semester + 1, taken, semesterLeft));

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

		memset(prereqSet, -1, sizeof(prereqSet));
		for (int subjectSet = (1 << N) - 1; subjectSet >= 0; subjectSet--)
			GetPrerequisiteSet(subjectSet);

		bool isPossible = false;
		for (int semesterLeft = 0; semesterLeft <= M; ++semesterLeft)
		{
			memset(memo, -1, sizeof(memo));

			if (GetMaxSubjects(0, 0, semesterLeft) >= K)
			{
				isPossible = true;
				cout << semesterLeft << "\n";
				break;
			}
		}

		if (!isPossible)
			cout << "IMPOSSIBLE" << "\n";
	}
	return 0;
}