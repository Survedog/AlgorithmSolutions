/*
* Problem: https://www.acmicpc.net/problem/11438
* 분류: LCA, 구간 트리
*
* (알고리즘 설명은 11437 참고)
* 11437 - LCA와 동일하나 N, M의 최대치가 모두 100000으로 증가했으며 시간 제한이 1.5초로 줄어들었다.
* 하지만 구간 트리를 사용한 해법에서 구간 트리 구성과 질의에는 각각 O(N), O(logN)의 시간복잡도가 필요하므로 알고리즘의 총 시간 복잡도는 O(N + M * logN)인데,
* 이는 O(1억)을 넘지 못하므로 충분히 시간 제한 안에 풀 수 있다고 생각했고, 결과적으로 문제 해결에 성공했다.
*/
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory.h>

using namespace std;

const int INF = 987654321;
int N, M, serial2Num[100001], num2Serial[100001], serialFirstIdx[100001], serialCount;
vector<int> adjacent[100001], dupPreorder;

struct RMQ
{
	int n;
	vector<int> minTree;

	RMQ(vector<int>& array) : n(array.size()), minTree(4 * n)
	{
		Init(0, n - 1, 1);
	}

	int Init(int lo, int hi, int node)
	{
		int& ret = minTree[node];
		if (lo == hi) return ret = dupPreorder[lo];

		int mid = (lo + hi) / 2;
		ret = min(Init(lo, mid, node * 2),
			Init(mid + 1, hi, node * 2 + 1));
		return ret;
	}

	int Query(int left, int right)
	{
		if (left > right) swap(left, right);
		return Query(left, right, 0, n - 1, 1);
	}

	int Query(int left, int right, int lo, int hi, int node)
	{
		if (right < lo || hi < left) return INF;
		if (left <= lo && hi <= right) return minTree[node];

		int mid = (lo + hi) / 2;
		return min(Query(left, right, lo, mid, node * 2),
			Query(left, right, mid + 1, hi, node * 2 + 1));
	}
};

void CalcDuplicatedPreOrder(int node, int parent)
{
	int serial = serialCount++;
	num2Serial[node] = serial;
	serial2Num[serial] = node;

	serialFirstIdx[serial] = dupPreorder.size();
	dupPreorder.push_back(serial);

	for (int adj : adjacent[node])
	{
		if (adj == parent) continue;
		CalcDuplicatedPreOrder(adj, node);
		dupPreorder.push_back(serial);
	}
}

int GetLCA(RMQ& segTree, int a, int b)
{
	int aIdx = serialFirstIdx[num2Serial[a]];
	int bIdx = serialFirstIdx[num2Serial[b]];
	int ret = segTree.Query(aIdx, bIdx);
	return serial2Num[ret];
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N;
	dupPreorder.reserve(2 * N - 1);
	for (int i = 0; i < N - 1; ++i)
	{
		int a, b;
		cin >> a >> b;
		adjacent[a].push_back(b);
		adjacent[b].push_back(a);
	}

	serialCount = 1;
	memset(serial2Num, INF, sizeof(serial2Num));
	memset(num2Serial, INF, sizeof(num2Serial));
	CalcDuplicatedPreOrder(1, 1);
	RMQ segTree(dupPreorder);

	cin >> M;
	for (int i = 0; i < M; ++i)
	{
		int a, b;
		cin >> a >> b;
		cout << GetLCA(segTree, a, b) << "\n";
	}

	return 0;
}