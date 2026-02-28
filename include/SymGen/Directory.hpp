#ifndef SYMGEN_DIRECTORY_HPP
#define SYMGEN_DIRECTORY_HPP

#include <SymGen/Package.hpp>
#include <SymGen/Library.hpp>
#include <SymGen/Executable.hpp>
#include <SymGen/Pybind11Module.hpp>
#include <SymGen/Test.hpp>

#include <string>
#include <string_view>
#include <vector>

namespace SymGen
{

template<class Derived>
class Directory : public CRTPBase<Derived>
{
public:
	using Base = CRTPBase<Derived>;
	
	Directory(const std::string_view name);
	
	inline std::string_view getName() const { return m_name; }
	
	inline std::string_view getCmakePrefix() const { return m_cmakePrefix; }
	
	inline bool hasTests() const { return not m_tests.empty(); }
	
	inline Derived& setCmakePrefix(const std::string_view cmakePrefix) { m_cmakePrefix = cmakePrefix; return Base::derived(); } 
	
	inline Derived& setIncludeDir(const std::string_view includeDir) { m_includeDir = includeDir; return Base::derived(); }
	
	inline Derived& addDependency(const Package& dependency) { m_dependencies.push_back(dependency); return Base::derived(); }
	
	inline Derived& addDependency(Package&& dependency) { m_dependencies.emplace_back(std::forward<Package>(dependency)); return Base::derived(); }
	
	inline Derived& addDependency(const std::string_view name) { m_dependencies.emplace_back(name); return Base::derived(); }
	
	inline Derived& addDependency(const std::string_view name, const std::string_view targetName) { m_dependencies.emplace_back(name, targetName); return Base::derived(); }
	
	inline Derived& addLibrary(const Library& library) { m_libraries.push_back(library); return Base::derived(); }
	
	inline Derived& addLibrary(Library&& library) { m_libraries.emplace_back(std::forward<Library>(library)); return Base::derived(); }
	
	inline Derived& addExecutable(const Executable& executable) { m_executables.push_back(executable); return Base::derived(); }
	
	inline Derived& addExecutable(Executable&& executable) { m_executables.emplace_back(std::forward<Executable>(executable)); return Base::derived(); }
		
	inline Derived& addPybind11Module(const Pybind11Module& pybind11Module) { m_pybind11Modules.push_back(pybind11Module); return Base::derived(); }
	
	inline Derived& addPybind11Module(Pybind11Module&& pybind11Module) { m_pybind11Modules.emplace_back(std::forward<Pybind11Module>(pybind11Module)); return Base::derived(); }
	
	inline Derived& addTest(const Test& test) { m_tests.push_back(test); return Base::derived(); }
	
	inline Derived& addTest(Test&& test) { m_tests.emplace_back(std::forward<Test>(test)); return Base::derived(); }
	
	void toCMakeLists(std::FILE* file) { Base::derived().toCMakeListsImpl(file); }
	
	void toCMakeLists(std::string_view fileName);
protected:
	Directory(const std::string_view name, const std::string_view parentCmakePrefix);

	void printDependencies(std::FILE* file);
	
	void printTargets(std::FILE* file);
	
	void printSources(std::FILE* file);
	
	void printTargetLink(std::FILE* file);
	
	void printTests(std::FILE* file);
	
	inline std::string_view getHelperLibrary(const std::size_t id) const { return m_helperLibraries[id]; }
	
	inline std::span<const std::string> getHelperLibraries() const { return m_helperLibraries; }
	
	inline std::span<const Package> getDependencies() const { return m_dependencies; }
	
	inline std::span<const Library> getLibraries() const { return m_libraries; }
	
	inline std::span<const Executable> getExecutables() const { return m_executables; }
	
	inline std::span<const Pybind11Module> getPybind11Modules() const { return m_pybind11Modules; }
	
	inline std::span<const Test> getTests() const { return m_tests; }
	
	inline void addHelperLibrary(const std::string_view name) { m_helperLibraries.emplace_back(name); }
private:
	void fillHelperLibraries();

	std::string                 m_name;
	std::string                 m_cmakePrefix;
	std::string                 m_includeDir = "include";
	std::string                 m_srcDir = "src";
	std::vector<Package>        m_dependencies;
	std::vector<std::string>    m_helperLibraries; // Helper libraries do not have deps so a std::string is sufficient
	std::vector<Library>        m_libraries;
	std::vector<Executable>     m_executables;
	std::vector<Pybind11Module> m_pybind11Modules;
	std::vector<Test>           m_tests;
	
	static constexpr size_t s_includeLibrary = 0;
};

} // namespace SymGen

#include <SymGen/Directory_impl.hpp>

#endif // SYMGEN_DIRECTORY_HPP
