/*
* Problem: https://www.acmicpc.net/problem/2357
* 분류: 구간 트리
* 
* 최소 구간 트리를 구현하고 배열과, 그 배열의 모든 원소에 -1을 곱해 만든 또다른 배열에 대해 최소 구간 트리를 만들어 최솟값, 최댓값을 구하면 된다.
*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 1000000001;

struct RMQ
{
	int n;
	const vector<int>& array;
	vector<int> minTree;

	RMQ(const vector<int>& array) : n(array.size()), array(array), minTree(4 * n)
	{
		Init(0, n - 1, 1);
	}

	int Init(int nodeLeft, int nodeRight, int node)
	{
		int& ret = minTree[node];
		if (nodeLeft == nodeRight) return ret = array[nodeLeft];

		int nodeMid = (nodeLeft + nodeRight) / 2;
		return ret = min(Init(nodeLeft, nodeMid, node * 2),
			Init(nodeMid + 1, nodeRight, node * 2 + 1));
	}

	int Query(int left, int right)
	{
		if (left > right) swap(left, right);
		return Query(left, right, 1, 0, n - 1);
	}

	int Query(int left, int right, int node, int nodeLeft, int nodeRight)
	{
		if (right < nodeLeft || nodeRight < left) return INF;
		if (left <= nodeLeft && nodeRight <= right) return minTree[node];

		int nodeMid = (nodeLeft + nodeRight) / 2;
		return min(Query(left, right, node * 2, nodeLeft, nodeMid),
			Query(left, right, node * 2 + 1, nodeMid + 1, nodeRight));
	}
};

int N, M;
vector<int> numbers, negNumbers;

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> M;
	numbers.resize(N);
	negNumbers.resize(N);
	for (int i = 0; i < N; ++i)
	{
		cin >> numbers[i];
		negNumbers[i] = -numbers[i];
	}

	RMQ minRMQ(numbers), maxRMQ(negNumbers);
	for (int i = 0; i < M; ++i)
	{
		int a, b;
		cin >> a >> b;
		a--; b--;
		cout << minRMQ.Query(a, b) << " " << -maxRMQ.Query(a, b) << "\n";
	}

	return 0;
}