/*
* Problem: https://algospot.com/judge/problem/read/BRACKETS2
* 스택을 이용해 해결하였다.
*/
#include <iostream>
#include <string>
#include <stack>

using namespace std;

string expr;

bool CheckExprValid()
{
	stack<char> st;
	for (int i = 0; i < expr.size(); ++i)
	{
		if (expr[i] == '(' || expr[i] == '{' || expr[i] == '[')
			st.push(expr[i]);
		else if (!st.empty())
		{
			bool closureValid =
				(expr[i] == ')' && st.top() == '(') ||
				(expr[i] == '}' && st.top() == '{') ||
				(expr[i] == ']' && st.top() == '[');

			if (closureValid) st.pop();
			else return false;
		}
		else
			return false;
	}

	return st.empty();
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	int C;
	cin >> C;
	while (C--)
	{
		cin >> expr;
		cout << (CheckExprValid() ? "YES" : "NO") << "\n";
	}

	return 0;
}