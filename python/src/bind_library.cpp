#include <bind_library.hpp>
#include <bind_target_methods.hpp>

#include <SymGen/Library.hpp>

#include <pybind11/stl.h>

void bind_library(pybind11::module_& m)
{
	namespace py = pybind11;

	py::class_<SymGen::Library> library(m, "Library");

	library.def(py::init<std::string_view>(),
		py::arg("name"));

	bind_target_methods(library);

	library.def("get_public_dependencies",
		[](const SymGen::Library& self)
		{
			const std::span<const SymGen::Dependency> deps = self.getPublicDependencies();
			return std::vector<SymGen::Dependency>(std::ranges::cbegin(deps), std::ranges::cend(deps));
		});

	library.def("add_public_dependency",
		py::overload_cast<std::string_view>(&SymGen::Library::addPublicDependency),
		py::arg("dependency"),
		py::return_value_policy::reference_internal);

	library.def("add_public_dependency",
		py::overload_cast<std::string_view, std::string_view>(&SymGen::Library::addPublicDependency),
		py::arg("package"), py::arg("component"),
		py::return_value_policy::reference_internal);

	library.def("get_private_dependencies",
		[](const SymGen::Library& self)
		{
			const std::span<const SymGen::Dependency> deps = self.getPrivateDependencies();
			return std::vector<SymGen::Dependency>(std::ranges::cbegin(deps), std::ranges::cend(deps));
		});

	library.def("add_private_dependency",
		py::overload_cast<std::string_view>(&SymGen::Library::addPrivateDependency),
		py::arg("dependency"),
		py::return_value_policy::reference_internal);

	library.def("add_private_dependency",
		py::overload_cast<std::string_view, std::string_view>(&SymGen::Library::addPrivateDependency),
		py::arg("package"), py::arg("component"),
		py::return_value_policy::reference_internal);
}
