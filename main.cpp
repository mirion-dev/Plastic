#include "stack.h"
#include "queue.h"
#include "deque.h"
#include "forward_list.h"
#include "list.h"

#include "binary_search_tree.h"

#include <iostream>

using namespace std;
using namespace plastic;

void basic() {
	stack<int> st;
	st.push(1);
	st.push(2);
	st.push(3);
	st.push(4);
	st.push(5);
	// 1 2 3 4 5
	cout << st.pop() << ' ';
	cout << st.pop() << ' ';
	cout << st.pop() << ' ';
	cout << st.pop() << ' ';
	cout << st.pop() << '\n';
	// 5 4 3 2 1

	queue<int> qu(10);
	qu.push(1);
	qu.push(2);
	qu.push(3);
	qu.push(4);
	qu.push(5);
	// 1 2 3 4 5
	cout << qu.pop() << ' ';
	cout << qu.pop() << ' ';
	cout << qu.pop() << ' ';
	cout << qu.pop() << ' ';
	cout << qu.pop() << '\n';
	// 1 2 3 4 5

	deque<int> dq(10);
	dq.push_front(1);
	dq.push_front(2);
	dq.push_back(3);
	dq.push_back(4);
	dq.push_back(5);
	// 2 1 3 4 5
	cout << dq.pop_front() << ' ';
	cout << dq.pop_front() << ' ';
	cout << dq.pop_front() << ' ';
	cout << dq.pop_back() << ' ';
	cout << dq.pop_back() << '\n';
	// 2 1 3 5 4

	forward_list<int> fl;
	fl.push_front(1);
	fl.push_front(2);
	fl.push_front(3);
	fl.insert_after(fl.begin()->advance(1), 4);
	fl.insert_after(fl.begin()->advance(2), 5);
	// 3 2 4 5 1
	cout << fl.pop_front() << ' ';
	cout << fl.pop_front() << ' ';
	cout << fl.pop_front() << ' ';
	cout << fl.pop_front() << ' ';
	cout << fl.pop_front() << '\n';
	// 3 2 4 5 1

	list<int> ls;
	ls.push_front(1);
	ls.push_front(2);
	ls.push_back(3);
	ls.insert(ls.begin()->advance(2), 4);
	ls.insert(ls.end()->advance(-1), 5);
	// 2 1 4 5 3
	cout << ls.pop_front() << ' ';
	cout << ls.pop_front() << ' ';
	cout << ls.pop_front() << ' ';
	cout << ls.pop_back() << ' ';
	cout << ls.pop_back() << '\n';
	// 2 1 4 3 5

	cout << '\n';
}

void tree() {
	binary_search_tree<int> bst;
}

int main() {
	basic();
	tree();
}