#ifndef SYMGEN_SUB_DIRECTORY_HPP
#define SYMGEN_SUB_DIRECTORY_HPP

#include <SymGen/Directory.hpp>

namespace SymGen
{

class SubDirectory : public Directory<SubDirectory>
{
public:
	using Base = Directory<SubDirectory>;

	SubDirectory(const std::string_view name) : Base(name) {}
	
	SubDirectory(const std::string_view name, const std::string_view parentCmakePrefix) : Base(name, parentCmakePrefix) {}
	
	void toCMakeListsImpl(std::FILE* file);
};

} // namespace SymGen

#endif // SYMGEN_SUB_DIRECTORY_HPP
