#ifndef SYMGEN_TEST_HPP
#define SYMGEN_TEST_HPP

#include <string>
#include <string_view>
#include <vector>
#include <span>

namespace SymGen
{

class Test
{
public:
	Test(const std::string_view name, const std::string_view command) : m_name(name), m_command(command) {}

	inline std::string_view getName() const { return m_name; }
	
	inline std::string_view getCommand() const { return m_command; }

	inline std::span<const std::string> getArgs() const { return m_args; }
	
	inline Test& addArg(const std::string_view args) { m_args.emplace_back(args); return *this; }
	
	inline Test& addArgs(const std::initializer_list<std::string> args) { return addArgs(std::span(args)); }
	
	inline Test& addArgs(const std::span<const std::string> args) { m_args.insert(std::ranges::end(m_args), std::ranges::cbegin(args), std::ranges::cend(args)); return *this; }
private:
	std::string              m_name;
	std::string              m_command;
	std::vector<std::string> m_args;
};

} // namespace SymGen

#endif // SYMGEN_TEST_HPP
