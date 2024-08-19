/*
* Problem: https://algospot.com/judge/problem/read/MEASURETIME
* 삽입 정렬은 배열의 앞 원소부터 순서대로 앞으로 삽입시키며 정렬하는데, 특정 원소를 삽입 시킬 차례가 됐을 때, 해당 원소의 앞에 있던 원소들끼리는 이미 정렬이 되어 있다.
* 또한 앞에 있는 원소들은 초기 배열에서부터 이미 현재 원소의 앞에 있던 원소들이므로, 한 원소를 이동시키는 횟수는 초기 상태의 배열에서 자신 앞에 있는 원소들 중 자신보다 더 큰 원소의 수와 같다.
* 책 해설) A를 앞에서부터 순회하며 현재까지 만난 각 숫자들이 등장한 횟수를 펜 윅 트리에 저장한 뒤, 이를 이용해 현재 a보다 큰 모든 원소의 개수를 구한다. (tree.sum(999999) - tree.sum(a)를 구하면 된다)
* 
*/
#include <iostream>
#include <vector>

using namespace std;

int N;

struct FenwickTree
{
	vector<int> tree;
	FenwickTree(int n) : tree(n + 1) {}

	int sum(int pos)
	{
		pos++;
		int ret = 0;
		while (pos > 0)
		{
			ret += tree[pos];
			pos &= pos - 1;
		}
		return ret;
	}

	void add(int pos, int val)
	{
		pos++;
		while (pos < tree.size())
		{
			tree[pos] += val;
			pos += (pos & -pos);
		}
	}
};

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> N;
		FenwickTree tree(1000000);
		int sum = 0;
		for (int i = 0; i < N; ++i)
		{
			int a;
			cin >> a;
			sum += tree.sum(999999) - tree.sum(a);
			tree.add(a, 1);
		}
		cout << sum << "\n";
	}
	return 0;
}