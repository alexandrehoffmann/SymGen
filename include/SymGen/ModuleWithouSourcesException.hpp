#ifndef SYMGEN_EXECUTABLE_WITHOU_SOURCES_EXCEPTION_HPP
#define SYMGEN_EXECUTABLE_WITHOU_SOURCES_EXCEPTION_HPP

#include <stdexcept>
#include <string_view>

namespace SymGen
{

class ModuleWithouSourcesException : public std::runtime_error
{
public:
	ModuleWithouSourcesException(const std::string_view message) : std::runtime_error(message.data()) {}
};

} // namespace SymGen

#endif // SYMGEN_EXECUTABLE_WITHOU_SOURCES_EXCEPTION_HPP
