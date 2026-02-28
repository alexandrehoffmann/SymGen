#include <SymGen/utils.hpp>
#include <SymGen/Dependency.hpp>
#include <SymGen/Executable.hpp>
#include <SymGen/Library.hpp>
#include <SymGen/Pybind11Module.hpp>
#include <SymGen/misc/AddPrefix.hpp>

#include <ranges>
#include <algorithm>

#include <fmt/format.h>
#include <fmt/ranges.h>

namespace SymGen
{
namespace utils
{

namespace detail
{

void printTargetLinkLibraries(std::FILE* file, const std::string_view name, const std::span<const Dependency> dependencies, const std::span<const std::string> helperLibraries)
{
	if (not dependencies.empty())
	{
		constexpr char prefix[] = "\n\tPRIVATE ";
			
		const auto dependenciesView = dependencies | std::views::transform([prefix](const Dependency& dependency) 
		{
			return misc::AddPrefix(prefix, dependency);
		});
		
		const auto helperLibrariesView = helperLibraries | std::views::transform([prefix](const std::string& dependency) 
		{
			return misc::AddPrefix(prefix, dependency);
		});
		
		fmt::println(file, "target_link_libraries({}{}{})", name, fmt::join(helperLibrariesView, ""), fmt::join(dependenciesView, ""));
		fmt::println(file, "");	
	}
}

} // namespace detail

void printSection(std::FILE* file, const std::string_view secName)
{
	constexpr char header[] = "########################################################################"; 
	constexpr char left[]   = "####"; 
	constexpr char right[]  = "####"; 
	
	constexpr size_t width = size_t(std::strlen(header) - std::strlen(left) - std::strlen(right));

	fmt::println(file, "{}", header);
	fmt::println(file, "{}{:^{}}{}", left, secName, width, right);
	fmt::println(file, "{}", header);
}

void printTargetSources(std::FILE* file, const std::string_view targetName, const std::span<const std::string> sources)
{
	fmt::println(file, "target_sources({} PRIVATE\n\t{})", targetName, fmt::join(sources, "\n\t"));
	fmt::println(file, "");
}

void printTargetLinkLibraries(std::FILE* file, const Library& library, const std::span<const std::string> helperLibraries)
{
	if (not (library.getPublicDependencies().empty() and library.getPrivateDependencies().empty()))
	{
		constexpr char publicPrefix[]  = "\n\tPUBLIC ";
		constexpr char privatePrefix[] = "\n\tPRIVATE ";
		
		const auto publicDependenciesView = library.getPublicDependencies() | std::views::transform([publicPrefix](const Dependency& dependency) 
		{
			return misc::AddPrefix(publicPrefix, dependency);
		});
		
		const auto privateDependenciesView = library.getPrivateDependencies() | std::views::transform([privatePrefix](const Dependency& dependency) 
		{
			return misc::AddPrefix(privatePrefix, dependency);
		});
		
		const auto helperLibrariesView = helperLibraries | std::views::transform([publicPrefix](const std::string& dependency) 
		{
			return misc::AddPrefix(publicPrefix, dependency);
		});
		
		fmt::println(file, "target_link_libraries({}{}{}{})", library.getName(), fmt::join(helperLibrariesView, ""), fmt::join(publicDependenciesView, ""), fmt::join(privateDependenciesView, ""));
		fmt::println(file, "");
	}
}

std::string concat(const std::span<const std::string_view> values)
{
	std::string ret;

	for (const std::string_view value : values)
	{
		ret += value;
	}

	return ret;
}

std::string toSnakeUpperCase(const std::string_view input) 
{
	const auto toUpper = [](const char c) -> char
	{
		return static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
	};

	const std::size_t nUpper = size_t(std::ranges::count_if(input, [](const char c) -> bool
	{
		return std::isupper(static_cast<unsigned char>(c));
	}));

	std::string ret;
	ret.reserve(input.size() + nUpper);
	ret += toUpper(input.front());

	char lastC = input.front();

	for (const char c : input | std::views::drop(1))
	{
		if (std::isspace(c) or c == '-')
		{
			if (ret.back() != '_') { ret += '_'; }
		}
		else if (std::isupper(static_cast<unsigned char>(c)) and std::islower(static_cast<unsigned char>(lastC)))
		{
			if (ret.back() != '_') { ret += '_'; }
			lastC = c;
			ret += toUpper(c);
		}
		else if (std::isalnum(c))
		{
			lastC = c;
			ret += toUpper(c);
		}
	}

	return ret;
}

} // namespace utils
} // namespace SymGen
