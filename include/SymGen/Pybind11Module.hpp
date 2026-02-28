#ifndef SYMGEN_PYBIND11_MODULE_HPP
#define SYMGEN_PYBIND11_MODULE_HPP

#include <SymGen/Target.hpp>
#include <SymGen/Dependency.hpp>

#include <string>
#include <string_view>
#include <vector>

namespace SymGen
{

class Pybind11Module : public Target<Pybind11Module>
{
public:
	using Base = Target<Pybind11Module>;

	Pybind11Module(const std::string_view name) : Base(name) {}
	
	inline std::span<const Dependency> getDependencies() const { return m_dependencies; }
	
	inline Pybind11Module& addDependency(const std::string_view dependency) { m_dependencies.emplace_back(dependency); return *this; }
	
	inline Pybind11Module& addDependency(const std::string_view package, const std::string_view component) { m_dependencies.emplace_back(package, component); return *this; }
private:
	std::vector<Dependency> m_dependencies;
};

} // namespace SymGen

#endif // SYMGEN_PYBIND11_MODULE_HPP
