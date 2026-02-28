#ifndef SYMGEN_DEPENDENCY_HPP
#define SYMGEN_DEPENDENCY_HPP

#include <string>
#include <string_view>

#include <fmt/format.h>

namespace SymGen
{
	
class Dependency
{
public:
	Dependency(const std::string_view name) : m_package(name) {}
	
	Dependency(const std::string_view package, const std::string_view component) : m_package(package), m_component(component) {}
	
	std::string_view getPackage() const { return m_package; }
	
	std::string_view getComponent() const { return m_component; }
	
	bool hasComponent() const { return m_component.size() != 0; }
private:
	std::string m_package;
	std::string m_component;
};

} // namespace SymGen

template<>
struct fmt::formatter<SymGen::Dependency>
{
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const SymGen::Dependency& dependency, FormatContext& ctx) const
	{
		auto out = fmt::format_to(ctx.out(), "{}", dependency.getPackage());
	
		if (dependency.hasComponent()) { out = fmt::format_to(ctx.out(), "::{}", dependency.getComponent()); }

		return out;
	}
};

#endif // SYMGEN_DEPENDENCY_HPP
