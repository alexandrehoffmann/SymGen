#include <bind_dependency.hpp>

#include <SymGen/Dependency.hpp>

void bind_dependency(pybind11::module_& m)
{
	namespace py = pybind11;

	py::class_<SymGen::Dependency>(m, "Dependency")

		.def(py::init<std::string_view>(),
			py::arg("name"))

		.def(py::init<std::string_view, std::string_view>(),
			py::arg("package"), py::arg("component"))

		.def("get_package",
			&SymGen::Dependency::getPackage)

		.def("get_component",
			[](const SymGen::Dependency& self) -> pybind11::object
			{
				if (!self.hasComponent()) return pybind11::none();
				return pybind11::str(self.getComponent());
			})

		.def("has_component",
			&SymGen::Dependency::hasComponent)

		.def("__repr__",
			[](const SymGen::Dependency& self)
			{
				return fmt::format("{}", self);
			});
}
