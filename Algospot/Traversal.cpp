/*
* Problem: https://algospot.com/judge/problem/read/TRAVERSAL
* 전위 순회 시 만나게 되는 노드들의 중위 순회에서의 위치(순서)를 큐에 저장하고, 큐에 저장된 노드들을 순서대로 방문하면서 부모 노드의 위치와 큐 상에서의 다음 노드들을 대조하면 좌우측 서브트리의 루트와 서브트리의 범위를 알 수 있다.
* 이를 이용해 재귀 함수에서 각 서브 트리를 조회하면서 후위 순회에 따라 노드 번호를 출력하는 로직을 구성할 수 있었다.
*/
#include <iostream>
#include <queue>

using namespace std;

int N, preorder[100], inorder[100];
queue<int> roots;

void PrintPostorder(int root, int start, int end)
{
	if (start > end)
		return;

	if (!roots.empty())
	{
		int nextRoot = roots.front();
		if (nextRoot < root && nextRoot >= start)
		{
			roots.pop();
			PrintPostorder(nextRoot, start, root - 1);
		}
	}

	if (!roots.empty())
	{
		int nextRoot = roots.front();
		if (nextRoot > root && nextRoot <= end)
		{
			roots.pop();
			PrintPostorder(nextRoot, root + 1, end);
		}
	}
	cout << inorder[root] << " ";
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{

		cin >> N;
		for (int i = 0; i < N; ++i)
			cin >> preorder[i];
		for (int i = 0; i < N; ++i)
			cin >> inorder[i];

		roots = queue<int>();
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
				if (preorder[i] == inorder[j])
				{
					roots.push(j);
					break;
				}

		int root = roots.front();
		roots.pop();
		PrintPostorder(root, 0, N - 1);
		cout << "\n";
	}

	return 0;
}
