#ifndef SYMGEN_MISC_CINTEGRAL_CONSTANT_HPP
#define SYMGEN_MISC_CINTEGRAL_CONSTANT_HPP

#include <type_traits>
#include <concepts>

namespace SymGen
{
namespace misc
{

template<typename T>
concept CIntergralConstant = requires
{
  typename T::value_type;
} and std::same_as<T, std::integral_constant<typename T::value_type, T::value>>;

} // misc
} // namespace SymGen

#endif // SYMGEN_MISC_CINTEGRAL_CONSTANT_HPP
