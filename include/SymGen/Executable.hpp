#ifndef SYMGEN_EXECUTABLE_HPP
#define SYMGEN_EXECUTABLE_HPP

#include <SymGen/Target.hpp>
#include <SymGen/Dependency.hpp>

namespace SymGen
{
	
class Executable : public Target<Executable>
{
public:
	using Base = Target<Executable>;

	Executable(const std::string_view name) : Base(name) {}
	
	inline std::span<const Dependency> getDependencies() const { return m_dependencies; }

	inline Executable& addDependency(const std::string_view dependency) { m_dependencies.emplace_back(dependency); return *this; }
	
	inline Executable& addDependency(const std::string_view package, const std::string_view component) { m_dependencies.emplace_back(package, component); return *this; }
private:
	std::vector<Dependency>  m_dependencies;
};

} // namespace SymGen

#endif // SYMGEN_EXECUTABLE_HPP
