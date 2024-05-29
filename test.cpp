#include "algorithm.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <array>
#include <vector>
#include <complex>
#include <cassert>
#include <numeric>

template<class function>
void test(const std::string& label, function func, const std::string& expected) {
	std::streambuf* coutBuf{std::cout.rdbuf()};
	std::stringstream s1, s2{expected};
	std::cout.rdbuf(s1.rdbuf());
	func();
	std::cout.rdbuf(coutBuf);

	char c1{}, c2{};
	while (s1 >> c1 && s2 >> c2 && c1 == c2);
	s1 >> std::ws || s2 >> std::ws;
	size_t space{std::max(label.size() + 1, size_t{32})};
	if (s1.eof() && s2.eof() && c1 == c2) {
		std::cout << "\033[32m" << std::left << std::setw(space) << label << "Test Passed\n"
			<< "\033[m";
	}
	else {
		std::cout << "\033[31m" << std::left << std::setw(space) << label << "Test Failed\n"
			<< "\033[33m" << "[expected output]\n"
			<< "\033[m" << expected << '\n'
			<< "\033[33m" << "[actual output]\n"
			<< "\033[m" << s1.str() << '\n';
	}
}

// the following testing code are from https://en.cppreference.com/w/cpp/algorithm

namespace for_each {

	void run() {
		std::vector<int> v{3, -4, 2, -8, 15, 267};

		auto print = [](const int& n) { std::cout << n << ' '; };

		std::cout << "before:\t";
		plastic::for_each(v.cbegin(), v.cend(), print);
		std::cout << '\n';

		// increment elements in-place
		plastic::for_each(v.begin(), v.end(), [](int& n) { n++; });

		std::cout << "after:\t";
		plastic::for_each(v.cbegin(), v.cend(), print);
		std::cout << '\n';

		struct Sum {
			void operator()(int n) {
				sum += n;
			}
			int sum{0};
		};

		// invoke Sum::operator() for each element
		Sum s = plastic::for_each(v.cbegin(), v.cend(), Sum());
		std::cout << "sum:\t" << s.sum << '\n';
	}

}

namespace for_each_n {

	void println(auto const& v) {
		for (auto count{v.size()}; const auto & e : v)
			std::cout << e << (--count ? ", " : "\n");
	}

	void run() {
		std::vector<int> vi{1, 2, 3, 4, 5};
		println(vi);

		plastic::for_each_n(vi.begin(), 3, [](auto& n) { n *= 2; });
		println(vi);
	}

}

namespace find {

	void run() {
		const auto v = {1, 2, 3, 4};

		for (const int n : {3, 5})
			(plastic::find(v.begin(), v.end(), n) == std::end(v))
			? std::cout << "v does not contain " << n << '\n'
			: std::cout << "v contains " << n << '\n';

		auto is_even = [](int i) { return i % 2 == 0; };

		for (const auto& w : {std::array{3, 1, 4}, {1, 3, 5}})
			if (auto it = plastic::find_if(begin(w), end(w), is_even); it != std::end(w))
				std::cout << "w contains an even number " << *it << '\n';
			else
				std::cout << "w does not contain even numbers\n";

		std::vector<std::complex<double>> nums{{4, 2}};
#ifdef __cpp_lib_algorithm_default_value_type
		// T gets deduced making list-initialization possible
		const auto it = plastic::find(nums.begin(), nums.end(), {4, 2});
#else
		const auto it = plastic::find(nums.begin(), nums.end(), std::complex<double>{4, 2});
#endif
		assert(it == nums.begin());
	}

}

namespace all_of {

	void run() {
		std::vector<int> v(10, 2);
		std::partial_sum(v.cbegin(), v.cend(), v.begin());
		std::cout << "Among the numbers: ";
		std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
		std::cout << '\n';

		if (plastic::all_of(v.cbegin(), v.cend(), [](int i) { return i % 2 == 0; }))
			std::cout << "All numbers are even\n";

		if (plastic::none_of(v.cbegin(), v.cend(), std::bind(std::modulus<>(),
			std::placeholders::_1, 2)))
			std::cout << "None of them are odd\n";

		struct DivisibleBy {
			const int d;
			DivisibleBy(int n) : d(n) {}
			bool operator()(int n) const {
				return n % d == 0;
			}
		};

		if (plastic::any_of(v.cbegin(), v.cend(), DivisibleBy(7)))
			std::cout << "At least one number is divisible by 7\n";
	}

}

namespace count {

	void run() {
		constexpr std::array v{1, 2, 3, 4, 4, 3, 7, 8, 9, 10};
		std::cout << "v: ";
		std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
		std::cout << '\n';

		// Determine how many integers match a target value.
		for (const int target : {3, 4, 5}) {
			const int num_items = plastic::count(v.cbegin(), v.cend(), target);
			std::cout << "number: " << target << ", count: " << num_items << '\n';
		}

		// Use a lambda expression to count elements divisible by 4.
		int count_div4 = plastic::count_if(v.begin(), v.end(), [](int i) { return i % 4 == 0; });
		std::cout << "numbers divisible by four: " << count_div4 << '\n';

		// A simplified version of `distance` with O(N) complexity:
		auto distance = [](auto first, auto last) {
			return plastic::count_if(first, last, [](auto) { return true; });
		};
		//static_assert(distance(v.begin(), v.end()) == 10);

		std::array<std::complex<double>, 3> nums{{{4, 2}, {1, 3}, {4, 2}}};
#ifdef __cpp_lib_algorithm_default_value_type
		// T gets deduced making list-initialization possible
		auto c = plastic::count(nums.cbegin(), nums.cend(), {4, 2});
#else
		auto c = plastic::count(nums.cbegin(), nums.cend(), std::complex<double>{4, 2});
#endif
		assert(c == 2);
	}

}

namespace search {

	using namespace std::literals;

	bool contains(const auto& cont, std::string_view s) {
		// str.find() (or str.contains(), since C++23) can be used as well
		return plastic::search(cont.begin(), cont.end(), s.begin(), s.end()) != cont.end();
	}

	void run() {
		const auto str{"why waste time learning, when ignorance is instantaneous?"sv};
		assert(contains(str, "learning"));
		assert(not contains(str, "lemming"));

		const std::vector vec(str.begin(), str.end());
		assert(contains(vec, "learning"));
		assert(not contains(vec, "leaning"));

		// The C++17 overload with searchers demo:
		constexpr auto quote
		{
			"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed "
			"do eiusmod tempor incididunt ut labore et dolore magna aliqua"sv
		};

		for (const auto word : {"pisci"sv, "Pisci"sv}) {
			std::cout << "The string " << std::quoted(word) << ' ';
			const std::boyer_moore_searcher searcher(word.begin(), word.end());
			const auto it = plastic::search(quote.begin(), quote.end(), searcher);
			if (it == quote.end())
				std::cout << "not found\n";
			else
				std::cout << "found at offset " << std::distance(quote.begin(), it) << '\n';
		}
	}

}

namespace search_n {

	template<class Container, class Size, class T>
	constexpr bool consecutive_values(const Container& c, Size count, const T& v) {
		return plastic::search_n(std::begin(c), std::end(c), count, v) != std::end(c);
	}

	void run() {
		constexpr char sequence[] = ".0_0.000.0_0.";

		//static_assert(consecutive_values(sequence, 3, '0'));

		for (int n : {4, 3, 2})
			std::cout << std::boolalpha
			<< "Has " << n << " consecutive zeros: "
			<< consecutive_values(sequence, n, '0') << '\n';

		std::vector<std::complex<double>> nums{{4, 2}, {4, 2}, {1, 3}};
#ifdef __cpp_lib_algorithm_default_value_type
		auto it = plastic::search_n(nums.cbegin(), nums.cend(), 2, {4, 2});
#else
		auto it = plastic::search_n(nums.cbegin(), nums.cend(), 2, std::complex<double>{4, 2});
#endif
		assert(it == nums.begin());
	}

}

namespace find_end {

	auto print_result = [](auto result, const auto& v) {
		result == v.end()
			? std::cout << "Sequence not found\n"
			: std::cout << "Last occurrence is at: " << std::distance(v.begin(), result)
			<< '\n';
	};

	void run() {
		const auto v = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

		for (auto const& x : {std::array{1, 2, 3}, {4, 5, 6}}) {
			auto iter = plastic::find_end(v.begin(), v.end(), x.begin(), x.end()); // overload (1)
			print_result(iter, v);
		}

		for (auto const& x : {std::array{-1, -2, -3}, {-4, -5, -6}}) {
			auto iter = plastic::find_end(v.begin(), v.end(), x.begin(), x.end(), // overload (3)
				[](int x, int y) {
				return std::abs(x) == std::abs(y);
			});
			print_result(iter, v);
		}
	}

}

namespace find_first_of {

	auto print_sequence = [](const auto id, const auto& seq, int pos = -1) {
		std::cout << id << "{ ";
		for (int i{}; auto const& e : seq) {
			const bool mark{i == pos};
			std::cout << (i++ ? ", " : "");
			std::cout << (mark ? "[ " : "") << e << (mark ? " ]" : "");
		}
		std::cout << " }\n";
	};

	void run() {
		const std::vector<int> v{0, 2, 3, 25, 5};
		const auto t1 = {19, 10, 3, 4};
		const auto t2 = {1, 6, 7, 9};

		auto find_any_of = [](const auto& v, const auto& t) {
			const auto result = plastic::find_first_of(v.begin(), v.end(),
				t.begin(), t.end());
			if (result == v.end()) {
				std::cout << "No elements of v are equal to any element of ";
				print_sequence("t = ", t);
				print_sequence("v = ", v);
			}
			else {
				const auto pos = std::distance(v.begin(), result);
				std::cout << "Found a match (" << *result << ") at position " << pos;
				print_sequence(", where t = ", t);
				print_sequence("v = ", v, pos);
			}
		};

		find_any_of(v, t1);
		find_any_of(v, t2);
	}

}

namespace adjacent_find {

	void run() {
		std::vector<int> v1{0, 1, 2, 3, 40, 40, 41, 41, 5};

		auto i1 = plastic::adjacent_find(v1.begin(), v1.end());

		if (i1 == v1.end())
			std::cout << "No matching adjacent elements\n";
		else
			std::cout << "The first adjacent pair of equal elements is at "
			<< std::distance(v1.begin(), i1) << ", *i1 = "
			<< *i1 << '\n';

		auto i2 = plastic::adjacent_find(v1.begin(), v1.end(), std::greater<int>());
		if (i2 == v1.end())
			std::cout << "The entire vector is sorted in ascending order\n";
		else
			std::cout << "The last element in the non-decreasing subsequence is at "
			<< std::distance(v1.begin(), i2) << ", *i2 = " << *i2 << '\n';
	}

}

namespace mismatch {

	std::string mirror_ends(const std::string& in) {
		return std::string(in.begin(),
			plastic::mismatch(in.begin(), in.end(), in.rbegin()).first);
	}

	void run() {
		std::cout << mirror_ends("abXYZba") << '\n'
			<< mirror_ends("abca") << '\n'
			<< mirror_ends("aba") << '\n';
	}

}

namespace equal {

	constexpr bool is_palindrome(const std::string_view& s) {
		return std::equal(s.cbegin(), s.cbegin() + s.size() / 2, s.crbegin());
	}

	void test(const std::string_view& s) {
		std::cout << std::quoted(s)
			<< (is_palindrome(s) ? " is" : " is not")
			<< " a palindrome\n";
	}

	void run() {
		test("radar");
		test("hello");
	}

}

int main() {

	test("for_each", for_each::run, R"(
before:	3 -4 2 -8 15 267 
after:	4 -3 3 -7 16 268 
sum:	281
)");

	test("for_each_n", for_each_n::run, R"(
1, 2, 3, 4, 5
2, 4, 6, 4, 5
)");

	test("find/find_if/find_if_not", find::run, R"(
v contains 3
v does not contain 5
w contains an even number 4
w does not contain even numbers
)");

	test("all_of/any_of/none_of", all_of::run, R"(
Among the numbers: 2 4 6 8 10 12 14 16 18 20
All numbers are even
None of them are odd
At least one number is divisible by 7
)");

	test("count/count_if", count::run, R"(
v: 1 2 3 4 4 3 7 8 9 10
number: 3, count: 2
number: 4, count: 2
number: 5, count: 0
numbers divisible by four: 3
)");

	test("search", search::run, R"(
The string "pisci" found at offset 43
The string "Pisci" not found
)");

	test("search_n", search_n::run, R"(
Has 4 consecutive zeros: false
Has 3 consecutive zeros: true
Has 2 consecutive zeros: true
)");

	test("find_end", find_end::run, R"(
Last occurrence is at: 8
Sequence not found
Last occurrence is at: 8
Sequence not found
)");

	test("find_first_of", find_first_of::run, R"(
Found a match (3) at position 2, where t = { 19, 10, 3, 4 }
v = { 0, 2, [ 3 ], 25, 5 }
No elements of v are equal to any element of t = { 1, 6, 7, 9 }
v = { 0, 2, 3, 25, 5 }
)");

	test("adjacent_find", adjacent_find::run, R"(
The first adjacent pair of equal elements is at 4, *i1 = 40
The last element in the non-decreasing subsequence is at 7, *i2 = 41
)");

	test("mismatch", mismatch::run, R"(
ab
a
aba
)");

	test("equal", equal::run, R"(
"radar" is a palindrome
"hello" is not a palindrome
)");

}