#include "algorithm.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <array>
#include <vector>
#include <forward_list>
#include <algorithm>
#include <numeric>
#include <complex>
#include <cassert>

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
	size_t space{std::max(label.size() + 1, size_t{36})};
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

// most of the following testing code are taken from https://en.cppreference.com/w/cpp/algorithm

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

		for (const auto& w : {std::array{3, 1, 4}, std::array{1, 3, 5}})
			if (auto it = plastic::find_if(std::begin(w), std::end(w), is_even); it != std::end(w))
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

		for (auto const& x : {std::array{1, 2, 3}, std::array{4, 5, 6}}) {
			auto iter = plastic::find_end(v.begin(), v.end(), x.begin(), x.end()); // overload (1)
			print_result(iter, v);
		}

		for (auto const& x : {std::array{-1, -2, -3}, std::array{-4, -5, -6}}) {
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
				print_sequence("v = ", v, static_cast<int>(pos));
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
			const int num_items = static_cast<int>(plastic::count(v.cbegin(), v.cend(), target));
			std::cout << "number: " << target << ", count: " << num_items << '\n';
		}

		// Use a lambda expression to count elements divisible by 4.
		int count_div4 = static_cast<int>(plastic::count_if(v.begin(), v.end(), [](int i) { return i % 4 == 0; }));
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

namespace remove_test {

	void run() {
		std::string str1{"Text with some   spaces"};

		auto noSpaceEnd = plastic::remove(str1.begin(), str1.end(), ' ');

		// The spaces are removed from the string only logically.
		// Note, we use view, the original string is still not shrunk:
		std::cout << std::string_view(str1.begin(), noSpaceEnd)
			<< " size: " << str1.size() << '\n';

		str1.erase(noSpaceEnd, str1.end());

		// The spaces are removed from the string physically.
		std::cout << str1 << " size: " << str1.size() << '\n';

		std::string str2 = "Text\n with\tsome \t  whitespaces\n\n";
		str2.erase(plastic::remove_if(str2.begin(),
			str2.end(),
			[](unsigned char x) { return std::isspace(x); }),
			str2.end());
		std::cout << str2 << '\n';

		std::vector<std::complex<double>> nums{{2, 2}, {1, 3}, {4, 8}};
#ifdef __cpp_lib_algorithm_default_value_type
		nums.erase(plastic::remove(nums.begin(), nums.end(), {1, 3}), nums.end());
#else
		nums.erase(plastic::remove(nums.begin(), nums.end(), std::complex<double>{1, 3}),
			nums.end());
#endif
		assert((nums == std::vector<std::complex<double>>{{2, 2}, {4, 8}}));
	}

}

namespace remove_copy {

	void run() {
		// Erase the hash characters '#' on the fly.
		std::string str = "#Return #Value #Optimization";
		std::cout << "before: " << std::quoted(str) << '\n';

		std::cout << "after:  \"";
		plastic::remove_copy(str.begin(), str.end(),
			std::ostream_iterator<char>(std::cout), '#');
		std::cout << "\"\n";

		// Erase {1, 3} value on the fly.
		std::vector<std::complex<double>> nums{{2, 2}, {1, 3}, {4, 8}, {1, 3}};
		plastic::remove_copy(nums.begin(), nums.end(),
			std::ostream_iterator<std::complex<double>>(std::cout),
#ifdef __cpp_lib_algorithm_default_value_type
			{1, 3}); // T gets deduced
#else
			std::complex<double>{1, 3});
#endif
	}

}

namespace unique {

	void run() {
		// a vector containing several duplicate elements
		std::vector<int> v{1, 2, 1, 1, 3, 3, 3, 4, 5, 4};
		auto print = [&](int id) {
			std::cout << "@" << id << ": ";
			for (int i : v)
				std::cout << i << ' ';
			std::cout << '\n';
		};
		print(1);

		// remove consecutive (adjacent) duplicates
		auto last = plastic::unique(v.begin(), v.end());
		// v now holds {1 2 1 3 4 5 4 x x x}, where 'x' is indeterminate
		v.erase(last, v.end());
		print(2);

		// sort followed by unique, to remove all duplicates
		std::sort(v.begin(), v.end()); // {1 1 2 3 4 4 5}
		print(3);

		last = plastic::unique(v.begin(), v.end());
		// v now holds {1 2 3 4 5 x x}, where 'x' is indeterminate
		v.erase(last, v.end());
		print(4);
	}

}

namespace unique_copy {

	void run() {
		std::string s1{"A string with mmmany letters!"};
		std::cout << "Before: " << s1 << '\n';

		std::string s2;
		plastic::unique_copy(s1.begin(), s1.end(), std::back_inserter(s2),
			[](char c1, char c2) { return c1 == 'm' && 'm' == c2; });

		std::cout << "After:  " << s2 << '\n';
	}

}

namespace reverse {

	void println(auto rem, auto const& v) {
		for (std::cout << rem; auto e : v)
			std::cout << e << ' ';
		std::cout << '\n';
	}

	void run() {
		std::vector<int> v{1, 2, 3};
		plastic::reverse(v.begin(), v.end());
		println("after reverse, v = ", v);

		int a[] = {4, 5, 6, 7};
		plastic::reverse(std::begin(a), std::end(a));
		println("after reverse, a = ", a);
	}

}

namespace reverse_copy {

	void run() {
		auto print = [](const std::vector<int>& v) {
			for (const auto& value : v)
				std::cout << value << ' ';
			std::cout << '\n';
		};

		std::vector<int> v{1, 2, 3};
		print(v);

		std::vector<int> destination(3);
		plastic::reverse_copy(std::begin(v), std::end(v), std::begin(destination));
		print(destination);

		plastic::reverse_copy(std::rbegin(v), std::rend(v), std::begin(destination));
		print(destination);
	}

}

namespace rotate {

	auto print = [](const auto remark, const auto& v) {
		std::cout << remark;
		for (auto n : v)
			std::cout << n << ' ';
		std::cout << '\n';
	};

	void run() {
		std::vector<int> v{2, 4, 2, 0, 5, 10, 7, 3, 7, 1};
		print("before sort:\t\t", v);

		// insertion sort
		for (auto i = v.begin(); i != v.end(); ++i)
			plastic::rotate(std::upper_bound(v.begin(), i, *i), i, i + 1);
		print("after sort:\t\t", v);

		// simple rotation to the left
		plastic::rotate(v.begin(), v.begin() + 1, v.end());
		print("simple rotate left:\t", v);

		// simple rotation to the right
		plastic::rotate(v.rbegin(), v.rbegin() + 1, v.rend());
		print("simple rotate right:\t", v);
	}

}

namespace rotate_copy {

	void run() {
		std::vector<int> src{1, 2, 3, 4, 5};
		std::vector<int> dest(src.size());
		auto pivot = std::find(src.begin(), src.end(), 3);

		plastic::rotate_copy(src.begin(), pivot, src.end(), dest.begin());
		for (int i : dest)
			std::cout << i << ' ';
		std::cout << '\n';

		// copy the rotation result directly to the std::cout
		pivot = std::find(dest.begin(), dest.end(), 1);
		plastic::rotate_copy(dest.begin(), pivot, dest.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << '\n';
	}

}

namespace shift_left {

	struct S {
		int value{0};
		bool specified_state{true};

		S(int v = 0) : value{v} {}
		S(S const& rhs) = default;
		S(S&& rhs) {
			*this = std::move(rhs);
		}
		S& operator=(S const& rhs) = default;
		S& operator=(S&& rhs) {
			if (this != &rhs) {
				value = rhs.value;
				specified_state = rhs.specified_state;
				rhs.specified_state = false;
			}
			return *this;
		}
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& os, std::vector<T> const& v) {
		for (const auto& s : v) {
			if constexpr (std::is_same_v<T, S>)
				s.specified_state ? os << s.value << ' ' : os << ". ";
			else if constexpr (std::is_same_v<T, std::string>)
				os << (s.empty() ? "." : s) << ' ';
			else
				os << s << ' ';
		}
		return os;
	}

	void run() {
		std::cout << std::left;

		std::vector<S>           a{1, 2, 3, 4, 5, 6, 7};
		std::vector<std::string> c{"¦Á", "¦Â", "¦Ã", "¦Ä", "¦Å", "¦Æ", "¦Ç"};

		std::cout << "vector<S> \tvector<string>\n";
		std::cout << a << "  " << c << '\n';

		plastic::shift_left(begin(a), end(a), 3);
		plastic::shift_left(begin(c), end(c), 3);
		std::cout << a << "  " << c << '\n';

		plastic::shift_right(begin(a), end(a), 2);
		plastic::shift_right(begin(c), end(c), 2);
		std::cout << a << "  " << c << '\n';

		plastic::shift_left(begin(a), end(a), 8); // has no effect: n >= last - first
		plastic::shift_left(begin(c), end(c), 8); // ditto
		std::cout << a << "  " << c << '\n';

		//  std::shift_left(begin(a), end(a), -3); // UB, e.g. segfault
	}

}

namespace shuffle {

	void run() {
		std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		std::mt19937 g{std::random_device{}()};
		plastic::shuffle(v.begin(), v.end(), g);

		std::string in{"ABCDEFGHIJK"}, out;
		plastic::sample(in.begin(), in.end(), std::back_inserter(out), 4, g);

		std::cout << "No Compile Error.";
	}

}

namespace is_partitioned {

	void run() {
		std::array<int, 9> v{1, 2, 3, 4, 5, 6, 7, 8, 9};

		auto is_even = [](int i) { return i % 2 == 0; };
		std::cout.setf(std::ios_base::boolalpha);
		std::cout << plastic::is_partitioned(v.begin(), v.end(), is_even) << ' ';

		std::partition(v.begin(), v.end(), is_even);
		std::cout << plastic::is_partitioned(v.begin(), v.end(), is_even) << ' ';

		std::reverse(v.begin(), v.end());
		std::cout << plastic::is_partitioned(v.cbegin(), v.cend(), is_even) << ' ';
		std::cout << plastic::is_partitioned(v.crbegin(), v.crend(), is_even) << '\n';
	}

}

namespace partition {

	template<class ForwardIt>
	void quicksort(ForwardIt first, ForwardIt last) {
		if (first == last)
			return;

		auto pivot = *std::next(first, std::distance(first, last) / 2);
		auto middle1 = plastic::partition(first, last, [pivot](const auto& em) {
			return em < pivot;
		});
		auto middle2 = plastic::partition(middle1, last, [pivot](const auto& em) {
			return !(pivot < em);
		});

		quicksort(first, middle1);
		quicksort(middle2, last);
	}

	void run() {
		std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		std::cout << "Original vector: ";
		for (int elem : v)
			std::cout << elem << ' ';

		auto it = plastic::partition(v.begin(), v.end(), [](int i) {return i % 2 == 0; });

		std::forward_list<int> fl{1, 30, -4, 3, 5, -4, 1, 6, -8, 2, -5, 64, 1, 92};
		std::cout << "\nUnsorted list: ";
		for (int n : fl)
			std::cout << n << ' ';

		quicksort(std::begin(fl), std::end(fl));
		std::cout << "\nSorted using quicksort: ";
		for (int fi : fl)
			std::cout << fi << ' ';
		std::cout << '\n';
	}

}

namespace partition_copy {

	void print(auto rem, const auto& v) {
		for (std::cout << rem; const auto & x : v)
			std::cout << x << ' ';
		std::cout << '\n';
	}

	void run() {
		int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		int true_arr[5] = {0};
		int false_arr[5] = {0};

		plastic::partition_copy(std::begin(arr), std::end(arr),
			std::begin(true_arr), std::begin(false_arr),
			[](int i) { return 4 < i; });

		print("true_arr:  ", true_arr);
		print("false_arr: ", false_arr);
	}

}

namespace stable_partition {

	void run() {
		std::vector<int> v{0, 0, 3, -1, 2, 4, 5, 0, 7};
		plastic::stable_partition(v.begin(), v.end(), [](int n) { return n > 0; });
		for (int n : v)
			std::cout << n << ' ';
		std::cout << '\n';
	}

}

namespace partition_point {

	auto print_seq = [](auto rem, auto first, auto last) {
		for (std::cout << rem; first != last; std::cout << *first++ << ' ') {
		}
		std::cout << '\n';
	};

	void run() {
		std::array v{1, 2, 3, 4, 5, 6, 7, 8, 9};

		auto is_even = [](int i) { return i % 2 == 0; };

		std::partition(v.begin(), v.end(), is_even);
		print_seq("After partitioning, v: ", v.cbegin(), v.cend());

		const auto pp = plastic::partition_point(v.cbegin(), v.cend(), is_even);
		const auto i = std::distance(v.cbegin(), pp);
		std::cout << "Partition point is at " << i << "; v[" << i << "] = " << *pp << '\n';

		print_seq("First partition (all even elements): ", v.cbegin(), pp);
		print_seq("Second partition (all odd elements): ", pp, v.cend());
	}

}

namespace is_sorted {

	void run() {
		int data[] = {3, 1, 4, 1, 5};
		if (plastic::is_sorted(std::begin(data), std::end(data))) {
			std::cout << "Not Sorted.\n";
		}
		std::sort(std::begin(data), std::end(data));
		if (!plastic::is_sorted(std::begin(data), std::end(data))) {
			std::cout << "Sorted.\n";
		}
		if (plastic::is_sorted(std::begin(data), std::end(data), std::greater<>{})) {
			std::cout << "Not Sorted On Relation std::greater<>.\n";
		}
		std::cout << "Test End.";
	}

}

namespace nth_element {

	void run() {
		std::vector<int> v{5, 10, 6, 4, 3, 2, 6, 7, 9, 3};
		auto m = v.begin() + v.size() / 2;
		plastic::nth_element(v.begin(), m, v.end());
		std::cout << "\nThe median is " << v[v.size() / 2] << '\n';
	}

}

namespace lower_bound {

	struct PriceInfo {
		double price;
	};

	void run() {
		const std::vector<int> data{1, 2, 4, 5, 5, 6};

		for (int i = 0; i < 8; ++i) {
			// Search for first element x such that i ¡Ü x
			auto lower = plastic::lower_bound(data.begin(), data.end(), i);

			std::cout << i << " ¡Ü ";
			lower != data.end()
				? std::cout << *lower << " at index " << std::distance(data.begin(), lower)
				: std::cout << "not found";
			std::cout << '\n';
		}

		std::vector<PriceInfo> prices{{100.0}, {101.5}, {102.5}, {102.5}, {107.3}};

		for (const double to_find : {102.5, 110.2}) {
			auto prc_info = plastic::lower_bound(prices.begin(), prices.end(), to_find,
				[](const PriceInfo& info, double value) {
				return info.price < value;
			});

			prc_info != prices.end()
				? std::cout << prc_info->price << " at index " << prc_info - prices.begin()
				: std::cout << to_find << " not found";
			std::cout << '\n';
		}

		using CD = std::complex<double>;
		std::vector<CD> nums{{1, 0}, {2, 2}, {2, 1}, {3, 0}};
		auto cmpz = [](CD x, CD y) { return x.real() < y.real(); };
#ifdef __cpp_lib_algorithm_default_value_type
		auto it = plastic::lower_bound(nums.cbegin(), nums.cend(), {2, 0}, cmpz);
#else
		auto it = plastic::lower_bound(nums.cbegin(), nums.cend(), CD{2, 0}, cmpz);
#endif
		assert((*it == CD{2, 2}));
	}

}

namespace upper_bound {

	struct PriceInfo {
		double price;
	};

	void run() {
		const std::vector<int> data{1, 2, 4, 5, 5, 6};

		for (int i = 0; i < 7; ++i) {
			// Search first element that is greater than i
			auto upper = plastic::upper_bound(data.begin(), data.end(), i);

			std::cout << i << " < ";
			upper != data.end()
				? std::cout << *upper << " at index " << std::distance(data.begin(), upper)
				: std::cout << "not found";
			std::cout << '\n';
		}

		std::vector<PriceInfo> prices{{100.0}, {101.5}, {102.5}, {102.5}, {107.3}};

		for (double to_find : {102.5, 110.2}) {
			auto prc_info = plastic::upper_bound(prices.begin(), prices.end(), to_find,
				[](double value, const PriceInfo& info) {
				return value < info.price;
			});

			prc_info != prices.end()
				? std::cout << prc_info->price << " at index " << prc_info - prices.begin()
				: std::cout << to_find << " not found";
			std::cout << '\n';
		}

		using CD = std::complex<double>;
		std::vector<CD> nums{{1, 0}, {2, 2}, {2, 1}, {3, 0}, {3, 1}};
		auto cmpz = [](CD x, CD y) { return x.real() < y.real(); };
#ifdef __cpp_lib_algorithm_default_value_type
		auto it = plastic::upper_bound(nums.cbegin(), nums.cend(), {2, 0}, cmpz);
#else
		auto it = plastic::upper_bound(nums.cbegin(), nums.cend(), CD{2, 0}, cmpz);
#endif
		assert((*it == CD{3, 0}));
	}

}

namespace equal_range {

	struct S {
		int number;
		char name;
		// note: name is ignored by this comparison operator
		bool operator<(const S& s) const {
			return number < s.number;
		}
	};

	struct Comp {
		bool operator()(const S& s, int i) const {
			return s.number < i;
		}
		bool operator()(int i, const S& s) const {
			return i < s.number;
		}
	};

	void run() {
		// note: not ordered, only partitioned w.r.t. S defined below
		const std::vector<S> vec{{1, 'A'}, {2, 'B'}, {2, 'C'},
								 {2, 'D'}, {4, 'G'}, {3, 'F'}};
		const S value{2, '?'};

		std::cout << "Compare using S::operator<(): ";
		const auto p = plastic::equal_range(vec.begin(), vec.end(), value);

		for (auto it = p.first; it != p.second; ++it)
			std::cout << it->name << ' ';
		std::cout << '\n';

		std::cout << "Using heterogeneous comparison: ";
		const auto p2 = plastic::equal_range(vec.begin(), vec.end(), 2, Comp{});

		for (auto it = p2.first; it != p2.second; ++it)
			std::cout << it->name << ' ';
		std::cout << '\n';

		using CD = std::complex<double>;
		std::vector<CD> nums{{1, 0}, {2, 2}, {2, 1}, {3, 0}, {3, 1}};
		auto cmpz = [](CD x, CD y) { return x.real() < y.real(); };
#ifdef __cpp_lib_algorithm_default_value_type
		auto p3 = plastic::equal_range(nums.cbegin(), nums.cend(), {2, 0}, cmpz);
#else
		auto p3 = plastic::equal_range(nums.cbegin(), nums.cend(), CD{2, 0}, cmpz);
#endif

		for (auto it = p3.first; it != p3.second; ++it)
			std::cout << *it << ' ';
		std::cout << '\n';
	}

}

namespace binary_search {

	void run() {
		const auto haystack = {1, 3, 4, 5, 9};

		for (const auto needle : {1, 2, 3}) {
			std::cout << "Searching for " << needle << '\n';
			if (plastic::binary_search(haystack.begin(), haystack.end(), needle))
				std::cout << "Found " << needle << '\n';
			else
				std::cout << "No dice!\n";
		}

		using CD = std::complex<double>;
		std::vector<CD> nums{{1, 1}, {2, 3}, {4, 2}, {4, 3}};
		auto cmpz = [](CD x, CD y) { return abs(x) < abs(y); };
#ifdef __cpp_lib_algorithm_default_value_type
		assert(plastic::binary_search(nums.cbegin(), nums.cend(), {4, 2}, cmpz));
#else
		assert(plastic::binary_search(nums.cbegin(), nums.cend(), CD{4, 2}, cmpz));
#endif
	}

}

namespace includes {

	template<class Os, class Co>
	Os& operator<<(Os& os, const Co& v) {
		for (const auto& i : v)
			os << i << ' ';
		return os << '\t';
	}

	void run() {
		const auto
			v1 = {'a', 'b', 'c', 'f', 'h', 'x'},
			v2 = {'a', 'b', 'c'},
			v3 = {'a', 'c'},
			v4 = {'a', 'a', 'b'},
			v5 = {'g'},
			v6 = {'a', 'c', 'g'},
			v7 = {'A', 'B', 'C'};

		auto no_case = [](char a, char b) { return std::tolower(a) < std::tolower(b); };

		std::cout
			<< v1 << "\nincludes:\n" << std::boolalpha
			<< v2 << ": " << plastic::includes(v1.begin(), v1.end(), v2.begin(), v2.end()) << '\n'
			<< v3 << ": " << plastic::includes(v1.begin(), v1.end(), v3.begin(), v3.end()) << '\n'
			<< v4 << ": " << plastic::includes(v1.begin(), v1.end(), v4.begin(), v4.end()) << '\n'
			<< v5 << ": " << plastic::includes(v1.begin(), v1.end(), v5.begin(), v5.end()) << '\n'
			<< v6 << ": " << plastic::includes(v1.begin(), v1.end(), v6.begin(), v6.end()) << '\n'
			<< v7 << ": " << plastic::includes(v1.begin(), v1.end(), v7.begin(), v7.end(), no_case)
			<< " (case-insensitive)\n";
	}

}

namespace set_union {

	void println(const std::vector<int>& v) {
		for (int i : v)
			std::cout << i << ' ';
		std::cout << '\n';
	}

	void run() {
		std::vector<int> v1, v2, dest;

		v1 = {1, 2, 3, 4, 5};
		v2 = {3, 4, 5, 6, 7};

		plastic::set_union(v1.cbegin(), v1.cend(),
			v2.cbegin(), v2.cend(),
			std::back_inserter(dest));
		println(dest);

		dest.clear();

		v1 = {1, 2, 3, 4, 5, 5, 5};
		v2 = {3, 4, 5, 6, 7};

		plastic::set_union(v1.cbegin(), v1.cend(),
			v2.cbegin(), v2.cend(),
			std::back_inserter(dest));
		println(dest);
	}

}

namespace set_intersection {

	void run() {
		std::vector<int> v1{7, 2, 3, 4, 5, 6, 7, 8};
		std::vector<int> v2{5, 7, 9, 7};
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());

		std::vector<int> v_intersection;
		plastic::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
			std::back_inserter(v_intersection));

		for (int n : v_intersection)
			std::cout << n << ' ';
		std::cout << '\n';
	}

}

namespace set_difference {

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
		os << '{';
		for (auto n{v.size()}; const auto & e : v)
			os << e << (--n ? ", " : "");
		return os << '}';
	}

	struct Order // a struct with very interesting data
	{
		int order_id{};

		friend std::ostream& operator<<(std::ostream& os, const Order& ord) {
			return os << ord.order_id;
		}
	};

	void run() {
		const std::vector<int> v1{1, 2, 5, 5, 5, 9};
		const std::vector<int> v2{2, 5, 7};
		std::vector<int> diff;

		plastic::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),
			std::inserter(diff, diff.begin()));

		std::cout << v1 << " \\ " << v2 << " == " << diff << "\n\n";

		// we want to know which orders "cut" between old and new states:
		std::vector<Order> old_orders{{1}, {2}, {5}, {9}};
		std::vector<Order> new_orders{{2}, {5}, {7}};
		std::vector<Order> cut_orders;

		plastic::set_difference(old_orders.begin(), old_orders.end(),
			new_orders.begin(), new_orders.end(),
			std::back_inserter(cut_orders),
			[](auto& a, auto& b) { return a.order_id < b.order_id; });

		std::cout << "old orders: " << old_orders << '\n'
			<< "new orders: " << new_orders << '\n'
			<< "cut orders: " << cut_orders << '\n';
	}

}

namespace set_symmetric_difference {

	void run() {
		std::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8};
		std::vector<int> v2{5, 7, 9, 10};
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());

		std::vector<int> v_symDifference;

		plastic::set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),
			std::back_inserter(v_symDifference));

		for (int n : v_symDifference)
			std::cout << n << ' ';
		std::cout << '\n';
	}

}

namespace merge {

	void run() {
		std::vector<int> v1{1, 3, 5, 7, 9}, v2{2, 4, 6, 8, 10}, v3;
		plastic::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));
		for (auto& i : v3) {
			std::cout << i << ' ';
		}
	}

}

namespace inplace_merge {

	template<class Iter>
	void merge_sort(Iter first, Iter last) {
		if (last - first > 1) {
			Iter middle = first + (last - first) / 2;
			merge_sort(first, middle);
			merge_sort(middle, last);
			plastic::inplace_merge(first, middle, last);
		}
	}

	void run() {
		std::vector<int> v{8, 2, -2, 0, 11, 11, 1, 7, 3};
		merge_sort(v.begin(), v.end());
		for (const auto& n : v)
			std::cout << n << ' ';
		std::cout << '\n';
	}

}

namespace push_heap {

	void println(std::string_view rem, const std::vector<int>& v) {
		std::cout << rem;
		for (int e : v)
			std::cout << e << ' ';
		std::cout << '\n';
	}

	void run() {
		std::vector<int> v{3, 1, 4, 1, 5, 9};

		std::make_heap(v.begin(), v.end());
		println("after make_heap: ", v);

		v.push_back(6);
		println("after push_back: ", v);

		plastic::push_heap(v.begin(), v.end());
		println("after push_heap: ", v);
	}

}

namespace pop_heap {

	void println(std::string_view rem, const auto& v) {
		std::cout << rem;
		if constexpr (std::is_scalar_v<std::decay_t<decltype(v)>>)
			std::cout << v;
		else
			for (int e : v)
				std::cout << e << ' ';
		std::cout << '\n';
	}

	void run() {
		std::vector<int> v{3, 1, 4, 1, 5, 9};

		std::make_heap(v.begin(), v.end());
		println("after make_heap: ", v);

		plastic::pop_heap(v.begin(), v.end()); // moves the largest to the end
		println("after pop_heap:  ", v);

		int largest = v.back();
		println("largest element: ", largest);

		v.pop_back(); // actually removes the largest element
		println("after pop_back:  ", v);
	}

}

namespace make_heap {

	void print(std::string_view text, const std::vector<int>& v = {}) {
		std::cout << text << ": ";
		for (const auto& e : v)
			std::cout << e << ' ';
		std::cout << '\n';
	}

	void run() {
		print("Max heap");

		std::vector<int> v{3, 2, 4, 1, 5, 9};
		print("initially, v", v);

		plastic::make_heap(v.begin(), v.end());
		print("after make_heap, v", v);

		std::pop_heap(v.begin(), v.end());
		print("after pop_heap, v", v);

		auto top = v.back();
		v.pop_back();
		print("former top element", {top});
		print("after removing the former top element, v", v);

		print("\nMin heap");

		std::vector<int> v1{3, 2, 4, 1, 5, 9};
		print("initially, v1", v1);

		plastic::make_heap(v1.begin(), v1.end(), std::greater<>{});
		print("after make_heap, v1", v1);

		std::pop_heap(v1.begin(), v1.end(), std::greater<>{});
		print("after pop_heap, v1", v1);

		auto top1 = v1.back();
		v1.pop_back();
		print("former top element", {top1});
		print("after removing the former top element, v1", v1);
	}

}

namespace sort_heap {

	void println(std::string_view fmt, const auto& v) {
		for (std::cout << fmt; const auto & i : v)
			std::cout << i << ' ';
		std::cout << '\n';
	}

	void run() {
		std::vector<int> v{3, 1, 4, 1, 5, 9};

		std::make_heap(v.begin(), v.end());
		println("after make_heap, v: ", v);

		plastic::sort_heap(v.begin(), v.end());
		println("after sort_heap, v: ", v);
	}

}

namespace is_heap {

	void run() {
		std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9};

		std::cout << "initially, v:\n";
		for (const auto& i : v)
			std::cout << i << ' ';
		std::cout << '\n';

		if (!plastic::is_heap(v.begin(), v.end())) {
			std::cout << "making heap...\n";
			std::make_heap(v.begin(), v.end());
		}

		std::cout << "after make_heap, v:\n";
		for (auto t{1U}; const auto & i : v)
			std::cout << i << (std::has_single_bit(++t) ? " | " : " ");
		std::cout << '\n';
	}

}

namespace is_heap_until {

	void run() {
		std::vector<int> v{3, 1, 4, 1, 5, 9};

		std::make_heap(v.begin(), v.end());

		// probably mess up the heap
		v.push_back(2);
		v.push_back(6);

		auto heap_end = plastic::is_heap_until(v.begin(), v.end());

		std::cout << "all of v:  ";
		for (const auto& i : v)
			std::cout << i << ' ';
		std::cout << '\n';

		std::cout << "only heap: ";
		for (auto i = v.begin(); i != heap_end; ++i)
			std::cout << *i << ' ';
		std::cout << '\n';
	}

}

namespace sort {

	void run() {
		std::array<int, 10> s{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};

		auto print = [&s](std::string_view const rem) {
			for (auto a : s)
				std::cout << a << ' ';
			std::cout << ": " << rem << '\n';
		};

		plastic::sort(s.begin(), s.end());
		print("sorted with the default operator<");

		plastic::sort(s.begin(), s.end(), std::greater<int>());
		print("sorted with the standard library compare function object");

		struct {
			bool operator()(int a, int b) const {
				return a < b;
			}
		}
		customLess;

		plastic::sort(s.begin(), s.end(), customLess);
		print("sorted with a custom function object");

		plastic::sort(s.begin(), s.end(), [](int a, int b) {
			return a > b;
		});
		print("sorted with a lambda expression");
	}

}

namespace stable_sort {

	struct Employee {
		int age;
		std::string name; // Does not participate in comparisons
	};

	bool operator<(const Employee& lhs, const Employee& rhs) {
		return lhs.age < rhs.age;
	}

#if __cpp_lib_constexpr_algorithms >= 202306L
	consteval auto get_sorted() {
		auto v = std::array{3, 1, 4, 1, 5, 9};
		plastic::stable_sort(v.begin(), v.end());
		return v;
	}
	static_assert(std::ranges::is_sorted(get_sorted()));
#endif

	void run() {
		std::vector<Employee> v{{108, "Zaphod"}, {32, "Arthur"}, {108, "Ford"}};

		plastic::stable_sort(v.begin(), v.end());

		for (const Employee& e : v)
			std::cout << e.age << ", " << e.name << '\n';
	}

}

namespace partial_sort {

	void run() {
		std::array<int, 10> s{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
		for (auto& i : s) {
			std::cout << i << ' ';
		}
		std::cout << '\n';
		plastic::partial_sort(s.begin(), s.begin() + 5, s.end());
		for (int i = 0; i < 5; ++i) {
			std::cout << s[i] << ' ';
		}
		std::cout << '\n';
	}

}

namespace partial_sort_copy {

	void println(std::string_view rem, const auto& v) {
		std::cout << rem;
		if constexpr (std::is_scalar_v<std::decay_t<decltype(v)>>)
			std::cout << v;
		else
			for (int e : v)
				std::cout << e << ' ';
		std::cout << '\n';
	}

	void run() {
		const auto v0 = {4, 2, 5, 1, 3};
		std::vector<int> v1{10, 11, 12};
		std::vector<int> v2{10, 11, 12, 13, 14, 15, 16};
		std::vector<int>::iterator it;

		it = plastic::partial_sort_copy(v0.begin(), v0.end(), v1.begin(), v1.end());
		println("Writing to the smaller vector in ascending order gives: ", v1);

		if (it == v1.end())
			println("The return value is the end iterator", ' ');

		it = plastic::partial_sort_copy(v0.begin(), v0.end(), v2.begin(), v2.end(),
			std::greater<int>());

		println("Writing to the larger vector in descending order gives: ", v2);
		println("The return value is the iterator to ", *it);
	}

}

namespace max_element {

	void run() {
		std::vector<int> v{3, 1, -14, 1, 5, 9, -14, 9};
		std::vector<int>::iterator result;

		result = plastic::max_element(v.begin(), v.end());
		std::cout << "Max element found at index "
			<< std::distance(v.begin(), result)
			<< " has value " << *result << '\n';

		result = plastic::max_element(v.begin(), v.end(), [](int a, int b) {
			return std::abs(a) < std::abs(b);
		});
		std::cout << "Absolute max element found at index "
			<< std::distance(v.begin(), result)
			<< " has value " << *result << '\n';
	}

}

namespace min_element {

	void run() {
		std::vector<int> v{3, 1, -4, 1, 5, 9};

		std::vector<int>::iterator result = plastic::min_element(v.begin(), v.end());
		std::cout << "min element has value " << *result << " and index ["
			<< std::distance(v.begin(), result) << "]\n";
	}

}

namespace minmax_element {

	void run() {
		const auto v = {3, 9, 1, 4, 2, 5, 9};
		const auto [min, max] = plastic::minmax_element(begin(v), end(v));

		std::cout << "min = " << *min << ", max = " << *max << '\n';
	}

}

namespace max {

	void run() {
		auto longest = [](const std::string_view s1, const std::string_view s2) {
			return s1.size() < s2.size();
		};

		std::cout << "Larger of 69 and 96 is " << plastic::max(69, 96) << "\n"
			"Larger of 'q' and 'p' is '" << plastic::max('q', 'p') << "'\n"
			"Largest of 010, 10, 0X10, and 0B10 is "
			<< plastic::max({010, 10, 0X10, 0B10}) << '\n'
			<< R"(Longest of "long", "short", and "int" is )"
			<< std::quoted(plastic::max({"long", "short", "int"}, longest)) << '\n';
	}

}

namespace min {

	void run() {
		std::cout << "smaller of 10 and 010 is " << plastic::min(10, 010) << '\n'
			<< "smaller of 'd' and 'b' is '" << plastic::min('d', 'b') << "'\n"
			<< "shortest of \"foo\", \"bar\", and \"hello\" is \""
			<< plastic::min({"foo", "bar", "hello"},
				[](const std::string_view s1, const std::string_view s2) {
			return s1.size() < s2.size();
		}) << "\"\n";
	}

}

namespace minmax {

	void run() {
		auto bounds = plastic::minmax({3, 1, 4, 1, 5, 9, 2, 6});
		std::cout << '(' << bounds.first << ',' << bounds.second << ')';
	}

}

namespace clamp {

	void run() {
		std::cout << "[raw] "
			"[" << static_cast<int>(INT8_MIN) << ',' << static_cast<int>(INT8_MAX) << "] "
			"[0," << static_cast<int>(UINT8_MAX) << "]\n";

		for (const int v : {-129, -128, -1, 0, 42, 127, 128, 255, 256})
			std::cout << std::setw(4) << v
			<< std::setw(11) << std::clamp(v, static_cast<int>(INT8_MIN), static_cast<int>(INT8_MAX))
			<< std::setw(8) << std::clamp(v, 0, static_cast<int>(UINT8_MAX)) << '\n';
	}

}

namespace lexicographical_compare {

	void run() {
		std::vector<char> v1{'a', 'b', 'c', 'd'};
		std::vector<char> v2{'a', 'c', 'c', 'd'};

		std::cout << std::boolalpha << plastic::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end());
	}

}

namespace lexicographical_compare_three_way {

	using namespace std::literals;

	void show_result(std::string_view s1, std::string_view s2, std::strong_ordering o) {
		std::cout << std::quoted(s1) << " is ";
		std::is_lt(o) ? std::cout << "less than " :
			std::is_gt(o) ? std::cout << "greater than " :
			std::cout << "equal to ";
		std::cout << std::quoted(s2) << '\n';
	}

	std::strong_ordering cmp_icase(unsigned char x, unsigned char y) {
		return std::toupper(x) <=> std::toupper(y);
	};

	void run() {
		for (const auto& [s1, s2] : {std::pair{"one"sv, "ONE"sv}, std::pair{"two"sv, "four"sv}, std::pair{"three"sv, "two"sv}}) {
			const auto res = plastic::lexicographical_compare_three_way(
				s1.cbegin(), s1.cend(), s2.cbegin(), s2.cend(), cmp_icase);
			show_result(s1, s2, res);
		}
	}

}

namespace next_permutation {

	void run() {
		std::string s = "aba";
		do {
			std::cout << s << '\n';
		} while (plastic::next_permutation(s.begin(), s.end()));

		std::cout << s << '\n';
	}

}

namespace prev_permutation {

	void run() {
		std::string s = "cab";
		do {
			std::cout << s << ' ';
		} while (plastic::prev_permutation(s.begin(), s.end()));

		std::cout << s << '\n';
	}

}

namespace is_permutation {

	template<typename Os, typename V>
	Os& operator<<(Os& os, const V& v) {
		os << "{ ";
		for (const auto& e : v)
			os << e << ' ';
		return os << '}';
	}

	void run() {
		static constexpr auto v1 = {1, 2, 3, 4, 5};
		static constexpr auto v2 = {3, 5, 4, 1, 2};
		static constexpr auto v3 = {3, 5, 4, 1, 1};

		std::cout << v2 << " is a permutation of " << v1 << ": " << std::boolalpha
			<< plastic::is_permutation(v1.begin(), v1.end(), v2.begin()) << '\n'
			<< v3 << " is a permutation of " << v1 << ": "
			<< plastic::is_permutation(v1.begin(), v1.end(), v3.begin()) << '\n';
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

	test("remove/remove_if", remove_test::run, R"(
Textwithsomespaces size: 23
Textwithsomespaces size: 18
Textwithsomewhitespaces
)");

	test("remove_copy/remove_copy_if", remove_copy::run, R"(
before: "#Return #Value #Optimization"
after:  "Return Value Optimization"
(2,2)(4,8)
)");

	test("unique", unique::run, R"(
@1: 1 2 1 1 3 3 3 4 5 4
@2: 1 2 1 3 4 5 4
@3: 1 1 2 3 4 4 5
@4: 1 2 3 4 5
)");

	test("unique_copy", unique_copy::run, R"(
Before: A string with mmmany letters!
After:  A string with many letters!
)");

	test("reverse", reverse::run, R"(
after reverse, v = 3 2 1
after reverse, a = 7 6 5 4
)");

	test("reverse_copy", reverse_copy::run, R"(
1 2 3 
3 2 1 
1 2 3
)");

	test("rotate", rotate::run, R"(
before sort:		2 4 2 0 5 10 7 3 7 1
after sort:		0 1 2 2 3 4 5 7 7 10
simple rotate left:	1 2 2 3 4 5 7 7 10 0
simple rotate right:	0 1 2 2 3 4 5 7 7 10
)");

	test("rotate_copy", rotate_copy::run, R"(
3 4 5 1 2
1 2 3 4 5
)");

	test("shift_left/shift_right", shift_left::run, R"(
vector<S>       vector<string>
1 2 3 4 5 6 7   ¦Á ¦Â ¦Ã ¦Ä ¦Å ¦Æ ¦Ç
4 5 6 7 . . .   ¦Ä ¦Å ¦Æ ¦Ç . . .
. . 4 5 6 7 .   . . ¦Ä ¦Å ¦Æ ¦Ç .
. . 4 5 6 7 .   . . ¦Ä ¦Å ¦Æ ¦Ç .
)");

	test("shuffle/sample", shuffle::run, R"(
No Compile Error.
)");

	test("is_partitioned", is_partitioned::run, R"(
false true false true
)");

	test("partition", partition::run, R"(
Original vector: 0 1 2 3 4 5 6 7 8 9
Unsorted list: 1 30 -4 3 5 -4 1 6 -8 2 -5 64 1 92
Sorted using quicksort: -8 -5 -4 -4 1 1 1 2 3 5 6 30 64 92
)");

	test("partition_copy", partition_copy::run, R"(
true_arr:  5 6 7 8 9
false_arr: 0 1 2 3 4
)");

	test("stable_partition", stable_partition::run, R"(
3 2 4 5 7 0 0 -1 0
)");

	test("partition_point", partition_point::run, R"(
After partitioning, v: 8 2 6 4 5 3 7 1 9
Partition point is at 4; v[4] = 5
First partition (all even elements): 8 2 6 4
Second partition (all odd elements): 5 3 7 1 9
)");

	test("is_sorted/is_sorted_until", is_sorted::run, R"(
Test End.
)");

	test("nth_element", nth_element::run, R"(
The median is 6
)");

	test("lower_bound", lower_bound::run, R"(
0 ¡Ü 1 at index 0
1 ¡Ü 1 at index 0
2 ¡Ü 2 at index 1
3 ¡Ü 4 at index 2
4 ¡Ü 4 at index 2
5 ¡Ü 5 at index 3
6 ¡Ü 6 at index 5
7 ¡Ü not found
102.5 at index 2
110.2 not found
)");

	test("upper_bound", upper_bound::run, R"(
0 < 1 at index 0
1 < 2 at index 1
2 < 4 at index 2
3 < 4 at index 2
4 < 5 at index 3
5 < 6 at index 5
6 < not found 
107.3 at index 4
110.2 not found
)");

	test("equal_range", equal_range::run, R"(
Compare using S::operator<(): B C D 
Using heterogeneous comparison: B C D
(2,2) (2, 1)
)");

	test("binary_search", binary_search::run, R"(
Searching for 1
Found 1
Searching for 2
No dice!
Searching for 3
Found 3
)");

	test("includes", includes::run, R"(
a b c f h x
includes:
a b c   : true
a c     : true
a a b   : false
g       : false
a c g   : false
A B C   : true (case-insensitive)
)");

	test("set_union", set_union::run, R"(
1 2 3 4 5 6 7 
1 2 3 4 5 5 5 6 7
)");

	test("set_intersection", set_intersection::run, R"(
5 7 7
)");

	test("set_difference", set_difference::run, R"(
{1, 2, 5, 5, 5, 9}\{2, 5, 7} == {1, 5, 5, 9}
 
old orders: {1, 2, 5, 9}
new orders: {2, 5, 7}
cut orders: {1, 9}
)");

	test("set_symmetric_difference", set_symmetric_difference::run, R"(
1 2 3 4 6 8 9 10
)");

	test("merge", merge::run, R"(
1 2 3 4 5 6 7 8 9 10
)");

	test("inplace_merge", inplace_merge::run, R"(
-2 0 1 2 3 7 8 11 11
)");

	test("push_heap", push_heap::run, R"(
after make_heap: 9 5 4 1 1 3
after push_back: 9 5 4 1 1 3 6
after push_heap: 9 5 6 1 1 3 4
)");

	test("pop_heap", pop_heap::run, R"(
after make_heap: 9 5 4 1 1 3
after pop_heap:  5 3 4 1 1 9
largest element: 9
after pop_back:  5 3 4 1 1
)");

	test("make_heap", make_heap::run, R"(
Max heap:
initially, v: 3 2 4 1 5 9
after make_heap, v: 9 5 4 1 2 3
after pop_heap, v: 5 3 4 1 2 9
former top element: 9
after removing the former top element, v: 5 3 4 1 2
 
Min heap:
initially, v1: 3 2 4 1 5 9
after make_heap, v1: 1 2 4 3 5 9
after pop_heap, v1: 2 3 4 9 5 1
former top element: 1
after removing the former top element, v1: 2 3 4 9 5
)");

	test("sort_heap", sort_heap::run, R"(
after make_heap, v: 9 5 4 1 1 3
after sort_heap, v: 1 1 3 4 5 9
)");

	test("is_heap", is_heap::run, R"(
initially, v:
3 1 4 1 5 9 2 6 5 3 5 8 9 7 9
making heap...
after make_heap, v:
9 | 6 9 | 5 5 9 7 | 1 1 3 5 8 3 4 2 |
)");

	test("is_heap_until", is_heap_until::run, R"(
all of v:  9 5 4 1 1 3 2 6
only heap: 9 5 4 1 1 3 2
)");

	test("sort", sort::run, R"(
0 1 2 3 4 5 6 7 8 9 : sorted with the default operator<
9 8 7 6 5 4 3 2 1 0 : sorted with the standard library compare function object
0 1 2 3 4 5 6 7 8 9 : sorted with a custom function object
9 8 7 6 5 4 3 2 1 0 : sorted with a lambda expression
)");

	test("stable_sort", stable_sort::run, R"(
32, Arthur
108, Zaphod
108, Ford
)");

	test("partial_sort", partial_sort::run, R"(
5 7 4 2 8 6 1 9 0 3
0 1 2 3 4
)");

	test("partial_sort_copy", partial_sort_copy::run, R"(
Writing to the smaller vector in ascending order gives: 1 2 3
The return value is the end iterator
Writing to the larger vector in descending order gives: 5 4 3 2 1 15 16
The return value is the iterator to 15
)");

	test("max_element", max_element::run, R"(
Max element found at index 5 has value 9
Absolute max element found at index 2 has value -14
)");

	test("min_element", min_element::run, R"(
min element has value -4 and index [2]
)");

	test("minmax_element", minmax_element::run, R"(
min = 1, max = 9
)");

	test("max", max::run, R"(
Larger of 69 and 96 is 96
Larger of 'q' and 'p' is 'q'
Largest of 010, 10, 0X10, and 0B10 is 16
Longest of "long", "short", and "int" is "short"
)");

	test("min", min::run, R"(
smaller of 10 and 010 is 8
smaller of 'd' and 'b' is 'b'
shortest of "foo", "bar", and "hello" is "foo"
)");

	test("minmax", minmax::run, R"(
(1,9)
)");

	test("clamp", clamp::run, R"(
[raw] [-128,127] [0,255]
-129       -128       0
-128       -128       0
  -1         -1       0
   0          0       0
  42         42      42
 127        127     127
 128        127     128
 255        127     255
 256        127     255
)");

	test("lexicographical_compare", lexicographical_compare::run, R"(
true
)");

	test("lexicographical_compare_three_way", lexicographical_compare_three_way::run, R"(
"one" is equal to "ONE"
"two" is greater than "four"
"three" is less than "two"
)");

	test("next_permutation", next_permutation::run, R"(
aba
baa
aab
)");

	test("prev_permutation", prev_permutation::run, R"(
cab bca bac acb abc cba
)");

	test("is_permutation", is_permutation::run, R"(
{ 3 5 4 1 2 } is a permutation of { 1 2 3 4 5 }: true
{ 3 5 4 1 1 } is a permutation of { 1 2 3 4 5 }: false
)");

}