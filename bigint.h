#pragma once

#include <iostream>
#include <array>

namespace plastic {

	class bigint {
	public:
		using sign_type = ::std::strong_ordering;
		using data_type = ::std::vector<::std::uint64_t>;

	private:
		sign_type _sign;
		data_type _data;

		static ::std::pair<::std::uint64_t, ::std::uint64_t> _mul(::std::uint64_t num1, ::std::uint64_t num2) {
#ifdef _MSC_VER
			::std::uint64_t high, low;
			low = _umul128(num1, num2, &high);
			return {high, low};
#else
			::std::uint32_t l1{num1 & 0xffffffff}, h1{num1 >> 32};
			::std::uint32_t l2{num2 & 0xffffffff}, h2{num2 >> 32};
			::std::uint64_t x0{::std::uint64_t{l1} *l2};
			::std::uint64_t x1{::std::uint64_t{l1} *h2};
			::std::uint64_t x2{::std::uint64_t{h1} *l2};
			::std::uint64_t x3{::std::uint64_t{h1} *h2};
			x1 += x2 + (x0 >> 32);
			if (x1 < x2) {
				x3 += 0x100000000;
			}
			return {x3 + (x1 >> 32), (x1 << 32) + (x0 & 0xffffffff)};
#endif
		}

		static sign_type _cmp(data_type::const_iterator begin, data_type::const_iterator end, ::std::uint64_t num) {
			return end - begin == 1 ? *begin <=> num : sign_type::greater;
		}

		static sign_type _cmp(data_type::const_iterator begin1, data_type::const_iterator end1, data_type::const_iterator begin2, data_type::const_iterator end2) {
			::std::ptrdiff_t size1{end1 - begin1}, size2{end2 - begin2};
			if (size1 != size2) {
				return size1 <=> size2;
			}
			while (begin1 != end1) {
				if (*--end1 != *--end2) {
					return *end1 <=> *end2;
				}
			}
			return sign_type::equal;
		}

		static ::std::uint64_t _add(data_type::const_iterator begin, data_type::const_iterator end, ::std::uint64_t num, data_type::iterator dest) {
			if (num == 0) {
				return 0;
			}
			while (begin != end) {
				::std::uint64_t sum{*begin++ + num};
				num = sum < num;
				*dest++ = sum;
			}
			return num;
		}

		static ::std::uint64_t _add(data_type::const_iterator begin1, data_type::const_iterator end1, data_type::const_iterator begin2, data_type::const_iterator end2, data_type::iterator dest, ::std::uint64_t carry = 0) {
			while (begin2 != end2) {
				::std::uint64_t sum{*begin1++ + carry};
				carry = sum < carry;
				sum += *begin2;
				carry += sum < *begin2++;
				*dest++ = sum;
			}
			return _add(begin1, end1, carry, dest);
		}

		static ::std::uint64_t _sub(data_type::const_iterator begin, data_type::const_iterator end, ::std::uint64_t num, data_type::iterator dest) {
			if (num == 0) {
				return 0;
			}
			while (begin != end) {
				::std::uint64_t borrow{*begin < num};
				*dest++ = *begin++ - num;
				num = borrow;
			}
			return num;
		}

		static ::std::uint64_t _sub(data_type::const_iterator begin1, data_type::const_iterator end1, data_type::const_iterator begin2, data_type::const_iterator end2, data_type::iterator dest, ::std::uint64_t borrow = 0) {
			while (begin2 != end2) {
				::std::uint64_t sub{*begin2++ + borrow};
				borrow = *begin1 < sub || sub < borrow;
				*dest++ = *begin1++ - sub;
			}
			return _sub(begin1, end1, borrow, dest);
		}

		static ::std::uint64_t _mul(data_type::const_iterator begin, data_type::const_iterator end, ::std::uint64_t num, data_type::iterator dest, ::std::uint64_t carry = 0) {
			while (begin != end) {
				auto [high, low] {_mul(*begin++, num)};
				low += carry;
				carry = high + (low < carry);
				*dest++ = low;
			}
			return carry;
		}

		static ::std::uint64_t _mul(data_type::const_iterator begin1, data_type::const_iterator end1, data_type::const_iterator begin2, data_type::const_iterator end2, data_type::iterator dest, ::std::uint64_t carry = 0) {

		}

		static sign_type _cmp(const bigint& num1, ::std::uint64_t num2) {
			sign_type cmp{_cmp(num1._data.begin(), num1._data.end(), num2)};
			return num1._sign > 0 ? cmp : 0 <=> cmp;
		}

		static sign_type _cmp(const bigint& num1, const bigint& num2) {
			sign_type cmp{_cmp(num1._data.begin(), num1._data.end(), num2._data.begin(), num2._data.end())};
			return num1._sign > 0 ? cmp : 0 <=> cmp;
		}

		static bigint& _add(bigint& num1, ::std::uint64_t num2) {
			::std::uint64_t carry{_add(num1._data.begin(), num1._data.end(), num2, num1._data.begin())};
			if (carry != 0) {
				num1._data.emplace_back(carry);
			}
			return num1;
		}

		static bigint& _add(bigint& num1, const bigint& num2) {
			const bigint* p1{&num1}, * p2{&num2};
			if (num1._data.size() < num2._data.size()) {
				::std::swap(p1, p2);
			}
			::std::uint64_t carry{_add(p1->_data.begin(), p1->_data.end(), p2->_data.begin(), p2->_data.end(), num1._data.begin())};
			if (carry != 0) {
				num1._data.emplace_back(carry);
			}
			return num1;
		}

		static bigint& _sub(bigint& num1, ::std::uint64_t num2) {
			sign_type cmp{_cmp(num1, num2)};
			if (cmp == 0) {
				num1._sign = sign_type::equal;
			}
			else if (cmp > 0) {
				_sub(num1._data.begin(), num1._data.end(), num2, num1._data.begin());
				if (num1._data.back() == 0) {
					num1._data.pop_back();
				}
			}
			else {
				num1.negate();
				num1._data.front() = num2 - num1._data.front();
			}
			return num1;
		}

		static bigint& _sub(bigint& num1, const bigint& num2) {
			sign_type cmp{_cmp(num1, num2)};
			if (cmp == 0) {
				num1._sign = sign_type::equal;
				return num1;
			}
			const bigint* p1{&num1}, * p2{&num2};
			if (cmp < 0) {
				num1.negate();
				::std::swap(p1, p2);
			}
			_sub(p1->_data.begin(), p1->_data.end(), p2->_data.begin(), p2->_data.end(), num1._data.begin());
			while (num1._data.back() == 0) {
				num1._data.pop_back();
			}
			return num1;
		}

		static bigint& _mul(bigint& num1, ::std::uint64_t num2) {
			::std::uint64_t carry{_mul(num1._data.begin(), num1._data.end(), num2, num1._data.begin())};
			if (carry != 0) {
				num1._data.emplace_back(carry);
			}
			return num1;
		}

		static bigint& _mul(bigint& num1, const bigint& num2) {

		}

	public:
		explicit bigint(::std::int64_t num = 0) {
			_sign = num <=> 0;
			if (_sign == 0) {
				return;
			}
			_data.emplace_back(_sign > 0 ? num : -num);
		}

		explicit bigint(::std::string_view num, int base = 0) {
			if (base < 0 || base == 1 || base > 36 || num.empty()) {
				::std::abort();
			}

			auto begin{num.begin()}, end{num.end()};

			_sign = sign_type::greater;
			if (*begin == '+') {
				if (++begin == end) {
					::std::abort();
				}
			}
			else if (*begin == '-') {
				_sign = sign_type::less;
				if (++begin == end) {
					::std::abort();
				}
			}

			if (base == 0) {
				base = 10;
				if (*begin == '0') {
					base = 8;
					if (++begin == end) {
						_sign = sign_type::equal;
						return;
					}
					if (*begin == 'b' || *begin == 'B') {
						base = 2;
						if (++begin == end) {
							::std::abort();
						}
					}
					else if (*begin == 'x' || *begin == 'X') {
						base = 16;
						if (++begin == end) {
							::std::abort();
						}
					}
				}
			}

			while (*begin == '0') {
				if (++begin == end) {
					_sign = sign_type::equal;
					return;
				}
			}

			static constexpr ::std::array<::std::uint8_t, 256> fromCharTable{
				36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 36, 36, 36, 36, 36, 36,
				36, 10, 11, 12, 13, 14, 15, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				36, 10, 11, 12, 13, 14, 15, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
				36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36
			};

			/*
				Table[StringJoin["0x", IntegerString[If[IntegerQ[Log2[b]], Log2[b], Floor[2^61*Log2[b]]], 16, 16]], {b, 2, 36}]
				Table[If[IntegerQ[Log2[b]], 0, Floor[Log[b, 2^64]], 0], {b, 2, 36}]
				Table[StringJoin["0x", IntegerString[If[IntegerQ[Log2[b]], 0, b^Floor[Log[b, 2^64]]], 16, 16]], {b, 2, 36}]
			*/

			static constexpr ::std::array<::std::uint64_t, 37> log2BaseTable{
				0x0000000000000000, 0x0000000000000000, 0x0000000000000001, 0x32b803473f7ad0f3,
				0x0000000000000002, 0x4a4d3c25e68dc57f, 0x52b803473f7ad0f3, 0x59d5d9fd5010b366,
				0x0000000000000003, 0x6570068e7ef5a1e7, 0x6a4d3c25e68dc57f, 0x6eb3a9f01975077f,
				0x72b803473f7ad0f3, 0x766a008e4788cbcd, 0x79d5d9fd5010b366, 0x7d053f6d26089673,
				0x0000000000000004, 0x82cc7edf592262cf, 0x8570068e7ef5a1e7, 0x87ef05ae409a0288,
				0x8a4d3c25e68dc57f, 0x8c8ddd448f8b845a, 0x8eb3a9f01975077f, 0x90c10500d63aa658,
				0x92b803473f7ad0f3, 0x949a784bcd1b8afe, 0x966a008e4788cbcd, 0x982809d5be7072db,
				0x99d5d9fd5010b366, 0x9b74948f5532da4b, 0x9d053f6d26089673, 0x9e88c6b3626a72aa,
				0x0000000000000005, 0xa16bad3758efd873, 0xa2cc7edf592262cf, 0xa4231623369e78e5,
				0xa570068e7ef5a1e7
			};

			static constexpr ::std::array<int, 37> chunkSizeTable{
				0, 0, 0, 40, 0, 27, 24, 22, 0, 20, 19, 18, 17, 17, 16, 16,
				0, 15, 15, 15, 14, 14, 14, 14, 13, 13, 13, 13, 13, 13, 13, 12, 0, 12, 12, 12,
				12
			};

			static constexpr ::std::array<::std::uint64_t, 37> chunkBaseTable{
				0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xa8b8b452291fe821,
				0x0000000000000000, 0x6765c793fa10079d, 0x41c21cb8e1000000, 0x3642798750226111,
				0x0000000000000000, 0xa8b8b452291fe821, 0x8ac7230489e80000, 0x4d28cb56c33fa539,
				0x1eca170c00000000, 0x780c7372621bd74d, 0x1e39a5057d810000, 0x5b27ac993df97701,
				0x0000000000000000, 0x27b95e997e21d9f1, 0x5da0e1e53c5c8000, 0xd2ae3299c1c4aedb,
				0x16bcc41e90000000, 0x2d04b7fdd9c0ef49, 0x5658597bcaa24000, 0xa0e2073737609371,
				0x0c29e98000000000, 0x14adf4b7320334b9, 0x226ed36478bfa000, 0x383d9170b85ff80b,
				0x5a3c23e39c000000, 0x8e65137388122bcd, 0xdd41bb36d259e000, 0x0aee5720ee830681,
				0x0000000000000000, 0x172588ad4f5f0981, 0x211e44f7d02c1000, 0x2ee56725f06e5c71,
				0x41c21cb8e1000000
			};

			if (::std::has_single_bit(static_cast<unsigned>(base))) {
				int bits{static_cast<int>(log2BaseTable[base])};

				_data.resize(((end - begin) * bits >> 6) + 1);
				auto dBegin{_data.begin()}, dEnd{dBegin};

				int offset{0};
				::std::uint64_t value{0};
				while (begin != end) {
					::std::uint8_t digit{fromCharTable[static_cast<::std::uint8_t>(*--end)]};
					if (digit >= base) {
						::std::abort();
					}
					value |= ::std::uint64_t{digit} << offset;
					offset += bits;
					if (offset >= 64) {
						*dEnd++ = value;
						offset -= 64;
						value = digit >> (bits - offset);
					}
				}
				if (value != 0) {
					*dEnd++ = value;
				}

				_data.resize(dEnd - dBegin);

				return;
			}

			::std::uint64_t log2Base{log2BaseTable[base]};
			int chunkSize{chunkSizeTable[base]};
			::std::uint64_t chunkBase{chunkBaseTable[base]};

			_data.resize((_mul(end - begin, log2Base).first >> 3) + 2);
			auto dBegin{_data.begin()}, dEnd{dBegin};

			//if (end - begin <= 2000) {
			while (begin != end) {
				::std::uint64_t value{0};
				if (end - begin >= chunkSize) {
					auto sentinel{begin + chunkSize};
					while (begin != sentinel) {
						::std::uint8_t digit{fromCharTable[static_cast<::std::uint8_t>(*begin++)]};
						if (digit >= base) {
							::std::abort();
						}
						value = value * base + digit;
					}
				}
				else {
					chunkBase = 1;
					while (begin != end) {
						::std::uint8_t digit{fromCharTable[static_cast<::std::uint8_t>(*begin++)]};
						if (digit >= base) {
							::std::abort();
						}
						value = value * base + digit;
						chunkBase *= base;
					}
				}
				::std::uint64_t carry{_mul(dBegin, dEnd, chunkBase, dBegin, value)};
				if (carry != 0) {
					*dEnd++ = carry;
				}
			}

			_data.resize(dEnd - dBegin);

			return;
			//}


		}

		sign_type operator<=>(::std::int64_t num) const {
			if (_sign == 0) {
				return 0 <=> num;
			}
			return _sign == num <=> 0 ? _cmp(*this, num) : _sign;
		}

		sign_type operator<=>(const bigint& num) const {
			if (_sign == 0) {
				return 0 <=> num._sign;
			}
			return _sign == num._sign <=> 0 ? _cmp(*this, num) : _sign;
		}

		bigint operator+() const {
			return *this;
		}

		bigint operator-() const {
			return bigint{*this}.negate();
		}

		bigint operator+(::std::int64_t num) const {
			return bigint{*this} += num;
		}

		bigint operator+(const bigint& num) const {
			return bigint{*this} += num;
		}

		bigint operator-(::std::int64_t num) const {
			return bigint{*this} -= num;
		}

		bigint operator-(const bigint& num) const {
			return bigint{*this} -= num;
		}

		bigint operator*(::std::int64_t num) const {
			return bigint{*this} *= num;
		}

		bigint operator*(const bigint& num) const {
			return bigint{*this} *= num;
		}

		bigint& operator+=(::std::int64_t num) {
			if (_sign == 0) {
				return *this = bigint{num};
			}
			if (num == 0) {
				return *this;
			}
			return _sign == num <=> 0 ? _add(*this, num) : _sub(*this, num);
		}

		bigint& operator+=(const bigint& num) {
			if (_sign == 0) {
				return *this = num;
			}
			if (num._sign == 0) {
				return *this;
			}
			return _sign == num <=> 0 ? _add(*this, num) : _sub(*this, num);
		}

		bigint& operator-=(::std::int64_t num) {
			if (_sign == 0) {
				return *this = bigint{-num};
			}
			if (num == 0) {
				return *this;
			}
			return _sign == num <=> 0 ? _sub(*this, num) : _add(*this, num);
		}

		bigint& operator-=(const bigint& num) {
			if (_sign == 0) {
				return *this = -num;
			}
			if (num._sign == 0) {
				return *this;
			}
			return _sign == num <=> 0 ? _sub(*this, num) : _add(*this, num);
		}

		bigint& operator*=(::std::int64_t num) {
			if (_sign == 0 || num == 0) {
				_sign = sign_type::equal;
				return *this;
			}
			_sign = _sign == num <=> 0 ? sign_type::greater : sign_type::less;
			return _mul(*this, num);
		}

		bigint& operator*=(const bigint& num) {
			if (_sign == 0 || num._sign == 0) {
				_sign = sign_type::equal;
				return *this;
			}
			_sign = _sign == num <=> 0 ? sign_type::greater : sign_type::less;
			return _mul(*this, num);
		}

		bigint& operator++() {
			return *this += 1;
		}

		bigint& operator--() {
			return *this -= 1;
		}

		bigint operator++(int) {
			bigint temp{*this};
			++*this;
			return temp;
		}

		bigint operator--(int) {
			bigint temp{*this};
			--*this;
			return temp;
		}

		sign_type& sign() {
			return _sign;
		}

		data_type& data() {
			return _data;
		}

		bigint& negate() {
			_sign = 0 <=> _sign;
			return *this;
		}

		explicit operator ::std::int64_t() const {
			return _sign == 0 ? 0 : _sign > 0 ? _data.front() : ~_data.front() + 1;
		}

		friend ::std::string to_string(const bigint& num, int base = 0) {

		}

		friend ::std::istream& operator>>(::std::istream& is, bigint& num) {

		}

		friend ::std::ostream& operator<<(::std::ostream& os, const bigint& num) {

		}
	};

}