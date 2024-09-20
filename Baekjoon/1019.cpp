/*
* Problem: https://www.acmicpc.net/problem/1019
* 분류: 수학, 구현
*
* 숫자의 각 자리를 순회하며, 현재, 이전, 이후 자리들의 숫자를 통해 0~9까지의 수 각각이 얼마나 나오는지를 알아냈다.
* 
*/
#include <iostream>
#include <vector>
#include <string>

using namespace std;

string number;
long long cnt[10];

void CalcNumberCount()
{
	for (int i = 0; i <= 9; ++i)
		cnt[i] = 0;

	int repeat = 0, amount = 1;
	const char* cursor = number.c_str();

	for (int i = 0; i < number.size() - 1; ++i)
		amount *= 10;

	for (int i = 0; i < number.size(); ++i)
	{
		int digitNum = number[i] - '0';
		for (int num = 0; num <= 9; ++num)
		{
			int newRepeat = (num == 0 ? repeat - 1 : repeat);
			if (digitNum > num)
			{
				cnt[num] += amount * (newRepeat + 1);
			}
			else if (digitNum == num)
			{
				if (*(cursor + 1) == 0) cnt[num] += newRepeat + 1;
				else cnt[num] += amount * newRepeat + stoi(cursor + 1) + 1;
			}
			else
			{
				cnt[num] += amount * newRepeat;
			}
		}

		repeat *= 10;
		repeat += digitNum;
		amount /= 10;
		cursor++;
	}
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> number;
	CalcNumberCount();

	for (int i = 0; i <= 9; ++i)
		cout << cnt[i] << " ";
	cout << "\n";

	return 0;
}