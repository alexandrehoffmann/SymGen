#ifndef SYMGEN_DIRECTORY_IMPL_HPP
#define SYMGEN_DIRECTORY_IMPL_HPP

#include <SymGen/utils.hpp>
#include <SymGen/ModuleWithouSourcesException.hpp>
#include <SymGen/ExecutableWithouSourcesException.hpp>
#include <SymGen/MissingExecutableException.hpp>

#include <ranges>

#include <fmt/ranges.h>

namespace SymGen
{

template<class Derived>
Directory<Derived>::Directory(const std::string_view name)
	: m_name(name)
	, m_cmakePrefix(utils::toSnakeUpperCase(name)) 
{
	fillHelperLibraries();
}

template<class Derived>
Directory<Derived>::Directory(const std::string_view name, const std::string_view parentCmakePrefix)
	: m_name(name)
	, m_cmakePrefix(utils::concat({parentCmakePrefix, "_", utils::toSnakeUpperCase(name)}))
{
	fillHelperLibraries();
}

template<class Derived>
void Directory<Derived>::toCMakeLists(std::string_view fileName)
{
	std::FILE* fout = std::fopen(fileName.data(), "w");
	toCMakeLists(fout);
	std::fclose(fout);
}

template<class Derived>
void Directory<Derived>::fillHelperLibraries()
{
	m_helperLibraries.emplace_back(m_name + "_include");
}

template<class Derived>
void Directory<Derived>::printDependencies(std::FILE* file)
{
	const auto hasGit      = [](const Package& package) -> bool { return     package.hasGit(); };
	const auto doNotHasGit = [](const Package& package) -> bool { return not package.hasGit(); };
	
	if (not m_dependencies.empty())
	{
		// print dependencies
		for (const Package& package : m_dependencies | std::views::filter(doNotHasGit))
		{
			package.writeFindPackage(file);
		}
		fmt::println(file, "");
		fmt::println(file, "include(FetchContent)");
		for (const Package& package : m_dependencies | std::views::filter(hasGit))
		{
			package.writeFetchContent(file);
		}
		fmt::println(file, "");
		for (const Package& package : m_dependencies | std::views::filter(hasGit))
		{
			fmt::println(file, "FetchContent_MakeAvailable({})", package.getName());
		}
		fmt::println(file, "");
	}
}

template<class Derived>
void Directory<Derived>::printTargets(std::FILE* file)
{
	const auto isInterface    = [](const Library& library) -> bool { return     library.isInterface(); };
	const auto isNotInterface = [](const Library& library) -> bool { return not library.isInterface(); };
	
	for (const std::string_view helperLibrary : m_helperLibraries)
	{
		fmt::println(file, "add_library({} INTERFACE)", helperLibrary);
		fmt::println(file, "");
	}
	for (const Library& library : m_libraries | std::views::filter(isInterface))
	{
		fmt::println(file, "add_library({} INTERFACE)", library.getName());
		fmt::println(file, "");
	}
	for (const Library& library : m_libraries | std::views::filter(isNotInterface))
	{
		fmt::println(file, "add_library({})", library.getName());
		fmt::println(file, "");
	}
	for (const Pybind11Module& pyBind11Module : m_pybind11Modules)
	{
		fmt::println(file, "pybind11_add_module({} MODULE)", pyBind11Module.getName());
		fmt::println(file, "");
	}
	for (const Executable& executable : m_executables)
	{
		fmt::println(file, "add_executable({})", executable.getName());
		fmt::println(file, "");
	}
}

template<class Derived>
void Directory<Derived>::printSources(std::FILE* file)
{
	const auto isNotInterface = [](const Library& library) -> bool { return not library.isInterface(); };
	
	for (const Library& library : m_libraries | std::views::filter(isNotInterface))
	{
		utils::printTargetSources(file, library.getName(), library.getSources());
	}
	for (const Pybind11Module& pyBind11Module : m_pybind11Modules)
	{
		if (pyBind11Module.isInterface()) { throw ModuleWithouSourcesException(fmt::format("Module {} has no sources", pyBind11Module.getName())); }
		utils::printTargetSources(file, pyBind11Module.getName(), pyBind11Module.getSources());
	}
	for (const Executable& executable : m_executables)
	{
		if (executable.isInterface()) { throw ExecutableWithouSourcesException(fmt::format("Executable {} has no sources", executable.getName())); }
		utils::printTargetSources(file, executable.getName(), executable.getSources());
	}
	fmt::println(file, "target_include_directories({} INTERFACE $<BUILD_INTERFACE:${{CMAKE_CURRENT_SOURCE_DIR}}/{}>)", m_helperLibraries[s_includeLibrary], m_includeDir);
	fmt::println(file, "");
}

template<class Derived>
void Directory<Derived>::printTargetLink(std::FILE* file)
{
	for (const Library& library : m_libraries)
	{
		utils::printTargetLinkLibraries(file, library, m_helperLibraries);
	}
	for (const Pybind11Module& pyBind11Module : m_pybind11Modules)
	{
		utils::printTargetLinkLibraries(file, pyBind11Module, m_helperLibraries);
	}
	for (const Executable& executable : m_executables)
	{
		utils::printTargetLinkLibraries(file, executable, m_helperLibraries);
	}
}

template<class Derived>
void Directory<Derived>::printTargetCompileDefinitions(std::FILE* file)
{

	for (const Library& library : m_libraries)
	{
		utils::printTargetCompileDefinitions(file, library.getName(), library.getPublicCompileDefinitions(), library.getPrivateCompileDefinitions());
	}
	for (const Pybind11Module& pyBind11Module : m_pybind11Modules )
	{
		utils::printTargetCompileDefinitions(file, pyBind11Module.getName(), pyBind11Module.getPublicCompileDefinitions(), pyBind11Module.getPrivateCompileDefinitions());
	}
	for (const Executable& executable : m_executables)
	{
		utils::printTargetCompileDefinitions(file, executable.getName(), executable.getPublicCompileDefinitions(), executable.getPrivateCompileDefinitions());
	}
}

template<class Derived>
void Directory<Derived>::printTests(std::FILE* file)
{	
	using Iterator = typename std::vector<Executable>::const_iterator;
	
	for (const Test& test : m_tests)
	{
		const Iterator it = std::ranges::find_if(m_executables, [&test](const Executable& executable) -> bool
		{
			return test.getCommand() == executable.getName();
		});
		
		if (it == std::ranges::cend(m_executables)) { throw MissingExecutableException(fmt::format("Missing executable for test {}", test.getName())); }
		
		fmt::println(file, "add_test(NAME {} COMMAND {}{})", test.getName(), test.getCommand(), fmt::join(test.getArgs(), " "));
		fmt::println(file, "");
	}
}

} // namespace SymGen

#endif // SYMGEN_DIRECTORY_IMPL_HPP
