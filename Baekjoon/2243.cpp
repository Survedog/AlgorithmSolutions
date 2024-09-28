/*
* Problem: https://www.acmicpc.net/problem/2243
* 분류: 이진트리 (트립)
* 
* 각 노드가 key의 맛을 가지는 사탕의 양을 나타내는 트립을 구성한 뒤, k번째 노드의 key(맛)를 찾아내는 함수를 구현해 해결했다.
*/
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

int N;
vector<bool> hasCandy;

struct TreapNode
{
	int key, priority, count, wholeCount;
	TreapNode* left, * right;

	TreapNode(int key, int count) : key(key), priority(rand()), count(count), wholeCount(count), left(nullptr), right(nullptr)
	{
	}

	void SetLeft(TreapNode* newLeft)
	{
		left = newLeft;
		CalcWholeCount();
	}

	void SetRight(TreapNode* newRight)
	{
		right = newRight;
		CalcWholeCount();
	}

	void AddCount(int added)
	{
		count += added;
		wholeCount += added;
	}

	void CalcWholeCount()
	{
		wholeCount = count;
		if (left) wholeCount += left->wholeCount;
		if (right) wholeCount += right->wholeCount;
	}
};

pair<TreapNode*, TreapNode*> Split(TreapNode* root, int key)
{
	if (root == nullptr) return make_pair(nullptr, nullptr);
	if (root->key < key)
	{
		pair<TreapNode*, TreapNode*> rightSplit = Split(root->right, key);
		root->SetRight(rightSplit.first);
		return make_pair(root, rightSplit.second);
	}
	pair<TreapNode*, TreapNode*> leftSplit = Split(root->left, key);
	root->SetLeft(leftSplit.second);
	return make_pair(leftSplit.first, root);
}

TreapNode* Insert(TreapNode* root, TreapNode* node)
{
	if (root == nullptr) return node;

	if (root->priority < node->priority)
	{
		hasCandy[node->key] = true;
		pair<TreapNode*, TreapNode*> split = Split(root, node->key);
		node->SetLeft(split.first);
		node->SetRight(split.second);
		return node;
	}
	else if (root->key < node->key)
		root->SetRight(Insert(root->right, node));
	else
		root->SetLeft(Insert(root->left, node));
	return root;
}

TreapNode* Merge(TreapNode* a, TreapNode* b)
{
	if (a == nullptr) return b;
	if (b == nullptr) return a;

	if (a->priority < b->priority)
	{
		b->SetLeft(Merge(a, b->left));
		return b;
	}
	a->SetRight(Merge(a->right, b));
	return a;
}

TreapNode* Erase(TreapNode* root, int key)
{
	if (root == nullptr) return nullptr;

	if (root->key == key)
	{
		hasCandy[key] = false;
		TreapNode* newRoot = Merge(root->left, root->right);
		delete root;
		return newRoot;
	}
	else if (root->key < key)
		root->SetRight(Erase(root->right, key));
	else
		root->SetLeft(Erase(root->left, key));
	return root;
}

int FindKthKey(TreapNode* root, int k)
{
	int leftWholeCount = 0;
	if (root->left) leftWholeCount = root->left->wholeCount;
	if (k <= leftWholeCount) return FindKthKey(root->left, k);
	if (k <= leftWholeCount + root->count) return root->key;
	return FindKthKey(root->right, k - leftWholeCount - root->count);
}

TreapNode* AddKeyCount(TreapNode* root, int key, int count)
{
	if (root->key == key)
	{
		root->AddCount(count);
		if (root->count == 0) return Erase(root, key);
		return root;
	}
	else if (root->key < key)
		root->SetRight(AddKeyCount(root->right, key, count));
	else
		root->SetLeft(AddKeyCount(root->left, key, count));
	return root;
}

void DeleteTree(TreapNode* root)
{
	if (root->left) DeleteTree(root->left);
	if (root->right) DeleteTree(root->right);
	delete root;
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> N;
	TreapNode* root = nullptr;
	hasCandy.resize(1000001, false);
	for (int i = 0; i < N; ++i)
	{
		int A, B, C;
		cin >> A;
		if (A == 1)
		{
			cin >> B;
			int targetKey = FindKthKey(root, B);
			cout << targetKey << "\n";
			root = AddKeyCount(root, targetKey, -1);
		}
		else
		{
			cin >> B >> C;
			if (C == 0) continue;

			if (!hasCandy[B])
				root = Insert(root, new TreapNode(B, C));
			else
				root = AddKeyCount(root, B, C);
		}
	}

	if (root) DeleteTree(root);
	return 0;
}