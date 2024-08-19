/*
* Problem: https://algospot.com/judge/problem/read/FAMILYTREE
* 가족 관계를 트리로 표현한 후 두 노드의 최소 공통 조상을 구하면 노드 사이의 깊이를 비교하여 두 노드의 촌수를 구할 수 있다.
* 이때 조상을 하나하나 거슬러 올라가며 최소 공통 조상을 구한다면 최대 O(N)의 시간 복잡도를 가지므로 시간 초과가 발생한다.
* 
* 책 해설) 트리의 각 노드들을 전위 순회하며 다시 번호를 부여하면 자식 노드는 반드시 부모 노드보다 높은 숫자를 가지게 된다.
* 이후 트리를 전위 순회할 때의 노드의 순서를 배열에 저장하고 (단 일반적인 전위 순회와 달리 자식 노드에서 부모 노드로 돌아올 때마다 부모 노드의 번호를 다시 저장한다),
* 그 배열에 대해 최소 구간 트리를 만들어, 해당 배열 상에서 두 노드 사이 구간의 최소 노드를 구하면 해당 노드는 최소 공통 조상이 된다.
* 구간 트리는 한 번의 질의에 대해 O(logN)의 시간 복잡도를 가지므로 무사히 시간 내에 문제를 해결할 수 있다.
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct RMQ
{
	int n;
	vector<int> rangeMin, & array;

	RMQ(vector<int>& inArray) : array(inArray)
	{
		n = array.size();
		rangeMin.resize(4 * n);
		Init();
	}

	inline void Init()
	{
		Init(0, n - 1, 1);
	}

	int Init(int left, int right, int node)
	{
		int& ret = rangeMin[node];
		if (left == right) return ret = array[left];

		int mid = (left + right) / 2;
		ret = min(Init(left, mid, node * 2),
			Init(mid + 1, right, node * 2 + 1));
		return ret;
	}

	inline int Query(int left, int right)
	{
		if (left > right) swap(left, right);
		return Query(left, right, 1, 0, n - 1);
	}

	int Query(int left, int right, int node, int nodeLeft, int nodeRight)
	{
		if (right < nodeLeft || nodeRight < left) return INT_MAX;
		if (left <= nodeLeft && nodeRight <= right) return rangeMin[node];

		int nodeMid = (nodeLeft + nodeRight) / 2;
		return min(Query(left, right, node * 2, nodeLeft, nodeMid),
			Query(left, right, node * 2 + 1, nodeMid + 1, nodeRight));
	}
};

int N, Q, parent[100000], locInTrip[100000], depth[100000], serial2no[100000], no2serial[100000];
vector<int> children[100000], trip;

void MakeP(int num, int serial, int& count, int d)
{
	trip.push_back(serial);
	locInTrip[serial] = trip.size() - 1;

	serial2no[serial] = num;
	no2serial[num] = serial;
	depth[num] = d;

	for (int child : children[num])
	{
		int nextSerial = ++count;
		MakeP(child, nextSerial, count, d + 1);
		trip.push_back(serial);
	}
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
		parent[0] = 0;
		for (int i = 0; i < N; ++i)
			children[i].clear();

		for (int i = 1; i < N; ++i)
		{
			cin >> parent[i];
			children[parent[i]].push_back(i);
		}

		int count = 0;
		trip.clear();
		trip.reserve(2 * N - 1);
		MakeP(0, 0, count, 0);

		RMQ rangeTree = RMQ(trip);
		for (int i = 0; i < Q; ++i)
		{
			int a, b;
			cin >> a >> b;

			int serialA = no2serial[a], serialB = no2serial[b];
			int serialLCA = rangeTree.Query(locInTrip[serialA], locInTrip[serialB]);
			cout << depth[a] + depth[b] - 2 * depth[serial2no[serialLCA]] << "\n";
		}
	}
	return 0;
}