#ifndef SYMGEN_MISSING_PACKAGE_EXCEPTION_HPP
#define SYMGEN_MISSING_PACKAGE_EXCEPTION_HPP

#include <stdexcept>
#include <string_view>

namespace SymGen
{

class MissingPackageExeception : public std::runtime_error
{
public:
	MissingPackageExeception(const std::string_view message) : std::runtime_error(message.data()) {}
};

} // namespace SymGen

#endif // SYMGEN_MISSING_PACKAGE_EXCEPTION_HPP
