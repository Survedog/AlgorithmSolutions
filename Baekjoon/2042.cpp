/*
* Problem: https://www.acmicpc.net/problem/2042
* 분류: 펜윅 트리 (구간 트리)
*/
#include <iostream>
#include <vector>

using namespace std;

int N, M, K;
long long nums[1000001];

struct FenwickTree
{
	vector<long long> tree;
	FenwickTree(int n) : tree(n + 1) {}

	long long Sum(int pos)
	{
		long long ret = 0;
		while (pos > 0)
		{
			ret += tree[pos];
			pos &= pos - 1;
		}
		return ret;
	}

	inline void Set(int pos, long long val)
	{
		Add(pos, val - nums[pos]);
	}

	void Add(int pos, long long val)
	{
		while (pos < tree.size())
		{
			tree[pos] += val;
			pos += (pos & -pos);
		}
	}
};

FenwickTree fwTree(0);

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> M >> K;
	fwTree = FenwickTree(N);
	for (int i = 1; i <= N; ++i)
	{
		cin >> nums[i];
		fwTree.Add(i, nums[i]);
	}

	for (int i = 0; i < M + K; ++i)
	{
		long long a, b, c;
		cin >> a >> b >> c;
		if (a == 1)
		{
			fwTree.Set(b, c);
			nums[b] = c;
		}
		else
		{
			cout << fwTree.Sum(c) - fwTree.Sum(b - 1) << "\n";
		}
	}
	return 0;
}

