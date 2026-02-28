#ifndef SYMGEN_PROJECT_HPP
#define SYMGEN_PROJECT_HPP

#include <SymGen/Language.hpp>
#include <SymGen/Compiler.hpp>
#include <SymGen/Option.hpp>
#include <SymGen/SubDirectory.hpp>
#include <SymGen/Directory.hpp>
#include <SymGen/misc/ConcatenateViews.hpp>

#include <map>

namespace SymGen
{

class Project : public Directory<Project>
{
	using LanguageCompiler = std::pair<Language, Compiler>;
public:
	using Base = Directory<Project>;

	Project(const std::string_view name);
	
	Project(const std::string_view name, const std::string_view version);
	
	inline std::ranges::view auto getSubDirectories() { return misc::concatenate(m_subDirectories, std::views::elements<0>(m_optionalSubdirectory)); }
	
	inline Project& setDescription(const std::string_view description) { m_description = description; return *this; }
	
	inline Project& addStandard(const Language language, unsigned int standard) { m_standards[language] = standard; return *this; }
	
	inline Project& addStandard(const std::initializer_list<Language> languages, unsigned int standard) { return addStandard(std::span(languages), standard); }
	
	Project& addStandard(const std::span<const Language> languages, unsigned int standard);
	
	inline Project& addOption(const Option& option) { m_options.push_back(option); return *this; }
	
	inline Project& addOption(Option&& option) { m_options.emplace_back(std::forward<Option>(option)); return *this; }
	
	inline Project& addLanguage(const Language language) { m_langages.push_back(language); return *this; }
	
	inline Project& addLanguages(const std::initializer_list<Language> languages) { return addLanguages(std::span(languages)); }
	
	inline Project& addLanguages(const std::span<const Language> languages) { m_langages.insert(std::ranges::end(m_langages), std::ranges::cbegin(languages), std::ranges::cend(languages)); return *this; }
	
	inline Project& addCompileOption(const Language language, const Compiler compiler, const std::string_view option) { m_compilerOptions[LanguageCompiler(language, compiler)].emplace_back(option); return *this; }
	
	inline Project& addCompileOption(const Language language, const std::initializer_list<Compiler> compilers, const std::string_view option) { return addCompileOption(language, std::span(compilers), option); }
	
	Project& addCompileOption(const Language language, const std::span<const Compiler> compilers, const std::string_view option);
	
	Project& addCompileOption(const std::span<const Language> languages, const Compiler compiler, const std::string_view option);
	
	inline Project& addCompileOption(const std::initializer_list<Language> languages, const Compiler compiler, const std::string_view option) { return addCompileOption(std::span(languages), compiler, option); }
	
	Project& addCompileOption(const std::span<const Language> languages, const std::span<const Compiler> compilers, const std::string_view option);
	
	inline Project& addCompileOption(const std::initializer_list<Language> languages, const std::initializer_list<Compiler> compilers, const std::string_view option) { return addCompileOption(std::span(languages), std::span(compilers), option); }
	
	Project& addCompileOptions(const Language language, const Compiler compiler, const std::span<const std::string> options);
	
	Project& addCompileOptions(const Language language, const std::span<const Compiler> compilers, const std::span<const std::string> options);
	
	inline Project& addCompileOptions(const Language language, const std::initializer_list<Compiler> compilers, const std::span<const std::string> options) { return addCompileOptions(language, std::span(compilers), options); }
	
	Project& addCompileOptions(const std::span<const Language> languages, const Compiler compiler, const std::span<const std::string> options);
	
	inline Project& addCompileOptions(const std::initializer_list<Language> languages, const Compiler compiler, const std::span<const std::string> options) { return addCompileOptions(std::span(languages), compiler, options); }
	
	Project& addCompileOptions(const std::span<const Language> languages, const std::span<const Compiler> compilers, const std::span<const std::string> options);
	
	inline Project& addCompileOptions(const std::initializer_list<Language> languages, const std::initializer_list<Compiler> compilers, const std::span<const std::string> options) { return addCompileOptions(std::span(languages), std::span(compilers), options); }
	
	inline Project& addSubDirectory(const SubDirectory& subDirectory) { m_subDirectories.push_back(subDirectory); return *this; }
	
	inline Project& addSubDirectory(SubDirectory&& subDirectory) { m_subDirectories.emplace_back(std::forward<SubDirectory>(subDirectory)); return *this; }
	
	inline Project& addOptionalSubDirectory(const SubDirectory& subDirectory, const Option& option) { m_optionalSubdirectory.emplace_back(subDirectory, option); return *this; }
	
	inline Project& addOptionalSubDirectory(SubDirectory&& subDirectory, Option&& option) { m_optionalSubdirectory.emplace_back(std::forward<SubDirectory>(subDirectory), std::forward<Option>(option)); return *this; }
	
	Project& createDirectories(const std::string_view path = "./");
	
	void toCMakeListsImpl(std::FILE* file);
private:
	inline std::ranges::view auto getAllOptions() const { return misc::concatenate(m_options, std::views::elements<1>(m_optionalSubdirectory)); }

	void mergeCommonCompileOptions();
	
	void addDefaultLibraryToDependencies();

	void fillHelperLibraries();
	
	void fillDefaultSubDir();
	
	void fillDefaultOptions();
	
	void checkIntegrity() const;
	
	bool satisfyDependency(const Dependency& dependency) const;
	
	std::string                                            m_version;
	std::string                                            m_description;
	std::vector<Language>                                  m_langages;
	std::map<Language, unsigned int>                       m_standards;
	std::map< LanguageCompiler, std::vector<std::string> > m_compilerOptions;
	std::vector<std::string>                               m_commonCompileOptions;
	std::vector<Option>                                    m_options;
	std::vector<SubDirectory>                              m_subDirectories;
	std::vector< std::pair<SubDirectory, Option> >         m_optionalSubdirectory;
	std::size_t                                            m_optionHelperLibId;
};

} // namespace SymGen

#endif // SYMGEN_PROJECT_HPP
