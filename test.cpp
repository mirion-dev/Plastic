#include "algorithm.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <array>
#include <vector>
#include <complex>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <random>

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

namespace copy {

	void run() {
		std::vector<int> from_vector(10);
		std::iota(from_vector.begin(), from_vector.end(), 0);

		std::vector<int> to_vector;
		plastic::copy(from_vector.begin(), from_vector.end(),
			std::back_inserter(to_vector));
		// or, alternatively,
		//  std::vector<int> to_vector(from_vector.size());
		//  std::copy(from_vector.begin(), from_vector.end(), to_vector.begin());
		// either way is equivalent to
		//  std::vector<int> to_vector = from_vector;

		std::cout << "to_vector contains: ";

		plastic::copy(to_vector.begin(), to_vector.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << '\n';

		std::cout << "odd numbers in to_vector are: ";

		plastic::copy_if(to_vector.begin(), to_vector.end(),
			std::ostream_iterator<int>(std::cout, " "),
			[](int x) { return x % 2 != 0; });
		std::cout << '\n';

		std::cout << "to_vector contains these multiples of 3: ";

		to_vector.clear();
		plastic::copy_if(from_vector.begin(), from_vector.end(),
			std::back_inserter(to_vector),
			[](int x) { return x % 3 == 0; });

		for (const int x : to_vector)
			std::cout << x << ' ';
		std::cout << '\n';
	}

}

namespace copy_n {

	void run() {
		std::string in{"1234567890"};
		std::string out;

		plastic::copy_n(in.begin(), 4, std::back_inserter(out));
		std::cout << out << '\n';

		std::vector<int> v_in(128);
		std::iota(v_in.begin(), v_in.end(), 1);
		std::vector<int> v_out(v_in.size());

		plastic::copy_n(v_in.cbegin(), 100, v_out.begin());
		std::cout << std::accumulate(v_out.begin(), v_out.end(), 0) << '\n';
	}

}

namespace copy_backward {

	void run() {
		std::vector<int> source(4);
		std::iota(source.begin(), source.end(), 1); // fills with 1, 2, 3, 4

		std::vector<int> destination(6);

		plastic::copy_backward(source.begin(), source.end(), destination.end());

		std::cout << "destination contains: ";
		for (auto i : destination)
			std::cout << i << ' ';
		std::cout << '\n';
	}

}

namespace move {

	using container = std::vector<std::string>;

	void print(std::string_view comment, const container& src, const container& dst = {}) {
		auto prn = [](std::string_view name, const container& cont) {
			std::cout << name;
			for (const auto& s : cont)
				std::cout << (s.empty() ? "." : s.data()) << ' ';
			std::cout << '\n';
		};
		std::cout << comment << '\n';
		prn("src: ", src);
		if (dst.empty())
			return;
		prn("dst: ", dst);
	}

	void run() {
		container src{"foo", "bar", "baz"};
		container dst{"qux", "quux", "quuz", "corge"};
		print("Non-overlapping case; before move:", src, dst);
		plastic::move(src.begin(), src.end(), dst.begin());
		print("After:", src, dst);

		src = {"snap", "crackle", "pop", "lock", "drop"};
		print("Overlapping case; before move:", src);
		plastic::move(std::next(src.begin(), 3), src.end(), src.begin());
		print("After:", src);
	}

}

namespace move_backward {

	using container = std::vector<std::string>;

	void print(std::string_view comment, const container& src, const container& dst = {}) {
		auto prn = [](std::string_view name, const container& cont) {
			std::cout << name;
			for (const auto& s : cont)
				std::cout << (s.empty() ? "." : s.data()) << ' ';
			std::cout << '\n';
		};
		std::cout << comment << '\n';
		prn("src: ", src);
		if (dst.empty())
			return;
		prn("dst: ", dst);
	}

	void run() {
		container src{"foo", "bar", "baz"};
		container dst{"qux", "quux", "quuz", "corge"};
		print("Non-overlapping case; before move_backward:", src, dst);
		plastic::move_backward(src.begin(), src.end(), dst.end());
		print("After:", src, dst);

		src = {"snap", "crackle", "pop", "lock", "drop"};
		print("Overlapping case; before move_backward:", src);
		plastic::move_backward(src.begin(), std::next(src.begin(), 3), src.end());
		print("After:", src);
	}

}

namespace swap_ranges {

	auto print = [](auto comment, auto const& seq) {
		std::cout << comment;
		for (const auto& e : seq)
			std::cout << e << ' ';
		std::cout << '\n';
	};

	void run() {
		std::vector<char> v{'a', 'b', 'c', 'd', 'e'};
		std::list<char> l{'1', '2', '3', '4', '5'};

		print("Before swap_ranges:\n" "v: ", v);
		print("l: ", l);

		plastic::swap_ranges(v.begin(), v.begin() + 3, l.begin());

		print("After swap_ranges:\n" "v: ", v);
		print("l: ", l);
	}

}

namespace iter_swap {

	void run() {
		std::vector<int> v{1, 2, 3, 4, 5};
		plastic::iter_swap(v.begin(), v.end() - 1);
		for (auto& i : v) {
			std::cout << i << ' ';
		}
	}

}

namespace transform {

	void print_ordinals(const std::vector<unsigned>& ordinals) {
		std::cout << "ordinals: ";
		for (unsigned ord : ordinals)
			std::cout << std::setw(3) << ord << ' ';
		std::cout << '\n';
	}

	char to_uppercase(unsigned char c) {
		return std::toupper(c);
	}

	void to_uppercase_inplace(char& c) {
		c = to_uppercase(c);
	}

	void unary_transform_example(std::string& hello, std::string world) {
		// Transform string to uppercase in-place

		plastic::transform(hello.cbegin(), hello.cend(), hello.begin(), to_uppercase);
		std::cout << "hello = " << std::quoted(hello) << '\n';

		// for_each version (see Notes above)
		std::for_each(world.begin(), world.end(), to_uppercase_inplace);
		std::cout << "world = " << std::quoted(world) << '\n';
	}

	void binary_transform_example(std::vector<unsigned> ordinals) {
		// Transform numbers to doubled values

		print_ordinals(ordinals);

		plastic::transform(ordinals.cbegin(), ordinals.cend(), ordinals.cbegin(),
			ordinals.begin(), std::plus<>{});

		print_ordinals(ordinals);
	}

	void run() {
		std::string hello("hello");
		unary_transform_example(hello, "world");

		std::vector<unsigned> ordinals;
		std::copy(hello.cbegin(), hello.cend(), std::back_inserter(ordinals));
		binary_transform_example(std::move(ordinals));
	}

}

namespace replace {

	void println(const auto& seq) {
		for (const auto& e : seq)
			std::cout << e << ' ';
		std::cout << '\n';
	}

	void run() {
		std::array<int, 10> s{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};

		// Replace all occurrences of 8 with 88.
		plastic::replace(s.begin(), s.end(), 8, 88);
		println(s);

		// Replace all values less than 5 with 55.
		plastic::replace_if(s.begin(), s.end(),
			std::bind(std::less<int>(), std::placeholders::_1, 5), 55);
		println(s);

		std::array<std::complex<double>, 2> nums{{{1, 3}, {1, 3}}};
#ifdef __cpp_lib_algorithm_default_value_type
		plastic::replace(nums.begin(), nums.end(), {1, 3}, {4, 2});
#else
		plastic::replace(nums.begin(), nums.end(), std::complex<double>{1, 3},
			std::complex<double>{4, 2});
#endif
		println(nums);
	}

}

namespace replace_copy {

	void println(const auto& seq) {
		for (const auto& e : seq)
			std::cout << e << ' ';
		std::cout << '\n';
	}

	void run() {
		std::vector<short> src{3, 1, 4, 1, 5, 9, 2, 6, 5};
		println(src);
		std::vector<int> dst(src.size());
		plastic::replace_copy_if(src.cbegin(), src.cend(),
			dst.begin(),
			[](short n) { return n > 5; }, 0);
		println(dst);

		std::vector<std::complex<double>> src2{{1, 3}, {2, 4}, {3, 5}},
			dst2(src2.size());
		println(src2);
#ifdef __cpp_lib_algorithm_default_value_type
		plastic::replace_copy_if(src2.cbegin(), src2.cend(), dst2.begin(),
			[](std::complex<double> z) { return std::abs(z) < 5; },
			{4, 2}); // Possible, since the T is deduced.
#else
		plastic::replace_copy_if(src2.cbegin(), src2.cend(), dst2.begin(),
			[](std::complex<double> z) { return std::abs(z) < 5; },
			std::complex<double>{4, 2});
#endif
		println(dst2);
	}

}

namespace fill {

	void println(const auto& seq) {
		for (const auto& e : seq)
			std::cout << e << ' ';
		std::cout << '\n';
	}

	void run() {
		std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8};
		println(v);

		// set all of the elements to 8
		plastic::fill(v.begin(), v.end(), 8);
		println(v);

		std::vector<std::complex<double>> nums{{1, 3}, {2, 2}, {4, 8}};
		println(nums);
#ifdef __cpp_lib_algorithm_default_value_type
		plastic::fill(nums.begin(), nums.end(), {4, 2});
#else
		plastic::fill(nums.begin(), nums.end(), std::complex<double>{4, 2});
#endif
		println(nums);
	}

}

namespace fill_n {

	void run() {
		std::vector<int> v1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

		// replace values of the first 5 elements with -1
		plastic::fill_n(v1.begin(), 5, -1);

		std::copy_n(v1.cbegin(), v1.size(), std::ostream_iterator<int>(std::cout, " "));
		std::cout << '\n';

		std::vector<std::complex<double>> nums{{1, 3}, {2, 2}, {4, 8}};
#ifdef __cpp_lib_algorithm_default_value_type
		plastic::fill_n(nums.begin(), 2, {4, 2});
#else
		plastic::fill_n(nums.begin(), 2, std::complex<double>{4, 2});
#endif
		std::copy_n(nums.cbegin(), nums.size(),
			std::ostream_iterator<std::complex<double>>(std::cout, " "));
		std::cout << '\n';
	}

}

namespace generate {

	void println(std::string_view fmt, const auto& v) {
		for (std::cout << fmt; const auto & e : v)
			std::cout << e << ' ';
		std::cout << '\n';
	};


	int f() {
		static int i;
		return ++i;
	}

	void run() {
		std::vector<int> v(5);

		plastic::generate(v.begin(), v.end(), f);
		println("v: ", v);

		// Initialize with default values 0,1,2,3,4 from a lambda function
		// Equivalent to std::iota(v.begin(), v.end(), 0);
		plastic::generate(v.begin(), v.end(), [n = 0]() mutable { return n++; });
		println("v: ", v);
	}

}

namespace generate_n {

	void println(std::string_view fmt, const auto& v) {
		for (std::cout << fmt; const auto & e : v)
			std::cout << e << ' ';
		std::cout << '\n';
	};

	void run() {
		std::vector<int> v(5);
		println("v: ", v);
		plastic::generate(v.begin(), v.end(), [n = 0]() mutable { return n++; });
		println("v: ", v);
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

	test("count/count_if", count::run, R"(
v: 1 2 3 4 4 3 7 8 9 10
number: 3, count: 2
number: 4, count: 2
number: 5, count: 0
numbers divisible by four: 3
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

	test("copy/copy_if", copy::run, R"(
to_vector contains: 0 1 2 3 4 5 6 7 8 9
odd numbers in to_vector are: 1 3 5 7 9
to_vector contains these multiples of 3: 0 3 6 9
)");

	test("copy_n", copy_n::run, R"(
1234
5050
)");

	test("copy_backward", copy_backward::run, R"(
destination contains: 0 0 1 2 3 4
)");

	test("move", move::run, R"(
Non-overlapping case; before move:
src: foo bar baz
dst: qux quux quuz corge
After:
src: . . .
dst: foo bar baz corge
Overlapping case; before move:
src: snap crackle pop lock drop
After:
src: lock drop pop . .
)");

	test("move_backward", move_backward::run, R"(
Non-overlapping case; before move_backward:
src: foo bar baz
dst: qux quux quuz corge
After:
src: . . .
dst: qux foo bar baz
Overlapping case; before move_backward:
src: snap crackle pop lock drop
After:
src: . . snap crackle pop
)");

	test("swap_ranges", swap_ranges::run, R"(
Before swap_ranges:
v: a b c d e
l: 1 2 3 4 5
After swap_ranges:
v: 1 2 3 d e
l: a b c 4 5
)");

	test("iter_swap", iter_swap::run, R"(
5 2 3 4 1
)");

	test("transform", transform::run, R"(
hello = "HELLO"
world = "WORLD"
ordinals:  72  69  76  76  79 
ordinals: 144 138 152 152 158
)");

	test("replace/replace_if", replace::run, R"(
5 7 4 2 88 6 1 9 0 3
5 7 55 55 88 6 55 9 55 55
(4,2) (4,2)
)");

	test("replace_copy/replace_copy_if", replace_copy::run, R"(
3 1 4 1 5 9 2 6 5 
3 1 4 1 5 0 2 0 5 
(1,3) (2,4) (3,5) 
(4,2) (4,2) (3,5)
)");

	test("fill", fill::run, R"(
0 1 2 3 4 5 6 7 8
8 8 8 8 8 8 8 8 8
(1,3) (2,2) (4,8) 
(4,2) (4,2) (4,2)
)");

	test("fill_n", fill_n::run, R"(
-1 -1 -1 -1 -1 5 6 7 8 9
(4,2) (4,2) (4,8)
)");

	test("generate", generate::run, R"(
v: 1 2 3 4 5
v: 0 1 2 3 4
)");

	test("generate_n", generate_n::run, R"(
v: 0 0 0 0 0
v: 0 1 2 3 4
)");

}