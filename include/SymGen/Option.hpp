#ifndef SYMGEN_OPTION_HPP
#define SYMGEN_OPTION_HPP

#include <SymGen/OnOff.hpp>

#include <string>
#include <string_view>

#include <fmt/format.h>

namespace SymGen
{

class Option
{
public:
	Option(const std::string_view name) : m_name(name), m_defaultValue(OFF)   {}
	
	Option(const std::string_view name, const std::string_view description) : m_name(name), m_description(description), m_defaultValue(OFF)   {}
	
	Option(const std::string_view name, const std::string_view description, const int value) : m_name(name), m_description(description), m_defaultValue(value) {}
	
	const std::string_view getName() const { return m_name; }
	
	std::string_view getDescription() const { return m_description; }
	
	int getDefaultValue() const { return m_defaultValue; }
	
private:
	std::string m_name;
	std::string m_description;
	int         m_defaultValue;
};

} // namespace SymGen

template<>
struct fmt::formatter<SymGen::Option>
{
	template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }
	
	template <typename FormatContext>
    auto format(const SymGen::Option& option, FormatContext& ctx) const
    {		
        return fmt::format_to(ctx.out(), "option({} \"{}\" {})", option.getName(), option.getDescription(), option.getDefaultValue() == SymGen::ON 
			? "ON" 
			: "OFF");
    }
};

#endif // SYMGEN_OPTION_HPP
