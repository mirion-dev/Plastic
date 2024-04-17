#pragma once

#include <functional>

namespace plastic {

	template<class T, ::std::strict_weak_order<T, T> auto _compare = ::std::less<>{} >
	class priority_queue {
		T* _begin;
		::std::size_t _size;
		::std::size_t _capacity;

		void _grow() {
			_capacity += _capacity <= 1 ? 1 : _capacity >> 1;

			auto newBegin{new T[_capacity]};
			for (::std::size_t i{1}; i != _size; ++i) {
				newBegin[i] = ::std::move(_begin[i]);
			}
			delete[] _begin;

			_begin = newBegin;
		}

		static ::std::size_t _parent(::std::size_t pos) {
			return pos >> 1;
		}

		static ::std::size_t _leftChild(::std::size_t pos) {
			return pos << 1;
		}

		static ::std::size_t _rightChild(::std::size_t pos) {
			return (pos << 1) + 1;
		}

		void _siftUp(::std::size_t pos) const {
			while (true) {
				::std::size_t parent{_parent(pos)};
				if (parent == 0 || !_compare(_begin[parent], _begin[pos])) {
					return;
				}
				::std::swap(_begin[pos], _begin[parent]);
				pos = parent;
			}
		}

		void _siftDown(::std::size_t pos) const {
			while (true) {
				::std::size_t left{_leftChild(pos)};
				if (left > _size) {
					return;
				}
				::std::size_t right{left + 1}, max{right};
				if (right > _size || _compare(_begin[right], _begin[left])) {
					max = left;
				}
				if (!_compare(_begin[pos], _begin[max])) {
					return;
				}
				::std::swap(_begin[pos], _begin[max]);
				pos = max;
			}
		}

	public:
		explicit priority_queue(::std::size_t capacity = 1) {
			_begin = new T[capacity + 1];
			_size = 1;
			_capacity = capacity + 1;
		}

		~priority_queue() {
			delete[] _begin;
		}

		bool empty() const {
			return _size == 1;
		}

		::std::size_t size() const {
			return _size - 1;
		}

		T& top() {
			return _begin[1];
		}

		void push(const T& value) {
			if (_size == _capacity) {
				_grow();
			}
			_begin[_size] = value;
			_siftUp(_size);
			++_size;
		}

		void pop() {
			if (empty()) {
				::std::abort();
			}
			--_size;
			_begin[1] = ::std::move(_begin[_size]);
			_siftDown(1);
		}
	};

}