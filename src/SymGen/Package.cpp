#include <SymGen/Package.hpp>
#include <SymGen/Dependency.hpp>

#include <fmt/ranges.h>

namespace SymGen
{

bool Package::satisfyDependency(const Dependency& dependency) const
{
	return m_name == dependency.getPackage() 
		and (not dependency.hasComponent() 
			or std::ranges::find(m_components, dependency.getComponent()) != std::end(m_components));
}

void Package::writeFindPackage(std::FILE* fout) const
{
	if (hasComponents())
	{
		if (hasMinMaxVersion())
		{
			fmt::println(fout, "find_package({} {}...{} REQUIRED COMPONENTS {})", m_name, m_minVersion, m_maxVersion, fmt::join(m_components, " "));
		}
		else if (hasVersion())
		{
			fmt::println(fout, "find_package({} {} REQUIRED COMPONENTS {})", m_name, m_minVersion, fmt::join(m_components, " "));
		}
		else
		{
			fmt::println(fout, "find_package({} REQUIRED COMPONENTS {})", m_name, fmt::join(m_components, " "));
		}
	}
	else
	{
		if (hasMinMaxVersion())
		{
			fmt::println(fout, "find_package({} {}...{} REQUIRED)", m_name, m_minVersion, m_maxVersion);
		}
		else if (hasVersion())
		{
			fmt::println(fout, "find_package({} {} REQUIRED)", m_name, m_minVersion);
		}
		else
		{
			fmt::println(fout, "find_package({} REQUIRED)", m_name);
		}
	}
}

void Package::writeFetchContent(std::FILE* fout) const
{
	if (hasComponents())
	{
		if (hasMinMaxVersion())
		{
			fmt::println(fout, "FetchContent_Declare({} GIT_REPOSITORY {} GIT_TAG {} FIND_PACKAGE_ARGS NAMES {} VERSION {}...{} COMPONENTS {})", m_name, m_gitDepot, m_gitTag, 
				m_name, m_minVersion, m_maxVersion, fmt::join(m_components, " "));
		}
		else if (hasVersion())
		{
			fmt::println(fout, "FetchContent_Declare({} GIT_REPOSITORY {} GIT_TAG {} FIND_PACKAGE_ARGS NAMES {} VERSION {} COMPONENTS {})", m_name, m_gitDepot, m_gitTag, m_name, 
				m_minVersion, fmt::join(m_components, " "));
		}
		else
		{
			fmt::println(fout, "FetchContent_Declare({} GIT_REPOSITORY {} GIT_TAG {} FIND_PACKAGE_ARGS NAMES {} COMPONENTS {})", m_name, m_gitDepot, m_gitTag, m_name, fmt::join(m_components, " "));
		}
	}
	else
	{
		if (hasMinMaxVersion())
		{
			fmt::println(fout, "FetchContent_Declare({} GIT_REPOSITORY {} GIT_TAG {} FIND_PACKAGE_ARGS NAMES {} VERSION {}...{})", m_name, m_gitDepot, m_gitTag, m_name, m_minVersion, m_maxVersion);
		}
		else if (hasVersion())
		{
			fmt::println(fout, "FetchContent_Declare({} GIT_REPOSITORY {} GIT_TAG {} FIND_PACKAGE_ARGS NAMES {} VERSION {})", m_name, m_gitDepot, m_gitTag, m_name, m_minVersion);
		}
		else
		{
			fmt::println(fout, "FetchContent_Declare({} GIT_REPOSITORY {} GIT_TAG {} FIND_PACKAGE_ARGS NAMES {})", m_name, m_gitDepot, m_gitTag, m_name);
		}
	}
}

} // namespace SymGen
