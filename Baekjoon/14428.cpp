/*
* Problem: https://www.acmicpc.net/problem/14428
* 분류: 구간 트리
* 
* 최솟값 자체가 아닌 최솟값의 인덱스를 저장하는 최소 구간 트리를 만들어 해결했다.
* 인덱스가 1부터 시작하기 때문에 간단한 계산을 위해 배열의 크기를 1 늘렸는데, 이 경우 구간 트리 생성 시 배열의 크기 n에 array.size() - 1을 넣어야 함에 주의.
*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 1000000001;
int N, M;
vector<int> series;

struct RMQ
{
	int n;
	vector<int> minTree;
	vector<int>& original;

	RMQ(vector<int>& array) : n(array.size() - 1), original(array), minTree(4 * n)
	{
		Init(1, n, 1);
	}

	int Init(int nodeLeft, int nodeRight, int node)
	{
		int& ret = minTree[node];
		if (nodeLeft == nodeRight) return ret = nodeLeft;

		int nodeMid = (nodeLeft + nodeRight) / 2;
		int leftMinIdx = Init(nodeLeft, nodeMid, node * 2);
		int rightMinIdx = Init(nodeMid + 1, nodeRight, node * 2 + 1);

		if (original[leftMinIdx] <= original[rightMinIdx])
			ret = leftMinIdx;
		else
			ret = rightMinIdx;
		return ret;
	}

	int Query(int left, int right)
	{
		if (left > right) swap(left, right);
		return Query(left, right, 1, n, 1);
	}

	int Query(int left, int right, int nodeLeft, int nodeRight, int node)
	{
		if (right < nodeLeft || nodeRight < left) return 0;
		if (left <= nodeLeft && nodeRight <= right) return minTree[node];

		int nodeMid = (nodeLeft + nodeRight) / 2;
		int leftMinIdx = Query(left, right, nodeLeft, nodeMid, node * 2);
		int rightMinIdx = Query(left, right, nodeMid + 1, nodeRight, node * 2 + 1);

		if (original[leftMinIdx] <= original[rightMinIdx])
			return leftMinIdx;
		else
			return rightMinIdx;
	}

	void Update(int targetIdx, int newValue)
	{
		original[targetIdx] = newValue;
		Update(targetIdx, newValue, 1, n, 1);
	}

	int Update(int targetIdx, int newValue, int nodeLeft, int nodeRight, int node)
	{
		int& ret = minTree[node];
		if (targetIdx < nodeLeft || nodeRight < targetIdx) return ret;
		if (nodeLeft == nodeRight) return nodeLeft;

		int nodeMid = (nodeLeft + nodeRight) / 2;
		int leftMinIdx = Update(targetIdx, newValue, nodeLeft, nodeMid, node * 2);
		int rightMinIdx = Update(targetIdx, newValue, nodeMid + 1, nodeRight, node * 2 + 1);

		if (original[leftMinIdx] <= original[rightMinIdx])
			return ret = leftMinIdx;
		else
			return ret = rightMinIdx;
	}
};

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N;
	series.resize(N + 1);
	series[0] = INF;
	for (int i = 1; i <= N; ++i)
		cin >> series[i];

	RMQ rangeTree(series);

	cin >> M;
	for (int i = 0; i < M; ++i)
	{
		int a, b, c;
		cin >> a >> b >> c;
		if (a == 1)
			rangeTree.Update(b, c);
		else
			cout << rangeTree.Query(b, c) << "\n";
	}

	return 0;
}