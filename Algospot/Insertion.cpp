/*
* Problem: https://algospot.com/judge/problem/read/INSERTION
* 움직인 칸 수 배열을 뒤에서부터 순회하며 원소들을 이동했던 만큼 원위치시키면 처음의 배열을 구할 수 있지만, 원위치 시 일일이 한 칸씩 swap한다면 O(N^2)의 시간 복잡도가 필요해 시간 제한을 초과한다. 
* 배열의 마지막 위치의 움직인 칸 수가 k일 때, 정렬된 수 중 뒤에서 k번째 수 x가 초기엔 마지막 위치에 있었다는 것을 알 수 있다.
* 따라서 마지막 위치에 x를 놓고, x를 정렬된 수의 집합에서 제외한 뒤 다시 그 집합의 마지막 위치에 대해 같은 작업을 반복함으로써 그 다음 위치들에 대해서도 초기 배열의 수를 구할 수 있다.
* 이때 k번째로 큰 수를 O(log)시간에 구하고, x를 수들의 집합에서 O(log)시간에 지우기 위해서 트립을 사용한다.
* 그러면 N개의 위치에 대해 매 반복마다 O(log)의 작업을 수행하니 N이 최대인 50000에서도 충분히 답을 구할 수 있다.
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <utility>

using namespace std;

int N, shift[50000], A[50000];

typedef int KeyType;

typedef struct Node {
	KeyType key;
	int priority, size;
	Node* left, * right;

	Node(KeyType key) : key(key), priority((rand()* rand()) % N), size(1), left(nullptr), right(nullptr) {};

	void SetLeft(Node* newLeft) { left = newLeft; CalcSize(); }
	void SetRight(Node* newRight) { right = newRight; CalcSize(); }

	void CalcSize()
	{
		size = 1;
		if (left) size += left->size;
		if (right) size += right->size;
	}
};

typedef pair<Node*, Node*> NodePair;

NodePair Split(Node* root, KeyType key)
{
	if (root == nullptr)
		return NodePair(nullptr, nullptr);

	if (key > root->key)
	{
		NodePair splitted = Split(root->right, key);
		root->SetRight(splitted.first);
		return NodePair(root, splitted.second);
	}

	NodePair splitted = Split(root->left, key);
	root->SetLeft(splitted.second);
	return NodePair(splitted.first, root);
}

Node* Insert(Node* root, Node* node)
{
	if (root == nullptr) return node;

	if (root->priority < node->priority)
	{
		NodePair splitted = Split(root, node->key);
		node->SetLeft(splitted.first);
		node->SetRight(splitted.second);
		return node;
	}
	else if (node->key < root->key)
		root->SetLeft(Insert(root->left, node));
	else
		root->SetRight(Insert(root->right, node));
	return root;
}

Node* Merge(Node* a, Node* b)
{
	if (a == nullptr) return b;
	if (b == nullptr) return a;

	if (a->priority > b->priority)
	{
		a->SetRight(Merge(a->right, b));
		return a;
	}
	b->SetLeft(Merge(a, b->left));
	return b;
}

Node* Erase(Node* root, KeyType key)
{
	if (root == nullptr) return nullptr;

	if (key == root->key)
	{
		Node* ret = Merge(root->left, root->right);
		delete root;
		return ret;
	}
	else if (key < root->key)
		root->SetLeft(Erase(root->left, key));
	else
		root->SetRight(Erase(root->right, key));
	return root;
}

Node* kth(Node* root, int k)
{
	int leftSize = root->left != nullptr ? root->left->size : 0;
	if (k <= leftSize) return kth(root->left, k);
	else if (k == leftSize + 1) return root;
	else return kth(root->right, k - leftSize - 1);
}

void FindA()
{
	Node* root = nullptr;
	for (int i = 1; i <= N; ++i)
		root = Insert(root, new Node(i));

	int size = N;
	for (int i = N - 1; i >= 0; --i)
	{
		int kthKey = kth(root, size - shift[i])->key;
		A[i] = kthKey;
		root = Erase(root, kthKey);
		size--;
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
		srand(time(NULL));
		cin >> N;
		for (int i = 0; i < N; ++i)
			cin >> shift[i];

		FindA();
		for (int i = 0; i < N; ++i)
			cout << A[i] << " ";
		cout << "\n";
	}
	return 0;
}