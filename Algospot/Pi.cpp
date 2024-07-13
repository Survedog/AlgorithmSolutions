#include <iostream>
#include <string>
#include <algorithm>
#include <memory.h>
#include <cassert>

using namespace std;

enum class PieceType { AllSame, OneSameDiff, ZigZag, NSameDiff, Other };

string numStr;
int memo[10000];
int difficulty[5] = { 1, 2, 4, 5, 10 };

PieceType CheckPieceType(int increment, int lastIncrement)
{
	if (increment == lastIncrement)
	{
		if (increment == 0)
		{
			return PieceType::AllSame;
		}
		else if (increment == 1 || increment == -1)
		{
			return PieceType::OneSameDiff;
		}
		else
		{
			return PieceType::NSameDiff;
		}
	}
	else if (increment + lastIncrement == 0)
	{
		return PieceType::ZigZag;
	}

	return PieceType::Other;
}

int CalcDifficulty(int start, int end)
{
	assert(end - start >= 2);
	PieceType lastType = CheckPieceType(numStr[start + 2] - numStr[start + 1], numStr[start + 1] - numStr[start]);

	int lastIncrement = numStr[start + 2] - numStr[start + 1];
	for (int cur = start + 2; cur < end; ++cur)
	{
		const int increment = numStr[cur + 1] - numStr[cur];

		PieceType curType = CheckPieceType(increment, lastIncrement);
		if (curType == PieceType::Other || curType != lastType)
			return difficulty[static_cast<int>(PieceType::Other)];

		lastIncrement = increment;
		lastType = curType;
	}

	return difficulty[static_cast<int>(lastType)];
}

int GetMinDifficulty(int start)
{
	if (start >= numStr.size()) return 0;

	int& ret = memo[start];
	if (ret >= 0) return ret;

	ret = 987654321;
	for (int offset = 3; offset <= 5 && start + offset <= numStr.size(); offset++)
	{
		const int next = start + offset;
		ret = min(ret, GetMinDifficulty(next) + CalcDifficulty(start, next - 1));
	}

	return ret;
}

int main()
{
	int C;
	cin >> C;
	while (C--)
	{
		cin >> numStr;
		memset(memo, -1, sizeof(memo));
		cout << GetMinDifficulty(0) << "\n";
	}
	return 0;
}
