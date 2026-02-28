#ifndef SYMEG_LANGUAGES_HPP
#define SYMEG_LANGUAGES_HPP

#include <fmt/format.h>

namespace SymGen
{

enum class Language { C, CXX };

} // namespace SymGen

template<>
struct fmt::formatter<SymGen::Language> : fmt::formatter<std::string_view>
{
    auto format(const SymGen::Language language, fmt::format_context& ctx) const
    {
		switch (language)
		{
			case SymGen::Language::C:   return fmt::formatter<std::string_view>::format("C",   ctx);
			case SymGen::Language::CXX: return fmt::formatter<std::string_view>::format("CXX", ctx);
		}
		return fmt::formatter<std::string_view>::format("unknown", ctx);
    }
};

#endif // SYMEG_LANGUAGES_HPP
