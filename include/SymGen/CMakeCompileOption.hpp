#ifndef SYMGEN_CMAKE_COMPILE_OPTION_HPP
#define SYMGEN_CMAKE_COMPILE_OPTION_HPP

#include <SymGen/Compiler.hpp>
#include <SymGen/Language.hpp>

#include <string_view>

#include <fmt/format.h>

namespace SymGen
{

struct CMakeCompileOption
{
	Compiler         compiler;
	Language         language; 
	std::string_view cmakePrefix;
};

} // namespace SymGen

template<>
struct fmt::formatter<SymGen::CMakeCompileOption>
{
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const SymGen::CMakeCompileOption& compileOption, FormatContext& ctx) const
	{
		return fmt::format_to(ctx.out(), "\t$<$<{}_COMPILER_ID:{}>:${{{}_{}_{}_COMPILE_OPTIONS}}>", compileOption.language, compileOption.compiler, 
			compileOption.cmakePrefix, compileOption.compiler, compileOption.language);
	}
};

#endif // SYMGEN_CMAKE_COMPILE_OPTION_HPP
