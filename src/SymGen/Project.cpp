#include <SymGen/Project.hpp>
#include <SymGen/CMakeCompileOption.hpp>
#include <SymGen/MissingPackageExeception.hpp>
#include <SymGen/misc/AddPrefix.hpp>

#include <algorithm>
#include <filesystem>

#include <fmt/format.h>
#include <fmt/ranges.h>

namespace SymGen
{

Project::Project(const std::string_view name)
	: Base(name)
	, m_version("1.0.0")
{ 
	fillHelperLibraries();
}

Project::Project(const std::string_view name, const std::string_view version)
	: Base(name)
	, m_version(version)
{ 
	fillHelperLibraries();
}

Project& Project::addStandard(const std::span<const Language> languages, unsigned int standard)
{
	for (const Language language : languages) { addStandard(language, standard); }
	
	return *this;
}

Project& Project::addCompileOption(const std::span<const Language> languages, const Compiler compiler, const std::string_view option)
{
	for (const Language language : languages) { addCompileOption(language, compiler, option); }
	
	return *this;
}

Project& Project::addCompileOption(const Language language, const std::span<const Compiler> compilers, const std::string_view option)
{
	for (const Compiler compiler : compilers) { addCompileOption(language, compiler, option); }
	
	return *this;
}

Project& Project::addCompileOption(const std::span<const Language> languages, const std::span<const Compiler> compilers, const std::string_view option)
{
	for (const Language language : languages) { addCompileOption(language, compilers, option); }
	
	return *this;
}

Project& Project::addCompileOptions(const Language language, const Compiler compiler, const std::span<const std::string> options)
{	
	std::vector<std::string>& compilerOptions = m_compilerOptions[LanguageCompiler(language, compiler)];
	
	compilerOptions.insert(std::end(compilerOptions), std::cbegin(options), std::cend(options));
	
	return *this;
}

Project& Project::addCompileOptions(const Language language, const std::span<const Compiler> compilers, const std::span<const std::string> options)
{
	for (const Compiler compiler : compilers)
	{
		addCompileOptions(language, compiler, options);
	}
	
	return *this;
}

Project& Project::addCompileOptions(const std::span<const Language> languages, const Compiler compiler, const std::span<const std::string> options)
{
	for (const Language language : languages)
	{
		addCompileOptions(language, compiler, options);
	}
	
	return *this;
}

Project& Project::addCompileOptions(const std::span<const Language> languages, const std::span<const Compiler> compilers, const std::span<const std::string> options)
{
	for (const Compiler compiler : compilers)
	{
		for (const Language language : languages)
		{
			addCompileOptions(language, compiler, options);
		}
	}
	
	return *this;
}

Project& Project::createDirectories(const std::string_view path)
{
	std::filesystem::create_directory(utils::concat({path, "/", getName()}));
	
	for (const SubDirectory& subDirectory : getSubDirectories())
	{
		std::filesystem::create_directory(utils::concat({path, "/", getName(), "/", subDirectory.getName()}));
	}
	
	return *this;
}

void Project::toCMakeListsImpl(std::FILE* file)
{	
	mergeCommonCompileOptions();
	checkIntegrity();
	fillDefaultOptions();
	
	const auto isNotCommonCompileOption = [&commonCompileOptions = m_commonCompileOptions] (const std::string_view option) -> bool
	{		
		return std::ranges::find(commonCompileOptions, option) == std::cend(commonCompileOptions);
	};
	
	// print header
	fmt::println(file, "cmake_minimum_required(VERSION 3.24)");
	fmt::println(file, "");
	fmt::println(file, "cmake_policy(SET CMP0135 NEW)");
	fmt::println(file, "");
	// print project metadata
	utils::printSection(file, "Project metadata");
	fmt::println(file, "");
	fmt::println(file, "project({}", getName());
	fmt::println(file, "\tVERSION {}", m_version);
	if (m_description.size() > 0)
	{
		fmt::println(file, "\tDESCRIPTION \"{}\"", m_description);
	}
	fmt::println(file, "\tLANGUAGES {})", fmt::join(m_langages, " "));
	fmt::println(file, "");
	// print variables
	utils::printSection(file, "Variables");
	fmt::println(file, "");
	fmt::println(file, "if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)");
	fmt::println(file, "\tset(CMAKE_BUILD_TYPE Release CACHE STRING \"Choose the type of build.\")");
	fmt::println(file, "\tset_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS \"Debug\" \"Release\" \"RelWithDebInfo\" \"MinSizeRel\")");
	fmt::println(file, "endif()");
	fmt::println(file, "");
	fmt::println(file, "set({}_SAN_FLAGS -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer)", getCmakePrefix());
	fmt::println(file, "");
	if (not m_commonCompileOptions.empty())
	{
		fmt::println(file, "set({}_COMMON_COMPILE_OPTIONS\n\t{})", getCmakePrefix(), fmt::join(m_commonCompileOptions, "\n\t"));
		fmt::println(file, "");
	}
	if (not m_compilerOptions.empty())
	{
		for (const auto& [langAndCompiler, options] : m_compilerOptions)
		{
			constexpr char prefix[] = "\n\t";
			
			const auto& [language, compiler] = langAndCompiler;
			
			auto nonCommonCompileOptions = options | std::views::filter(isNotCommonCompileOption) | std::views::transform([prefix](const std::string& option) 
			{
				return misc::AddPrefix(prefix, option);
			});
			
			fmt::println(file, "set({}_{}_{}_COMPILE_OPTIONS ${{{}_COMMON_COMPILE_OPTIONS}}{})", getCmakePrefix(), compiler, language, getCmakePrefix(), fmt::join(nonCommonCompileOptions, ""));
			fmt::println(file, "");
		}
	}
	// print options
	utils::printSection(file, "Options");
	fmt::println(file, "");
	
	const std::size_t nameAlignLen = std::ranges::max(getAllOptions() | std::views::transform([](const Option& option) 
	{ 
		return option.getName().size(); 
	}));
	
	const std::size_t descAlignLen = std::ranges::max(getAllOptions() | std::views::transform([](const Option& option) 
	{
		return option.getDescription().size();
	}));
	
	for (const Option& option : getAllOptions())
	{
		//~ fmt::println(file, "option({:<{}} \"{:<{}}\" {})", option.getName(), nameAlignLen, option.getDescription(), descAlignLen, option.getDefaultValue() == SymGen::ON ? "ON" : "OFF");
		fmt::println(file, "option({:<{}} {:<{}} {})", option.getName(), nameAlignLen, fmt::format("\"{}\"", option.getDescription()), descAlignLen + 2, option.getDefaultValue() == SymGen::ON ? "ON" : "OFF");
	}
	fmt::println(file, "");
	// Tests
	printEnableTesting(file);
	// print dependencies
	utils::printSection(file, "Dependencies");
	fmt::println(file, "");
	printDependencies(file);
	// Subfolders
	utils::printSection(file, "SubDirectories");
	fmt::println(file, "");
	for (const SubDirectory& subDirectory : m_subDirectories)
	{
		fmt::println(file, "add_subdirectory({})", subDirectory.getName());
		fmt::println(file, "");
	}
	for (const auto& [subDirectory, option] : m_optionalSubdirectory)
	{
		fmt::println(file, "if(${{{}}})", option.getName());
		fmt::println(file, "\tadd_subdirectory({})", subDirectory.getName());
		fmt::println(file, "endif()");
		fmt::println(file, "");
	}
	// Targets
	utils::printSection(file, "Targets");
	fmt::println(file, "");
	printTargets(file);
	//
	utils::printSection(file, "Sources");
	fmt::println(file, "");
	printSources(file);
	// Link
	utils::printSection(file, "Link");
	fmt::println(file, "");
	printTargetLink(file);
	// Compile options
	utils::printSection(file, "Compile options");
	fmt::println(file, "");
	for (const auto [language, standard] : m_standards)
	{
		std::string str = fmt::format("{}_std_{}", language, standard);
		std::transform(std::cbegin(str), std::cend(str), std::begin(str), [](const char c) -> char
		{
			return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
		});
		
		fmt::println(file, "target_compile_features({} INTERFACE {})", getHelperLibrary(m_optionHelperLibId), str);
	}
	fmt::println(file, "");
	printTargetCompileDefinitions(file);
	if (not m_compilerOptions.empty())
	{
		const auto cmakeCompileOptions = m_compilerOptions | std::views::keys | std::views::transform([cmakePrefix = getCmakePrefix()](const LanguageCompiler& languageCompiler) -> CMakeCompileOption
		{
			return CMakeCompileOption{.compiler = languageCompiler.second, .language = languageCompiler.first, .cmakePrefix = cmakePrefix};
		}); 
		fmt::println(file, "target_compile_options({} INTERFACE\n{})", getHelperLibrary(m_optionHelperLibId), fmt::join(cmakeCompileOptions, "\n"));
	}
	fmt::println(file, "");
	fmt::println(file, "if({}_ENABLE_SANITIZERS)", getCmakePrefix());
	fmt::println(file, "\ttarget_compile_options({} INTERFACE $<$<CONFIG:Debug>:${{SAN_FLAGS}}>)", getHelperLibrary(m_optionHelperLibId));
	fmt::println(file, "endif()");
	fmt::println(file, "");
	fmt::println(file, "if({}_USE_NATIVE_OPT)", getCmakePrefix());
	fmt::println(file, "\ttarget_compile_options({} INTERFACE -march=native -mtune=native)", getHelperLibrary(m_optionHelperLibId));
	fmt::println(file, "endif()");
}

void Project::printEnableTesting(std::FILE* file)
{
	if (hasTests())
	{
		utils::printSection(file, "Tests");
		fmt::println(file, "");
		printTests(file);
		fmt::println(file, "");
		fmt::println(file, "enable_testing()");
		fmt::println(file, "");
	}
	else
	{		
		const std::input_iterator auto itDir = std::ranges::find_if(m_subDirectories, [](const SubDirectory subDir) -> bool
		{
			return subDir.hasTests();
		});
		
		const std::input_iterator auto itDirOpt = std::ranges::find_if(m_optionalSubdirectory, [](const std::pair<SubDirectory,Option> subDirAndOption) -> bool
		{
			return subDirAndOption.first.hasTests();
		});
		
		if (itDir != std::ranges::cend(m_subDirectories))
		{
			utils::printSection(file, "Tests");
			fmt::println(file, "");
			fmt::println(file, "enable_testing()");
			fmt::println(file, "");
		}
		else if (itDirOpt != std::ranges::cend(m_optionalSubdirectory))
		{
			utils::printSection(file, "Tests");
			fmt::println(file, "");
			fmt::println(file, "if(${{{}}})", itDirOpt->second.getName());
			fmt::println(file, "\tenable_testing()");
			fmt::println(file, "endif()");
			fmt::println(file, "");
		}
		
	}
}

void Project::mergeCommonCompileOptions()
{		
	m_commonCompileOptions.clear();
		
	for (const auto& [langAndCompiler, options] : m_compilerOptions)
	{
		for (const std::string& option : options)
		{
			const bool isCommon = std::ranges::all_of(std::views::values(m_compilerOptions), [option](const std::vector<std::string>& otherOptions) -> bool
			{
				return std::ranges::find(otherOptions, option) != std::cend(otherOptions);
			});
			
			if (isCommon) { m_commonCompileOptions.insert(option); }
		}
	}
}

void Project::fillHelperLibraries()
{
	m_optionHelperLibId = getHelperLibraries().size();
	addHelperLibrary(utils::concat(getName(), "_options"));
}

void Project::fillDefaultOptions()
{
	const auto allOptionsView = misc::concatenate(m_options, std::views::elements<1>(m_optionalSubdirectory));
	
	const std::string enableSanOption      = utils::concat(getCmakePrefix(), "_ENABLE_SANITIZERS");
	const std::string buildDocOption       = utils::concat(getCmakePrefix(), "_BUILD_DOC");
	const std::string useNativeOptsOption  = utils::concat(getCmakePrefix(), "_USE_NATIVE_OPT");
	
	const auto it_san = std::ranges::find_if(allOptionsView, [&enableSanOption](const Option& option) -> bool
	{
		return option.getName() == enableSanOption;
	});
	
	const auto it_doc = std::ranges::find_if(allOptionsView, [&buildDocOption](const Option& option) -> bool
	{
		return option.getName() == buildDocOption;
	});
	
	const auto it_nativeOpts = std::ranges::find_if(allOptionsView, [&useNativeOptsOption](const Option& option) -> bool
	{
		return option.getName() == useNativeOptsOption;
	});
	
	if (it_san == std::cend(allOptionsView))
	{
		m_options.emplace_back(enableSanOption, "Enable sanitizers in Debug builds", OFF);
	}
	if (it_doc == std::cend(allOptionsView))
	{
		m_options.emplace_back(buildDocOption, "Build Doxygen documentation", OFF);
	}
	if (it_nativeOpts == std::cend(allOptionsView))
	{
		m_options.emplace_back(useNativeOptsOption, "Enable -march=native -mtune=native", OFF);
	}
}

void Project::checkIntegrity() const
{
	// check that all dependancies have been included.
	
	for (const Executable& executable : getExecutables())
	{
		for (const Dependency& dependency : executable.getDependencies())
		{			
			if (not satisfyDependency(dependency)) 
			{ 
				throw MissingPackageExeception(fmt::format("Executable {} requires {} which is not included.", executable.getName(), dependency)); 
			}
		}
	}
	for (const Pybind11Module& pybind11Module : getPybind11Modules())
	{
		for (const Dependency& dependency : pybind11Module.getDependencies())
		{			
			if (not satisfyDependency(dependency)) 
			{ 
				throw MissingPackageExeception(fmt::format("Executable {} requires {} which is not included.", pybind11Module.getName(), dependency)); 
			}
		}
	}
	for (const Library& library : getLibraries())
	{
		for (const Dependency& dependency : library.getPublicDependencies())
		{			
			if (not satisfyDependency(dependency)) 
			{ 
				throw MissingPackageExeception(fmt::format("Library {} requires {} which is not included.", library.getName(), dependency)); 
			}
		}
		for (const Dependency& dependency : library.getPrivateDependencies())
		{			
			if (not satisfyDependency(dependency)) 
			{ 
				throw MissingPackageExeception(fmt::format("Library {} requires {} which is not included.", library.getName(), dependency)); 
			}
		}
	}
	
	// Check that all executables have sources
	
	for (const Executable& executable : getExecutables())
	{
		if (executable.isInterface())
		{
			throw ExecutableWithouSourcesException(fmt::format("Executable {} has no sources", executable.getName()));
		}
	}
}

bool Project::satisfyDependency(const Dependency& dependency) const
{	
	if (not dependency.hasComponent())
	{
		const bool isInLibraries = std::ranges::find_if(getLibraries(), [dependency](const Library& library) -> bool
		{
			return library.getName() == dependency.getPackage();
		}) != std::cend(getLibraries());
		
		if (isInLibraries) { return true; }
	}
	
	const bool isInDeps = std::ranges::find_if(getDependencies(), [dependency](const Package& package) -> bool
	{
		
		return package.satisfyDependency(dependency);
	}) != std::cend(getDependencies());
	
	return isInDeps;
}

} // namespace SymGen
