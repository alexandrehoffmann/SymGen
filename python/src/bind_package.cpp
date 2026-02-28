#include <bind_package.hpp>

#include <SymGen/Package.hpp>
#include <SymGen/Dependency.hpp>

#include <pybind11/stl.h>

void bind_package(pybind11::module_& m)
{
	namespace py = pybind11;

	py::class_<SymGen::Package>(m, "Package")

		.def(py::init<std::string_view>(),
			py::arg("name"))

		.def(py::init<std::string_view, std::string_view>(),
			py::arg("name"), py::arg("component"))

		.def("get_name",
			&SymGen::Package::getName)

		.def("get_components",
			[](const SymGen::Package& self)
			{
				const std::span<const std::string> components = self.getComponents();
				return std::vector<std::string>(std::ranges::cbegin(components), std::ranges::cend(components));
			})

		.def("get_min_version",
			&SymGen::Package::getMinVersion)

		.def("get_max_version",
			&SymGen::Package::getMaxVersion)

		.def("get_git_depot",
			&SymGen::Package::getGitDepot)

		.def("get_git_tag",
			&SymGen::Package::getGitTag)

		.def("has_components",
			&SymGen::Package::hasComponents)

		.def("has_version",
			&SymGen::Package::hasVersion)

		.def("has_min_max_version",
			&SymGen::Package::hasMinMaxVersion)

		.def("has_git",
			&SymGen::Package::hasGit)

		.def("satisfy_dependency",
			&SymGen::Package::satisfyDependency,
			py::arg("dependency"))

		.def("set_version",
			py::overload_cast<std::string_view>(&SymGen::Package::setVersion),
			py::arg("version"),
			py::return_value_policy::reference_internal)

		.def("set_version",
			py::overload_cast<std::string_view, std::string_view>(&SymGen::Package::setVersion),
			py::arg("min_version"), py::arg("max_version"),
			py::return_value_policy::reference_internal)

		.def("set_min_version",
			&SymGen::Package::setMinVersion,
			py::arg("min_version"),
			py::return_value_policy::reference_internal)

		.def("set_max_version",
			&SymGen::Package::setMaxVersion,
			py::arg("max_version"),
			py::return_value_policy::reference_internal)

		.def("set_git",
			&SymGen::Package::setGit,
			py::arg("depot"), py::arg("tag"),
			py::return_value_policy::reference_internal)

		.def("add_component",
			&SymGen::Package::addComponent,
			py::arg("component"),
			py::return_value_policy::reference_internal);
}
