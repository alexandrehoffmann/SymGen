#include <SymGen/misc/ConcatenateViews.hpp>

#include <gtest/gtest.h>

#include <ranges>
#include <numeric>

TEST(ConcatenateViews, self)
{
	std::vector<double> a(10);
	std::iota(std::ranges::begin(a), std::ranges::end(a), double{});
	
	const std::ranges::view auto res = SymGen::misc::concatenate(a, a);
	
	std::vector<double> expected(2*a.size());
	std::iota(std::ranges::begin(expected), std::ranges::begin(expected) + std::ssize(a), double{});
	std::iota(std::ranges::begin(expected) + std::ssize(a), std::ranges::end(expected), double{});
	
	EXPECT_TRUE(std::ranges::equal(res, expected));
}

TEST(ConcatenateViews, basic)
{
	std::vector<double> a(10);
	std::vector<double> b(10);
	std::iota(std::ranges::begin(a), std::ranges::end(a), double{});
	std::iota(std::ranges::begin(b), std::ranges::end(b), double(a.size()));
	
	const std::ranges::view auto res = SymGen::misc::concatenate(a, b);
	
	std::vector<double> expected(2*a.size());
	std::iota(std::ranges::begin(expected), std::ranges::end(expected), double{});
	
	EXPECT_TRUE(std::ranges::equal(res, expected));
}

TEST(ConcatenateViews, not_basic)
{
	const auto isEven = [](const double x) -> double
	{
		return int(x) % 2 == 0;
	};

	std::vector<double> a(10);
	std::iota(std::begin(a), std::end(a), double{});

	std::ranges::view auto res = SymGen::misc::concatenate(a, a | std::views::filter(isEven), a);
	
	std::vector<double> expected;
	std::ranges::copy(a, std::back_inserter(expected));
	std::ranges::copy(a | std::views::filter(isEven), std::back_inserter(expected));
	std::ranges::copy(a, std::back_inserter(expected));
	
	EXPECT_TRUE(std::ranges::equal(res, expected));
}
