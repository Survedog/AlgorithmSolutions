#include <iostream>
#include <string>
#include <cassert>

// Problem: https://algospot.com/judge/problem/read/QUADTREE

using namespace std;

int C = 0;
int ReversedOrder[4] = { 3, 4, 1, 2 };

int GetPartialImageEnd(const string& Image, int Start)
{
	assert(Image.length() > Start);
	if (Image[Start] == 'x')
	{
		Start++;
	}
	else
	{
		return Start;
	}

	int EndIdx, PartCount = 1;
	for (EndIdx = Start; EndIdx < Image.length() && PartCount <= 4; PartCount++)
	{
		if (Image[EndIdx] == 'x')
		{
			EndIdx = GetPartialImageEnd(Image, EndIdx) + 1;
		}
		else
		{
			EndIdx++;
		}
	}
	return EndIdx - 1;
}

void ReverseImage(const string& OriginalImage, const int Low, const int High)
{
	if (OriginalImage[Low] == 'x')
	{
		cout << 'x';
	}
	else
	{
		cout << OriginalImage[Low];
		return;
	}

	int PartStartIdxes[6] = { 0, };
	PartStartIdxes[0] = Low;
	PartStartIdxes[1] = Low + 1;
	PartStartIdxes[5] = High + 1;

	for (int Part = 2; Part < 5; Part++)
	{
		PartStartIdxes[Part] = GetPartialImageEnd(OriginalImage, PartStartIdxes[Part - 1]) + 1;
		assert(PartStartIdxes[Part] <= High);
	}

	for (int Idx = 0; Idx < 4; Idx++)
	{
		const int PartStart = PartStartIdxes[ReversedOrder[Idx]];
		const int PartEnd = PartStartIdxes[ReversedOrder[Idx] + 1] - 1;
		ReverseImage(OriginalImage, PartStart, PartEnd);
	}
}

int main()
{
	cin >> C;
	for (int i = 0; i < C; ++i)
	{
		string OriginalImage;
		cin >> OriginalImage;

		ReverseImage(OriginalImage, 0, OriginalImage.length() - 1);
		cout << "\n";
	}
	return 0;
}