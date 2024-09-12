/*
* Problem: https://www.acmicpc.net/problem/11437
* 분류: LCA, 구간 트리
* 
* Algospot의 FamilyTree를 푼 코드와 같은 원리로 해결함. (Family.cpp 참고)
* 트리를 전위 순회하면서 노드들에 방문 순서대로 시리얼 번호를 부여한다. 이렇게 하면 부모 노드가 자식 노드보다 반드시 작은 번호를 가진다.
* 이때 전위 순회에서 방문한(첫 방문 이외에도 모든 방문을 포함) 노드들의 시리얼 번호를 순서대로 배열에 기록하고, 이 배열에 대한 최소 구간 트리를 만든다.
* 이제 구간 트리를 사용해 위 배열에서 최소 공통 조상을 알고자하는 두 노드의 첫 등장 위치 사이의 최소 정점을 구한다.
* 부모의 시리얼 번호는 자식보다 작고, 해당 배열에서 두 정점의 첫 등장 위치 사이엔 모든 공통 조상 노드가 있으므로, 구해진 최소 정점이 최소 공통 조상이 된다.
*/
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory.h>

using namespace std;

const int INF = 987654321;
int N, M, serial2Num[50001], num2Serial[50001], serialFirstIdx[50001], serialCount;
vector<int> adjacent[50001], dupPreorder;

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