#pragma once

#include <functional>

namespace plastic {

	template<class T, ::std::strict_weak_order<T, T> compare = ::std::less<>, bool unchecked = false>
	class priority_queue {
		T* _begin;
		::std::size_t _size;
		::std::size_t _capacity;
		static constexpr compare _compare{};

		void _grow() {
			_capacity += _capacity <= 1 ? 1 : _capacity >> 1;

			T* newBegin{new T[_capacity]};
			if constexpr (!unchecked) {
				if (newBegin == nullptr) {
					::std::abort();
				}
			}
			for (::std::size_t i{0}; i != _size; ++i) {
				newBegin[i] = ::std::move(_begin[i]);
			}
			delete[] _begin;

			_begin = newBegin;
		}

		void _siftUp(::std::size_t i) const {
			while (i != 0) {
				::std::size_t parent{(i - 1) >> 1};
				if (!_compare(_begin[parent], _begin[i])) {
					return;
				}
				::std::swap(_begin[i], _begin[parent]);
				i = parent;
			}
		}

		void _siftDown(::std::size_t i) const {
			while (true) {
				::std::size_t left{(i << 1) + 1};
				if (left >= _size) {
					return;
				}
				::std::size_t right{left + 1}, max{right};
				if (right >= _size || _compare(_begin[right], _begin[left])) {
					max = left;
				}
				if (!_compare(_begin[i], _begin[max])) {
					return;
				}
				::std::swap(_begin[i], _begin[max]);
				i = max;
			}
		}

	public:
		explicit priority_queue(::std::size_t capacity = 0) {
			_begin = new T[capacity];
			_size = 0;
			_capacity = capacity;
		}

		~priority_queue() {
			delete[] _begin;
		}

		bool empty() const {
			return _size == 0;
		}

		::std::size_t size() const {
			return _size;
		}

		T& top() {
			if constexpr (!unchecked) {
				if (empty()) {
					::std::abort();
				}
			}
			return *_begin;
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
			if constexpr (!unchecked) {
				if (empty()) {
					::std::abort();
				}
			}
			--_size;
			_begin[0] = ::std::move(_begin[_size]);
			_siftDown(0);
		}
	};

}