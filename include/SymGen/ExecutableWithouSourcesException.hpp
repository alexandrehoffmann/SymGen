#ifndef SYMGEN_EXECUTABLE_WITHOUT_SOURCES_EXCEPTION_HPP
#define SYMGEN_EXECUTABLE_WITHOUT_SOURCES_EXCEPTION_HPP

#include <stdexcept>
#include <string_view>

namespace SymGen
{

class ExecutableWithouSourcesException : public std::runtime_error
{
public:
	ExecutableWithouSourcesException(const std::string_view message) : std::runtime_error(message.data()) {}
};

} // namespace SymGen

#endif // SYMGEN_EXECUTABLE_WITHOUT_SOURCES_EXCEPTION_HPP
