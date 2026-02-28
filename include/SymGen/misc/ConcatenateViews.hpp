#ifndef SYMGEN_MISC_CONCATENATE_VIEW_HPP
#define SYMGEN_MISC_CONCATENATE_VIEW_HPP

#include <SymGen/misc/CIntergralConstant.hpp>

#include <concepts>
#include <tuple>
#include <ranges>
#include <variant>

namespace SymGen
{
namespace misc
{

template<class... Views>
concept CConcatenableViews = (std::ranges::input_range<Views> and ...) and (std::ranges::view<Views> and ...) and requires
{
    typename std::common_type_t<std::ranges::range_difference_t<Views>...>;
    typename std::common_reference_t<std::ranges::range_reference_t<Views>...>;
};

template<class... Ranges>
concept CConcatenableRanges = CConcatenableViews<std::views::all_t<Ranges>...>;

template<class... Views>
class ConcatenateViews : public std::ranges::view_interface<ConcatenateViews<Views...>>
{
    static_assert(CConcatenableViews<Views...>);
public:
    using Self = ConcatenateViews<Views...>;
    using Base = std::ranges::view_interface<Self>;
    
    using Sentinel = std::default_sentinel_t;
    
    template<bool isConst>
    class Iterator
    {
        template<std::ranges::view View>
        using MaybeConst = std::conditional_t<isConst, std::add_const_t<View>, View>;
        
        template<std::ranges::view View>
        using IteratorFor = std::ranges::iterator_t< MaybeConst<View> >;
        
        template<std::ranges::view View>
        using SentinelFor = std::ranges::sentinel_t< MaybeConst<View> >;
    public:
        using reference = std::common_reference_t<std::ranges::range_reference_t<MaybeConst<Views>>...>;
        using value_type = std::remove_cvref_t<reference>;
        using difference_type = std::common_type_t<std::ranges::range_difference_t<MaybeConst<Views>>...>;
        
        Iterator(const IteratorFor<Views>... firsts, const IteratorFor<Views>... bounds) : m_firsts(firsts...), m_bounds(bounds...) { moveToIndex(std::integral_constant<std::size_t, 0>{}); }
        
        inline reference operator*() const { return getCurrentElement(); }
        
        inline Iterator& operator++() { nextElement(); return *this; }
        
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        
        friend bool operator==(const Iterator& it, const Sentinel /* sentinel */) { return std::holds_alternative<std::default_sentinel_t>(it.m_current); }
        
        friend bool operator!=(const Iterator& it, const Sentinel sentinel) { return not(it == sentinel); }
    private:
        reference getCurrentElement() const;
        
        void nextElement();
    
        template<CIntergralConstant Size> 
        void moveToIndex(const Size size);
        
        void nextNonEmptyView();
        
        bool isAtEndOfCurrentView() const;
    
        // we add a sentinel at the end to express that we have exhausted all the views.
        std::variant< IteratorFor<Views>..., std::default_sentinel_t > m_current;
        
        std::tuple< IteratorFor<Views>...> m_firsts;
        std::tuple< SentinelFor<Views>...> m_bounds;
        
        static constexpr size_t s_nViews = sizeof...(Views);
    };
    
    explicit ConcatenateViews(Views&&... views) : m_views(std::forward<Views>(views)...) {} 
    
    Iterator<false> begin(); 
    
    Iterator<true> begin() const requires(std::ranges::range<const Views> && ...); 
    
    Sentinel end() { return {}; } 
    
    Sentinel end() const requires(std::ranges::range<const Views> and ...) { return {}; } 
    
    std::unsigned_integral auto size() const requires(std::ranges::sized_range<Views> and ...); 
private: 
    std::tuple<Views...> m_views;
};

template<class... Ranges> requires(CConcatenableRanges<Ranges...>)
std::ranges::view auto concatenate(Ranges&&... ranges) { return ConcatenateViews(std::views::all(std::forward<Ranges>(ranges))...); }

} // misc
} // namespace SymGen

#include <SymGen/misc/ConcatenateViews_impl.hpp>

#endif // SYMGEN_MISC_CONCATENATE_VIEW_HPP
