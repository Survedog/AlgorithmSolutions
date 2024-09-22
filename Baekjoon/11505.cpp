/*
* Problem: https://www.acmicpc.net/problem/11505
* 분류: 구간 트리
*/
#include <iostream>
#include <vector>

using namespace std;

const int DIVIDER = 1000000007;
int N, M, K;
vector<long long> numbers;

struct RMQ
{
	int n;
	vector<long long> tree, & array;

	RMQ(vector<long long>& array) : array(array), n(array.size()), tree(n, -1)
	{
		Init(0, n - 1, 1);
	}

	long long Init(int nodeLeft, int nodeRight, int node)
	{
		long long& ret = tree[node];
		if (nodeLeft == nodeRight) return ret = array[nodeLeft];

		int nodeMid = (nodeLeft + nodeRight) / 2;
		return ret = Init(nodeLeft, nodeMid, node * 2) * Init(nodeMid + 1, nodeRight, node * 2 + 1) % DIVIDER;
	}

	long long Query(int left, int right)
	{
		if (left > right) swap(left, right);
		return Query(left, right, 0, n - 1, 1);
	}

	long long Query(int left, int right, int nodeLeft, int nodeRight, int node)
	{
		if (right < nodeLeft || nodeRight < left) return 1;
		if (left <= nodeLeft && nodeRight <= right) return tree[node];

		int nodeMid = (nodeLeft + nodeRight) / 2;
		return Query(left, right, nodeLeft, nodeMid, node * 2) * Query(left, right, nodeMid + 1, nodeRight, node * 2 + 1) % DIVIDER;
	}

	void Update(int target, int value)
	{
		array[target] = value;
		Update(target, 0, n - 1, 1);
	}

	long long Update(int target, int nodeLeft, int nodeRight, int node)
	{
		long long& ret = tree[node];
		if (nodeLeft == nodeRight) return ret = array[target];

		int nodeMid = (nodeLeft + nodeRight) / 2;
		Update(target, nodeLeft, nodeMid, node * 2)* Update(target, nodeMid, nodeRight, node * 2 + 1);
	}
};

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> M >> K;
	numbers.resize(N);
	for (int i = 0; i < N; ++i)
		cin >> numbers[i];
	for (int i = 0; i < M + K; ++i)
	{
		int a, b, c;
		cin >> a >> b >> c;
		if (a == 1)
		{

		}
		else
		{

		}
	}


	cout << IsPossible() << "\n";
	return 0;
}