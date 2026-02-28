#ifndef SYMGEN_UTILS_HPP
#define SYMGEN_UTILS_HPP

#include <SymGen/Executable.hpp>
#include <SymGen/Pybind11Module.hpp>

#include <concepts>
#include <cstdio>
#include <string_view>
#include <string>
#include <span>

namespace SymGen
{

class Dependency;
class Library;
	
namespace utils
{

namespace detail
{
	void printTargetLinkLibraries(std::FILE* file, const std::string_view name, const std::span<const Dependency> dependencies, const std::span<const std::string> helperLibraries);
} // namespace detail

void printSection(std::FILE* file, const std::string_view secName);

void printTargetSources(std::FILE* file, const std::string_view targetName, const std::span<const std::string> sources);

void printTargetLinkLibraries(std::FILE* file, const Library& library, const std::span<const std::string> helperLibraries);

inline void printTargetLinkLibraries(std::FILE* file, const Executable& executable, const std::span<const std::string> helperLibraries) { detail::printTargetLinkLibraries(file, executable.getName(), executable.getDependencies(), helperLibraries); }

inline void printTargetLinkLibraries(std::FILE* file, const Pybind11Module& pybind11Module, const std::span<const std::string> helperLibraries)  { detail::printTargetLinkLibraries(file, pybind11Module.getName(), pybind11Module.getDependencies(), helperLibraries); }

template<std::integral T, T... CASES, class UnaryFunc>
inline UnaryFunc&& staticSwitch(std::integer_sequence<T, CASES...> /* cases */, const T targetCase, UnaryFunc&& func) { ((targetCase == CASES and (func(std::integral_constant<T,CASES>{}), true)) or ...); return std::forward<UnaryFunc>(func); }

template<std::integral T, T CASES_BOUND, class UnaryFunc>
inline UnaryFunc&& staticSwitch(std::integral_constant<T, CASES_BOUND> /* casesBound */, const T targetCase, UnaryFunc&& func) { return staticSwitch(std::make_integer_sequence<T, CASES_BOUND>{}, targetCase, std::forward<UnaryFunc>(func)); }

std::string toSnakeUpperCase(const std::string_view input); 

// because std::string_view do not overload operator + yet...
std::string concat(const std::span<const std::string_view> values);

inline std::string concat(const std::initializer_list<std::string_view> values) { return concat(std::span(values)); }

inline std::string concat(const std::string_view lhs, const std::string_view rhs) { return concat({lhs, rhs}); }

template<std::integral T, T VALUE>
constexpr inline std::integral_constant<T, VALUE + T(1)> next(const std::integral_constant<T,VALUE> /*value*/) { return {}; }


} // namespace utils
} // namespace SymGen

#endif // SYMGEN_UTILS_HPP
