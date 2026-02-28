#ifndef SYMGEN_PACKAGE_HPP
#define SYMGEN_PACKAGE_HPP

#include <vector>
#include <string>
#include <string_view>
#include <span>

namespace SymGen
{
	
class Dependency;

class Package
{
public:
	Package(const std::string_view name) : m_name(name) {}
	
	Package(const std::string_view name, const std::string_view component) : m_name(name) { m_components.emplace_back(component); }
	
	inline std::string_view getName() const { return m_name; }
	
	inline std::span<const std::string> getComponents() const { return m_components; }
	
	inline std::string_view getMinVersion() const { return m_minVersion; }
	
	inline std::string_view getMaxVersion() const { return m_maxVersion; }
	
	inline std::string_view getGitDepot() const { return m_gitDepot; }
	
	inline std::string_view getGitTag() const { return m_gitTag; }
	
	inline bool hasComponents() const { return m_components.size() != 0; }
	
	inline bool hasVersion() const { return m_minVersion.size() != 0; }
	
	inline bool hasMinMaxVersion() const { return hasVersion() and m_minVersion != m_maxVersion; }
	
	inline bool hasGit() const { return m_gitDepot.size() != 0; }
	
	bool satisfyDependency(const Dependency& dependency) const;
	
	inline Package& setVersion(const std::string_view version) { m_minVersion = version; m_maxVersion = version; return *this; }
	
	inline Package& setVersion(const std::string_view minVersion, const std::string_view maxVersion) { m_minVersion = minVersion; m_maxVersion = maxVersion; return *this; }
	
	inline Package& setMinVersion(const std::string_view minVersion) { m_minVersion = minVersion; return *this; }
	
	inline Package& setMaxVersion(const std::string_view maxVersion) { m_maxVersion = maxVersion; return *this; }
	
	inline Package& setGit(const std::string_view depot, const std::string_view tag) { m_gitDepot = depot; m_gitTag = tag; return *this; }
	
	inline Package& addComponent(const std::string_view component) { m_components.emplace_back(component); return *this; }
	
	void writeFindPackage(std::FILE* fout) const;
	
	void writeFetchContent(std::FILE* fout) const;
private:
	std::string              m_name;       // e.g. Eigen3
	std::vector<std::string> m_components; // e.g. Eigen  
	std::string              m_minVersion; // e.g. 0.3.3
	std::string              m_maxVersion; // e.g. 0.3.3
	std::string              m_gitDepot; 
	std::string              m_gitTag; 
};

} // namespace SymGen

#endif // SYMGEN_PACKAGE_HPP
