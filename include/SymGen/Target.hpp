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
	
	inline bool hasCompileDefinitions() const { return m_privateCompileDefinitions.empty(); }
	
	inline std::string_view getName() const { return m_name; }

	inline std::span<const std::string> getSources() const { return m_sources; }
	
	inline std::span<const std::string> getPublicCompileDefinitions() const { return m_publicCompileDefinitions; }
	
	inline std::span<const std::string> getPrivateCompileDefinitions() const { return m_privateCompileDefinitions; }
	
	inline Derived& addSource(const std::string_view source) { m_sources.emplace_back(source); return Base::derived(); }
	
	inline Derived& addSources(const std::initializer_list<std::string> sources) { addSources(std::span(sources)); }
	
	inline Derived& addSources(const std::span<const std::string> sources) { m_sources.insert(std::ranges::end(m_sources), std::ranges::cbegin(sources), std::ranges::cend(sources)); return Base::derived(); }
	
	inline Derived& addPublicCompileDefinition(const std::string_view publicCompileDefinition) { m_publicCompileDefinitions.emplace_back(publicCompileDefinition); return Base::derived(); }
	
	inline Derived& addPublicCompileDefinitions(const std::initializer_list<std::string> publicCompileDefinitions) { addPublicCompileDefinitions(std::span(publicCompileDefinitions)); }
	
	inline Derived& addPublicCompileDefinitions(const std::span<const std::string> publicCompileDefinitions) { m_publicCompileDefinitions.insert(std::ranges::end(m_publicCompileDefinitions), std::ranges::cbegin(publicCompileDefinitions), std::ranges::cend(publicCompileDefinitions)); return Base::derived(); }
	
	inline Derived& addPrivateCompileDefinition(const std::string_view privateCompileDefinition) { m_privateCompileDefinitions.emplace_back(privateCompileDefinition); return Base::derived(); }
	
	inline Derived& addPrivateCompileDefinitions(const std::initializer_list<std::string> privateCompileDefinitions) { addPrivateCompileDefinitions(std::span(privateCompileDefinitions)); }
	
	inline Derived& addPrivateCompileDefinitions(const std::span<const std::string> privateCompileDefinitions) { m_privateCompileDefinitions.insert(std::ranges::end(m_privateCompileDefinitions), std::ranges::cbegin(privateCompileDefinitions), std::ranges::cend(privateCompileDefinitions)); return Base::derived(); }
protected:
	std::string              m_name;
	std::vector<std::string> m_sources;
	std::vector<std::string> m_privateCompileDefinitions;
	std::vector<std::string> m_publicCompileDefinitions;
};

} // namespace SymGen

#endif // SYMGEN_TARGET_HPP
