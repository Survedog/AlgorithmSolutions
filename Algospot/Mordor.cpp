/*
* Problem: https://algospot.com/judge/problem/read/MORDOR
* N이 최대 10만, Q가 최대 1만이기 때문에, 배열을 일일이 순회하는 시간 복잡도가 O(N)인 알고리즘은 사용할 수 없다. (N * Q가 10억을 넘어서므로 O(NQ)는 시간 초과가 발생할 확률이 높음)
* 하지만 O(logN)의 시간복잡도를 가지는 구간 트리는 충분히 사용할 수 있으므로, 구간의 최대값, 최소값을 구하는 구간트리를 각각 만들어 구간의 최대값 - 최소값을 구해 해결했다.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int N, Q, h[100000];
vector<int> maxTree, minTree;

int InitMaxTree(int left, int right, int node)
{
	int& ret = maxTree[node];
	if (left == right) return ret = h[left];

	int mid = (left + right) / 2;
	ret = max(InitMaxTree(left, mid, node * 2),
		InitMaxTree(mid + 1, right, node * 2 + 1));
	return ret;
}

int InitMinTree(int left, int right, int node)
{
	int& ret = minTree[node];
	if (left == right) return ret = h[left];

	int mid = (left + right) / 2;
	ret = min(InitMinTree(left, mid, node * 2),
		InitMinTree(mid + 1, right, node * 2 + 1));
	return ret;
}

int QueryMaxTree(int left, int right, int node, int nodeLeft, int nodeRight)
{
	if (right < nodeLeft || left > nodeRight) return INT_MIN;
	if (left <= nodeLeft && right >= nodeRight) return maxTree[node];

	int nodeMid = (nodeLeft + nodeRight) / 2;
	return max(QueryMaxTree(left, right, node * 2, nodeLeft, nodeMid),
		QueryMaxTree(left, right, node * 2 + 1, nodeMid + 1, nodeRight));
}

int QueryMinTree(int left, int right, int node, int nodeLeft, int nodeRight)
{
	if (right < nodeLeft || left > nodeRight) return INT_MAX;
	if (left <= nodeLeft && right >= nodeRight) return minTree[node];

	int nodeMid = (nodeLeft + nodeRight) / 2;
	return min(QueryMinTree(left, right, node * 2, nodeLeft, nodeMid),
		QueryMinTree(left, right, node * 2 + 1, nodeMid + 1, nodeRight));
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> N >> Q;
		for (int i = 0; i < N; ++i)
			cin >> h[i];

		maxTree.resize(4 * N);
		minTree.resize(4 * N);
		InitMaxTree(0, N - 1, 1);
		InitMinTree(0, N - 1, 1);

		for (int i = 0; i < Q; ++i)
		{
			int a, b;
			cin >> a >> b;
			cout << QueryMaxTree(a, b, 1, 0, N - 1) - QueryMinTree(a, b, 1, 0, N - 1) << "\n";
		}
	}
	return 0;
}