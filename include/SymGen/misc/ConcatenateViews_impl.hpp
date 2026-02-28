#ifndef SYMGEN_MISC_CONCATENATE_VIEW_IMPL_HPP
#define SYMGEN_MISC_CONCATENATE_VIEW_IMPL_HPP

#include <SymGen/misc/ConcatenateViews.hpp>
#include <SymGen/utils.hpp>

namespace SymGen
{

namespace misc
{

template<class... Views> template<bool isConst>
auto ConcatenateViews<Views...>::Iterator<isConst>::getCurrentElement() const -> reference
{
    return std::visit([]<typename Current>(Current& current) -> reference
    {
        if constexpr (std::same_as<std::decay_t<Current>, std::default_sentinel_t>)
        {
            // TODO: switch to std::unreachable when switching to C++23 
#if defined(_MSC_VER)
            __assume(false);
#else
            __builtin_unreachable();
#endif      
        }
        else
        {
            return *current;
        }
    }, m_current);
}

template<class... Views> template<bool isConst> 
auto ConcatenateViews<Views...>::Iterator<isConst>::nextElement() -> void
{
    std::visit([]<typename Current>(Current& current) -> void
    {
        if constexpr (not std::same_as<std::decay_t<Current>, std::default_sentinel_t>)
        {
            ++current;
        }
    }, m_current);
    
    if (isAtEndOfCurrentView()) { nextNonEmptyView(); }
}

template<class... Views> template<bool isConst> template<CIntergralConstant Size> 
auto ConcatenateViews<Views...>::Iterator<isConst>::moveToIndex(const Size i) -> void
{   
    if constexpr (i != s_nViews)
    {
        if (std::get<i>(m_firsts) != std::get<i>(m_bounds)) { m_current.template emplace<i>(std::get<i>(m_firsts)); }
        else                                                { moveToIndex(utils::next(i));                          }
    }
    else
    {
        // Terminal state: Exhausted all ranges.
        // Set current to the sentinel of the very last range.
        m_current.template emplace<std::default_sentinel_t>();
    }
}

template<class... Views> template<bool isConst>
auto ConcatenateViews<Views...>::Iterator<isConst>::nextNonEmptyView() -> void
{
    using N = std::integral_constant<std::size_t, s_nViews>;
    
    utils::staticSwitch(N{}, m_current.index(), [this](const CIntergralConstant auto i) -> void
    {
        moveToIndex(utils::next(i));
    });
}

template<class... Views> template<bool isConst>
auto ConcatenateViews<Views...>::Iterator<isConst>::isAtEndOfCurrentView() const -> bool
{
    using N = std::integral_constant<std::size_t, s_nViews>;
    
    if (std::holds_alternative<std::default_sentinel_t>(m_current)) { return true; }
    
    bool isAtEnd = false;
    utils::staticSwitch(N{}, m_current.index(), [&isAtEnd, &current = m_current, &bounds = m_bounds](const CIntergralConstant auto i) -> void
    {
        isAtEnd = (std::get<i>(current) == std::get<i>(bounds));
    });
    
    return isAtEnd;
}

template<class... Views>
auto ConcatenateViews<Views...>::begin() -> Iterator<false>
{
    return std::apply([](auto&... views) -> Iterator<false>
    {
        return Iterator<false>(std::begin(views)..., std::end(views)...);
    }, m_views);
}

template<class... Views>
auto ConcatenateViews<Views...>::begin() const -> Iterator<true> requires(std::ranges::range<const Views> and ...)
{
    return std::apply([](const auto&... views) -> Iterator<true>
    {
        return Iterator<true>(std::cbegin(views)..., std::cend(views)...);
    }, m_views);
}

template<class... Views>
std::unsigned_integral auto ConcatenateViews<Views...>::size() const requires(std::ranges::sized_range<Views> and ...)
{
    return std::apply([](const auto&... views) -> std::unsigned_integral auto
    {
        return (views.size() + ...); 
    }, m_views);
}

} // misc
} // namespace SymGen

#endif // SYMGEN_MISC_CONCATENATE_VIEW_IMPL_HPP
