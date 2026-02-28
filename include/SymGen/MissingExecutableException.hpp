#ifndef SYMGEN_MISSING_EXECUTABLE_EXCEPTION_HPP
#define SYMGEN_MISSING_EXECUTABLE_EXCEPTION_HPP

#include <stdexcept>
#include <string_view>

namespace SymGen
{

class MissingExecutableException : public std::runtime_error
{
public:
	MissingExecutableException(const std::string_view message) : std::runtime_error(message.data()) {}
};

} // namespace SymGen

#endif // SYMGEN_MISSING_EXECUTABLE_HPP_HPP
