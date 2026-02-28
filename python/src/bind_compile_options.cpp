#include <bind_compile_options.hpp>

#include <SymGen/CompileOptions.hpp>

#include <pybind11/stl.h>

void bind_compile_options(pybind11::module_& m)
{
	namespace py = pybind11;

	py::class_<SymGen::CompileOptions>(m, "CompileOptions")

		.def(py::init<>())

		.def("add_option",
			&SymGen::CompileOptions::addOption,
			py::arg("option"),
			py::return_value_policy::reference_internal)

		.def("add_options",
			[](SymGen::CompileOptions& self, const std::vector<std::string>& options) -> SymGen::CompileOptions&
			{
				return self.addOptions(std::span(options));
			},
			py::arg("options"),
			py::return_value_policy::reference_internal)

		.def("__iter__",
			[](const SymGen::CompileOptions& self)
			{
				return py::make_iterator(std::ranges::cbegin(self), std::ranges::cend(self));
			},
			py::keep_alive<0, 1>());
}
