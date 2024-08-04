/*
* Problem: https://algospot.com/judge/problem/read/GRADUATION
*/
#include <iostream>
#include <memory.h>
#include <algorithm>

using namespace std;

int N, K, M, L, prereq[12], open[10];
int memo[10][1 << 20], prereqSet[1 << 20];

inline int GetSubjectCount(int subjects)
{
	return __popcnt(subjects);
}

inline int GetLSBSubject(int subjects)
{
	unsigned long lsb;
	_BitScanForward(&lsb, subjects);
	return lsb;
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
	if (taken == (1 << 20) - 1 || semesterLeft == 0 || semester == 10) return 0;

	int& ret = memo[semester][taken];
	if (ret != -1) return ret;

	for (int subset = open[semester]; subset; subset = ((subset - 1) & open[semester]))
	{
		int newSubjects = (~taken) & subset;
		int newSubjectCount = GetSubjectCount(newSubjects);
		if (newSubjectCount > 0 && newSubjectCount <= L && ((prereqSet[newSubjects] & (~taken)) == 0))
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
		for (int subjectSet = (1 << 20) - 1; subjectSet >= 0; subjectSet--)
			GetPrerequisiteSet(subjectSet);

		bool isPossible = false;
		for (int semesterLeft = 1; semesterLeft <= M; ++semesterLeft)
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