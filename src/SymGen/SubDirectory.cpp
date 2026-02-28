#include <SymGen/SubDirectory.hpp>
#include <SymGen/utils.hpp>
#include <SymGen/ModuleWithouSourcesException.hpp>
#include <SymGen/ExecutableWithouSourcesException.hpp>

#include <ranges>

#include <fmt/format.h>

namespace SymGen
{
	
void SubDirectory::toCMakeListsImpl(std::FILE* file)
{
	utils::printSection(file, utils::concat(getName(), " subdirectory"));
	fmt::println(file, "");
	// print Dependencies
	utils::printSection(file, "Dependencies");
	fmt::println(file, "");
	printDependencies(file);
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
	// Tests
	
	if (hasTests())
	{
		utils::printSection(file, "Tests");
		fmt::println(file, "");
		printTests(file);
	}
}

} // namespace SymGen
