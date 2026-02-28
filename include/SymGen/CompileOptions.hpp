#ifndef SYMGEN_COMPILE_OPTIONS_HPP
#define SYMGEN_COMPILE_OPTIONS_HPP

#include <vector>
#include <span>
#include <string>
#include <string_view>

namespace SymGen
{

class CompileOptions : private std::vector<std::string>
{
	using Base = std::vector<std::string>;
public:
	using Iterator = typename Base::const_iterator;

	CompileOptions() {}
	
	inline CompileOptions& addOption(const std::string_view option) { Base::emplace_back(option); return *this; }
	
	inline CompileOptions& addOptions(const std::span<const std::string> options) { Base::insert(Base::end(), std::cbegin(options), std::cend(options)); return *this; }
	
	inline CompileOptions& addOptions(const std::initializer_list<std::string> options) { return addOptions(std::span(options)); }
	
	inline Iterator begin() const { return Base::cbegin(); }
	
	inline Iterator end() const { return Base::cend(); }
};

} // namespace SymGen

#endif // SYMGEN_COMPILE_OPTIONS_HPP
