#include <bind_pybind11_module.hpp>
#include <bind_target_methods.hpp>

#include <SymGen/Pybind11Module.hpp>

#include <pybind11/stl.h>

void bind_pybind11_module(pybind11::module_& m)
{
	namespace py = pybind11;

	py::class_<SymGen::Pybind11Module> pybind11Module(m, "Pybind11Module");

	pybind11Module.def(py::init<std::string_view>(),
		py::arg("name"));

	bind_target_methods(pybind11Module);

	pybind11Module.def("get_dependencies",
		[](const SymGen::Pybind11Module& self)
		{
			const std::span<const SymGen::Dependency> deps = self.getDependencies();
			return std::vector<SymGen::Dependency>(std::ranges::cbegin(deps), std::ranges::cend(deps));
		});

	pybind11Module.def("add_dependency",
		py::overload_cast<std::string_view>(&SymGen::Pybind11Module::addDependency),
		py::arg("dependency"),
		py::return_value_policy::reference_internal);

	pybind11Module.def("add_dependency",
		py::overload_cast<std::string_view, std::string_view>(&SymGen::Pybind11Module::addDependency),
		py::arg("package"), py::arg("component"),
		py::return_value_policy::reference_internal);
}
