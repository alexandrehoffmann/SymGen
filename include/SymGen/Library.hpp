#ifndef SYMGEN_LIBRARY_HPP
#define SYMGEN_LIBRARY_HPP

#include <SymGen/Target.hpp>
#include <SymGen/Dependency.hpp>

namespace SymGen
{
	
class Library : public Target<Library>
{
public:
	using Base = Target;

	Library(const std::string_view name) : Base(name) {}
	
	inline std::span<const Dependency> getPublicDependencies() const { return m_publicDependencies; }
	
	inline std::span<const Dependency> getPrivateDependencies() const { return m_privateDependencies; }

	inline Library& addPublicDependency(const std::string_view dependency) { m_publicDependencies.emplace_back(dependency); return *this; }
	
	inline Library& addPublicDependency(const std::string_view package, const std::string_view component) { m_publicDependencies.emplace_back(package, component); return *this; }

	inline Library& addPrivateDependency(const std::string_view dependency) { m_privateDependencies.emplace_back(dependency); return *this; }
	
	inline Library& addPrivateDependency(const std::string_view package, const std::string_view component) { m_privateDependencies.emplace_back(package, component); return *this; }
private:
	std::vector<Dependency>  m_publicDependencies;
	std::vector<Dependency>  m_privateDependencies;
};

} // namespace SymGen

#endif // SYMGEN_LIBRARY_HPP
