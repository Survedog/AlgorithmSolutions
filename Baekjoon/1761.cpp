/*
* Problem: https://www.acmicpc.net/problem/1761
* 분류: 구간 트리
* 
* Algospot/FamilyTree와 거의 동일한 알고리즘을 사용해 해결하였다. (자세한 설명은 해당 소스코드 참고)
* 요약: 1번 노드를 트리의 루트로 잡고, 루트에서 DFS를 했을 때의 노드 방문 순서(serial)를 각 노드의 시리얼ID로서 저장한다.
* 이제 루트에서부터 트리를 전위 순회하며 방문하는 각 노드의 시리얼 ID를 저장한다. (이때 흐름이 자식에서 부모로 돌아올 때 부모를 다시 기록해야 한다.)
* 마지막으로 이 방문 순서에 대한 구간 트리를 만들어 두 노드 사이의 최소 공통 조상(LCA)을 찾고 그 조상에서 두 노드까지의 길이를 더하면 된다.
*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 987654321;

struct RMQ
{
	int n;
	vector<int> minTree;
	const vector<int>& original;

	RMQ(const vector<int>& array) : n(array.size()), minTree(4 * n, -1), original(array)
	{
		Init(0, n - 1, 1);
	}

	int Init(int nodeLeft, int nodeRight, int node)
	{
		int& ret = minTree[node];
		if (nodeLeft == nodeRight) return ret = original[nodeLeft];

		int nodeMid = (nodeLeft + nodeRight) / 2;
		return ret = min(Init(nodeLeft, nodeMid, node * 2),
						 Init(nodeMid + 1, nodeRight, node * 2 + 1));
	}

	int Query(int left, int right)
	{
		if (left > right) swap(left, right);
		return Query(left, right, 0, n - 1, 1);
	}

	int Query(int left, int right, int nodeLeft, int nodeRight, int node)
	{
 		if (right < nodeLeft || nodeRight < left) return INF;
		if (left <= nodeLeft && nodeRight <= right) return minTree[node];

		int nodeMid = (nodeLeft + nodeRight) / 2;
		return min(Query(left, right, nodeLeft, nodeMid, node * 2),
				   Query(left, right, nodeMid + 1, nodeRight, node * 2 + 1));
	}
};

int N, M, serialCnt;
vector<int> serial2Id, id2Serial, serialDupPreorder, distFromRoot, serialFirstIdx;
vector<pair<int, int>> adjacent[40001];

void DupPreorderTraversal(int nodeId, int parentId, int rootDist, vector<int>& dupPreorder)
{
	int serial = serialCnt++;
	serialFirstIdx[serial] = dupPreorder.size();
	dupPreorder.push_back(serial);

	id2Serial[nodeId] = serial;
	serial2Id[serial] = nodeId;
	distFromRoot[nodeId] = rootDist;

	for (pair<int, int> adj : adjacent[nodeId])
	{
		int dist = adj.first;
		int childId = adj.second;

		if (childId == parentId) continue;

		DupPreorderTraversal(childId, nodeId, rootDist + dist, dupPreorder);
		dupPreorder.push_back(serial);
	}
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N;
	for (int i = 0; i < N - 1; ++i)
	{
		int node1, node2, d;
		cin >> node1 >> node2 >> d;
		adjacent[node1].emplace_back(d, node2);
		adjacent[node2].emplace_back(d, node1);
	}

	serial2Id.resize(N, -1);
	id2Serial.resize(N + 1, -1);
	distFromRoot.resize(N + 1);
	serialFirstIdx.resize(N, -1);
	
	serialDupPreorder.reserve(2 * N);
	serialCnt = 0;
	DupPreorderTraversal(1, -1, 0, serialDupPreorder);

	RMQ rangeTree(serialDupPreorder);

	cin >> M;
	for (int i = 0; i < M; ++i)
	{
		int nodeA, nodeB;
		cin >> nodeA >> nodeB;

		int aSerial = id2Serial[nodeA];
		int bSerial = id2Serial[nodeB];
		int ancesterId = serial2Id[rangeTree.Query(serialFirstIdx[aSerial], serialFirstIdx[bSerial])];
		cout << distFromRoot[nodeA] + distFromRoot[nodeB] - 2 * distFromRoot[ancesterId] << "\n";
	}
	
	return 0;
}