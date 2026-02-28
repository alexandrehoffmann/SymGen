#include <bind_executable.hpp>
#include <bind_target_methods.hpp>

#include <SymGen/Executable.hpp>

#include <pybind11/stl.h>

void bind_executable(pybind11::module_& m)
{
	namespace py = pybind11;

	py::class_<SymGen::Executable> executable(m, "Executable");

	executable.def(py::init<std::string_view>(),
		py::arg("name"));

	bind_target_methods(executable);

	executable.def("get_dependencies",
		[](const SymGen::Executable& self)
		{
			const std::span<const SymGen::Dependency> deps = self.getDependencies();
			return std::vector<SymGen::Dependency>(std::ranges::cbegin(deps), std::ranges::cend(deps));
		});

	executable.def("add_dependency",
		py::overload_cast<std::string_view>(&SymGen::Executable::addDependency),
		py::arg("dependency"),
		py::return_value_policy::reference_internal);

	executable.def("add_dependency",
		py::overload_cast<std::string_view, std::string_view>(&SymGen::Executable::addDependency),
		py::arg("package"), py::arg("component"),
		py::return_value_policy::reference_internal);
}
