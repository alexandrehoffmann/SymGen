#ifndef SYMGEN_TARGET_HPP
#define SYMGEN_TARGET_HPP

#include <SymGen/CRTPBase.hpp>

#include <string>
#include <string_view>
#include <vector>
#include <span>

namespace SymGen
{

template<class Derived>
class Target : public CRTPBase<Derived>
{
public:
	using Base = CRTPBase<Derived>;

	Target(const std::string_view name) : m_name(name) {}
	
	inline bool isInterface() const { return m_sources.empty(); }
	
	inline std::string_view getName() const { return m_name; }

	inline std::span<const std::string> getSources() const { return m_sources; }
	
	inline Derived& addSource(const std::string_view source) { m_sources.emplace_back(source); return Base::derived(); }
	
	inline Derived& addSources(const std::initializer_list<std::string> sources) { addSources(std::span(sources)); }
	
	inline Derived& addSources(const std::span<const std::string> sources) { m_sources.insert(std::ranges::end(m_sources), std::ranges::cbegin(sources), std::ranges::cend(sources)); return Base::derived(); }
protected:
	std::string              m_name;
	std::vector<std::string> m_sources;
};

} // namespace SymGen

#endif // SYMGEN_TARGET_HPP
