#ifndef SYMGEN_MISC_ADD_PREFIX_HPP
#define SYMGEN_MISC_ADD_PREFIX_HPP

#include <string_view>

#include <fmt/core.h>

namespace SymGen
{
namespace misc 
{

template <typename T>
struct AddPrefix 
{	
	AddPrefix(const std::string_view a_prefix, const T& a_value) : prefix(a_prefix), value(a_value) {} 
	
	std::string_view prefix;
	const T& value;
};

} // namespace misc
} // namespace SymGen

template <typename T>
struct fmt::formatter<SymGen::misc::AddPrefix<T>>
{
	constexpr auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const SymGen::misc::AddPrefix<T>& v, FormatContext& ctx) const
	{
		auto out = fmt::format_to(ctx.out(), "{}", v.prefix);
		return fmt::format_to(out, "{}", v.value);
	}
};

#endif // SYMGEN_MISC_ADD_PREFIX_HPP
