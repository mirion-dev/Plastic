#include "stack.h"
#include "queue.h"
#include "deque.h"
#include "forward_list.h"
#include "list.h"

#include "binary_search_tree.h"

#include <iostream>

using namespace plastic;

void basic() {
	stack<int> st;
	st.push(5);
	st.push(4);
	st.push(3);
	st.push(2);
	st.push(1);
	std::cout << st.pop() << ' ';
	std::cout << st.pop() << ' ';
	std::cout << st.pop() << ' ';
	std::cout << st.pop() << ' ';
	std::cout << st.pop() << '\n';

	queue<int> qu(10);
	qu.push(1);
	qu.push(2);
	qu.push(3);
	qu.push(4);
	qu.push(5);
	std::cout << qu.pop() << ' ';
	std::cout << qu.pop() << ' ';
	std::cout << qu.pop() << ' ';
	std::cout << qu.pop() << ' ';
	std::cout << qu.pop() << '\n';

	deque<int> dq(10);
	dq.push_front(2);
	dq.push_front(1);
	dq.push_back(3);
	dq.push_back(5);
	dq.push_back(4);
	std::cout << dq.pop_front() << ' ';
	std::cout << dq.pop_front() << ' ';
	std::cout << dq.pop_front() << ' ';
	std::cout << dq.pop_back() << ' ';
	std::cout << dq.pop_back() << '\n';

	forward_list<int> fl;
	fl.push_front(5);
	fl.push_front(2);
	fl.push_front(1);
	fl.insert_after(++fl.begin(), 3);
	fl.insert_after(++++fl.begin(), 4);
	std::cout << fl.pop_front() << ' ';
	std::cout << fl.pop_front() << ' ';
	std::cout << fl.pop_front() << ' ';
	std::cout << fl.pop_front() << ' ';
	std::cout << fl.pop_front() << '\n';

	list<int> ls;
	ls.push_front(2);
	ls.push_front(1);
	ls.push_back(4);
	ls.insert(++++ls.begin(), 3);
	ls.insert(--ls.end(), 5);
	std::cout << ls.pop_front() << ' ';
	std::cout << ls.pop_front() << ' ';
	std::cout << ls.pop_front() << ' ';
	std::cout << ls.pop_back() << ' ';
	std::cout << ls.pop_back() << '\n';

	std::cout << '\n';
}

void bst() {
	binary_search_tree<int> bst;
	bst.insert(12);
	bst.insert(5);
	bst.insert(18);
	bst.insert(2);
	bst.insert(9);
	bst.insert(15);
	bst.insert(19);
	bst.insert(13);
	bst.insert(17);
	bst.insert(1);
	bst.insert(3);
	bst.insert(20);
	bst.erase(1);
	bst.erase(3);
	bst.erase(20);
}

int main() {
	basic();
	bst();
}