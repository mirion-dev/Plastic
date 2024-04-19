#define PLASTIC_VERIFY _DEBUG

// Data Structure
#include "vector.h"
#include "stack.h"
#include "queue.h"
#include "priority_queue.h"
#include "forward_list.h"
#include "list.h"
#include "binary_search_tree.h"
#include "red_black_tree.h"
#include "avl_tree.h"

// Algorithm
#include "sort.h"
#include "bigint.h"

// For Test
#include <iostream>
#include <array>
#include <forward_list>

using namespace plastic;

void dataStructure() {
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.back() = 2;
	v.erase(v.begin() + 1);
	v.insert(v.end(), 3, 5);
	v[3] = 4;
	v.erase(v.end() - 2, v.end());
	v.pop_back();
	v.push_back(5);
	for (auto& i : v) {
		std::cout << i << ' ';
	}
	std::cout << '\n';

	stack<int> s;
	s.push(5);
	s.push(4);
	s.push(3);
	s.push(2);
	s.push(1);
	std::cout << s.top() << ' '; s.pop();
	std::cout << s.top() << ' '; s.pop();
	std::cout << s.top() << ' '; s.pop();
	std::cout << s.top() << ' '; s.pop();
	std::cout << s.top() << '\n';

	queue<int> q;
	q.push(1);
	q.push(2);
	q.push(3);
	q.push(4);
	q.push(5);
	std::cout << q.front() << ' '; q.pop();
	std::cout << q.front() << ' '; q.pop();
	std::cout << q.front() << ' '; q.pop();
	std::cout << q.front() << ' '; q.pop();
	std::cout << q.front() << '\n';

	priority_queue<int, std::greater<>> p;
	p.push(3);
	p.push(1);
	p.push(2);
	p.push(5);
	p.push(4);
	std::cout << p.top() << ' '; p.pop();
	std::cout << p.top() << ' '; p.pop();
	std::cout << p.top() << ' '; p.pop();
	std::cout << p.top() << ' '; p.pop();
	std::cout << p.top() << '\n';

	forward_list<int> f;
	f.push_front(5);
	f.push_front(2);
	f.push_front(1);
	f.insert_after(++f.begin(), 3);
	f.insert_after(++++f.begin(), 4);
	for (auto& i : f) {
		std::cout << i << ' ';
	}
	std::cout << '\n';

	list<int> l;
	l.push_front(2);
	l.push_front(1);
	l.push_back(4);
	l.insert(++++l.begin(), 3);
	l.insert(--l.end(), 5);
	for (auto& i : f) {
		std::cout << i << ' ';
	}
	std::cout << '\n';

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

	avl_tree<int> avl;
	avl.insert(18);
	avl.insert(5);
	avl.insert(12);
	avl.insert(15);
	avl.insert(2);
	avl.insert(9);
	avl.insert(13);
	avl.insert(19);
	avl.insert(17);
	/*
			 12
		   /    \
		  5      15
		 / \    /  \
		2   9  13  18
				  /  \
				 17  19
	*/
	avl.erase(12);
	avl.erase(5);
	avl.erase(13);
	/*
			15
		   /  \
		  9   18
		 /   /  \
		2   17  19
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
	dataStructure();
}