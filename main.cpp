#include "stack.h"
#include "queue.h"
#include "deque.h"
#include "forward_list.h"
#include "list.h"

#include <iostream>

using namespace std;
using namespace plastic;

void basic() {
	stack<int> st;
	st.push(5);
	st.push(4);
	st.push(3);
	st.push(2);
	st.push(1);
	cout << st.pop() << ' ';
	cout << st.pop() << ' ';
	cout << st.pop() << ' ';
	cout << st.pop() << ' ';
	cout << st.pop() << '\n';

	queue<int> qu(10);
	qu.push(1);
	qu.push(2);
	qu.push(3);
	qu.push(4);
	qu.push(5);
	cout << qu.pop() << ' ';
	cout << qu.pop() << ' ';
	cout << qu.pop() << ' ';
	cout << qu.pop() << ' ';
	cout << qu.pop() << '\n';

	deque<int> dq(10);
	dq.push_front(2);
	dq.push_front(1);
	dq.push_back(3);
	dq.push_back(5);
	dq.push_back(4);
	cout << dq.pop_front() << ' ';
	cout << dq.pop_front() << ' ';
	cout << dq.pop_front() << ' ';
	cout << dq.pop_back() << ' ';
	cout << dq.pop_back() << '\n';

	forward_list<int> fl;
	fl.push_front(5);
	fl.push_front(2);
	fl.push_front(1);
	fl.insert_after(fl.begin()->advance(1), 3);
	fl.insert_after(fl.begin()->advance(2), 4);
	cout << fl.pop_front() << ' ';
	cout << fl.pop_front() << ' ';
	cout << fl.pop_front() << ' ';
	cout << fl.pop_front() << ' ';
	cout << fl.pop_front() << '\n';

	list<int> ls;
	ls.push_front(2);
	ls.push_front(1);
	ls.push_back(4);
	ls.insert(++++ls.begin(), 3);
	ls.insert(--ls.end(), 5);
	cout << ls.pop_front() << ' ';
	cout << ls.pop_front() << ' ';
	cout << ls.pop_front() << ' ';
	cout << ls.pop_back() << ' ';
	cout << ls.pop_back() << '\n';

	cout << '\n';
}

void tree() {
	//binary_search_tree<int> bst;
}

int main() {
	basic();
	tree();
}