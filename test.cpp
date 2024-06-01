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

}