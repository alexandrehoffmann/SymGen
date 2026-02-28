#include <bind_project.hpp>
#include <bind_directory_methods.hpp>

#include <SymGen/Project.hpp>

#include <pybind11/stl.h>

void bind_project(pybind11::module_& m)
{
	namespace py = pybind11;

	py::class_<SymGen::Project> project(m, "Project");

	// ---- Constructors -------------------------------------------------------

	project.def(py::init<std::string_view>(),
		py::arg("name"));

	project.def(py::init<std::string_view, std::string_view>(),
		py::arg("name"), py::arg("version"));

	// ---- Inherited Directory<Project> methods --------------------------------

	bind_directory_methods(project);

	// ---- Project-specific setters -------------------------------------------

	project.def("set_description",
		&SymGen::Project::setDescription,
		py::arg("description"),
		py::return_value_policy::reference_internal);

	// ---- Standards ----------------------------------------------------------

	project.def("add_standard",
		py::overload_cast<SymGen::Language, unsigned int>(&SymGen::Project::addStandard),
		py::arg("language"), py::arg("standard"),
		py::return_value_policy::reference_internal);

	project.def("add_standard",
		[](SymGen::Project& self,
		   const std::vector<SymGen::Language>& languages,
		   unsigned int standard) -> SymGen::Project&
		{
			return self.addStandard(std::span(languages), standard);
		},
		py::arg("languages"), py::arg("standard"),
		py::return_value_policy::reference_internal);

	// ---- Options ------------------------------------------------------------

	project.def("add_option",
		py::overload_cast<const SymGen::Option&>(&SymGen::Project::addOption),
		py::arg("option"),
		py::return_value_policy::reference_internal);

	// ---- Languages ----------------------------------------------------------

	project.def("add_language",
		&SymGen::Project::addLanguage,
		py::arg("language"),
		py::return_value_policy::reference_internal);

	project.def("add_languages",
		[](SymGen::Project& self, const std::vector<SymGen::Language>& languages) -> SymGen::Project&
		{
			return self.addLanguages(std::span(languages));
		},
		py::arg("languages"),
		py::return_value_policy::reference_internal);

	// ---- Compile options ----------------------------------------------------

	project.def("add_compile_option",
		py::overload_cast<SymGen::Language, SymGen::Compiler, std::string_view>(
			&SymGen::Project::addCompileOption),
		py::arg("language"), py::arg("compiler"), py::arg("option"),
		py::return_value_policy::reference_internal);

	project.def("add_compile_option",
		[](SymGen::Project& self,
		   SymGen::Language language,
		   const std::vector<SymGen::Compiler>& compilers,
		   std::string_view option) -> SymGen::Project&
		{
			return self.addCompileOption(language, std::span(compilers), option);
		},
		py::arg("language"), py::arg("compilers"), py::arg("option"),
		py::return_value_policy::reference_internal);

	project.def("add_compile_option",
		[](SymGen::Project& self,
		   const std::vector<SymGen::Language>& languages,
		   SymGen::Compiler compiler,
		   std::string_view option) -> SymGen::Project&
		{
			return self.addCompileOption(std::span(languages), compiler, option);
		},
		py::arg("languages"), py::arg("compiler"), py::arg("option"),
		py::return_value_policy::reference_internal);

	project.def("add_compile_option",
		[](SymGen::Project& self,
		   const std::vector<SymGen::Language>& languages,
		   const std::vector<SymGen::Compiler>& compilers,
		   std::string_view option) -> SymGen::Project&
		{
			return self.addCompileOption(std::span(languages), std::span(compilers), option);
		},
		py::arg("languages"), py::arg("compilers"), py::arg("option"),
		py::return_value_policy::reference_internal);

	// ---- Compile options (bulk) ---------------------------------------------

	project.def("add_compile_options",
		[](SymGen::Project& self,
		   SymGen::Language language,
		   SymGen::Compiler compiler,
		   const std::vector<std::string>& options) -> SymGen::Project&
		{
			return self.addCompileOptions(language, compiler, std::span(options));
		},
		py::arg("language"), py::arg("compiler"), py::arg("options"),
		py::return_value_policy::reference_internal);

	project.def("add_compile_options",
		[](SymGen::Project& self,
		   SymGen::Language language,
		   const std::vector<SymGen::Compiler>& compilers,
		   const std::vector<std::string>& options) -> SymGen::Project&
		{
			return self.addCompileOptions(language, std::span(compilers), std::span(options));
		},
		py::arg("language"), py::arg("compilers"), py::arg("options"),
		py::return_value_policy::reference_internal);

	project.def("add_compile_options",
		[](SymGen::Project& self,
		   const std::vector<SymGen::Language>& languages,
		   SymGen::Compiler compiler,
		   const std::vector<std::string>& options) -> SymGen::Project&
		{
			return self.addCompileOptions(std::span(languages), compiler, std::span(options));
		},
		py::arg("languages"), py::arg("compiler"), py::arg("options"),
		py::return_value_policy::reference_internal);

	project.def("add_compile_options",
		[](SymGen::Project& self,
		   const std::vector<SymGen::Language>& languages,
		   const std::vector<SymGen::Compiler>& compilers,
		   const std::vector<std::string>& options) -> SymGen::Project&
		{
			return self.addCompileOptions(std::span(languages), std::span(compilers), std::span(options));
		},
		py::arg("languages"), py::arg("compilers"), py::arg("options"),
		py::return_value_policy::reference_internal);

	// ---- Sub-directories ----------------------------------------------------

	project.def("add_sub_directory",
		py::overload_cast<const SymGen::SubDirectory&>(&SymGen::Project::addSubDirectory),
		py::arg("sub_directory"),
		py::return_value_policy::reference_internal);

	project.def("add_optional_sub_directory",
		py::overload_cast<const SymGen::SubDirectory&, const SymGen::Option&>(
			&SymGen::Project::addOptionalSubDirectory),
		py::arg("sub_directory"), py::arg("option"),
		py::return_value_policy::reference_internal);

	project.def("get_sub_directories",
		[](SymGen::Project& self)
		{
			const std::ranges::view auto subDirectories = self.getSubDirectories();
			std::vector<SymGen::SubDirectory> ret;
			
			ret.reserve(subDirectories.size());
			
			for (const SymGen::SubDirectory& subDirectory : subDirectories)
			{
				ret.push_back(subDirectory);
			}
			
			return ret;
		});

	// ---- Output -------------------------------------------------------------

	project.def("create_directories",
		[](SymGen::Project& self, std::string_view path)
		{
			self.createDirectories(path);
		},
		py::arg("path") = ".");

	project.def("__repr__",
		[](const SymGen::Project& self)
		{
			return fmt::format("Project(\"{}\")", self.getName());
		});
}
