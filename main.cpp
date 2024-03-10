// basic
#include "stack.h"
#include "queue.h"
#include "priority_queue.h"
#include "deque.h"
#include "forward_list.h"
#include "list.h"

// bst
#include "binary_search_tree.h"
#include "red_black_tree.h"

// algorithm
#include "sort.h"

#include <iostream>
#include <array>

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

	queue<int> qu;
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

	priority_queue<int, std::greater<>{} > pq;
	pq.push(3);
	pq.push(1);
	pq.push(2);
	pq.push(5);
	pq.push(4);
	std::cout << pq.pop() << ' ';
	std::cout << pq.pop() << ' ';
	std::cout << pq.pop() << ' ';
	std::cout << pq.pop() << ' ';
	std::cout << pq.pop() << '\n';

	deque<int> dq;
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
	bst.insert(18);
	bst.insert(5);
	bst.insert(12);
	bst.insert(15);
	bst.insert(2);
	bst.insert(9);
	bst.insert(13);
	bst.insert(19);
	bst.insert(17);
	/*
			18
		   /  \
		  5	  19
		 / \
		2  12
		  /  \
		 9   15
			/  \
		   13  17
	*/
	bst.erase(12);
	bst.erase(5);
	bst.erase(13);
	/*
			18
		   /  \
		  9   19
		 / \
		2  15
			 \
			 17
	*/

	red_black_tree<int> rbt;
	rbt.insert(18);
	rbt.insert(5);
	rbt.insert(12);
	rbt.insert(15);
	rbt.insert(2);
	rbt.insert(9);
	rbt.insert(13);
	rbt.insert(19);
	rbt.insert(17);
	/*
			   12
			/      \
		   5      [15]
		  / \     /  \
		[2] [9]  13  18
					/  \
				  [17][19]
	*/
	rbt.erase(12);
	rbt.erase(5);
	rbt.erase(13);
	/*
			 15
			/  \
		   9  [18]
		  /   /  \
		[2]  17  19
	*/
}

void algorithm() {
	std::array data{2, 5, 12, 20, 4, 7, 16, 8, 10, 8, 12, 6, 10, 2, 19, 14, 17, 3, 18, 19};
	std::array sorted{2, 2, 3, 4, 5, 6, 7, 8, 8, 10, 10, 12, 12, 14, 16, 17, 18, 19, 19, 20};
	std::array<int, 20> copy;
	std::cout << std::boolalpha;
	copy = data;
	selection_sort(copy.begin(), copy.end());
	std::cout << (copy == sorted) << ' ';
	copy = data;
	insertion_sort(copy.begin(), copy.end());
	std::cout << (copy == sorted) << ' ';
	copy = data;
	bubble_sort(copy.begin(), copy.end());
	std::cout << (copy == sorted) << ' ';
	copy = data;
	heap_sort(copy.begin(), copy.end());
	std::cout << (copy == sorted) << ' ';
	copy = data;
	merge_sort(copy.begin(), copy.end());
	std::cout << (copy == sorted) << ' ';
	copy = data;
	quick_sort(copy.begin(), copy.end());
	std::cout << (copy == sorted) << '\n';
}

int main() {
	bst();
}