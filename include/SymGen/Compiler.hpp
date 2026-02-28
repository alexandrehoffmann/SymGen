#ifndef SYMEG_COMPILERS_HPP
#define SYMEG_COMPILERS_HPP

#include <fmt/format.h>

namespace SymGen
{

enum class Compiler { GNU, CLANG, MSV };

} // namespace SymGen

template<>
struct fmt::formatter<SymGen::Compiler> : fmt::formatter<std::string_view>
{
    auto format(const SymGen::Compiler language, fmt::format_context& ctx) const
    {
		switch (language)
		{
			case SymGen::Compiler::GNU:   return fmt::formatter<std::string_view>::format("GNU",   ctx);
			case SymGen::Compiler::CLANG: return fmt::formatter<std::string_view>::format("CLANG", ctx);
			case SymGen::Compiler::MSV:   return fmt::formatter<std::string_view>::format("MSV",   ctx);
		}
		// fallback (should never happen)
        return fmt::formatter<std::string_view>::format("Unknown", ctx);
    }
};

#endif // SYMEG_LANGUAGES_HPP
