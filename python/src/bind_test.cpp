#include <bind_test.hpp>

#include <SymGen/Test.hpp>

#include <pybind11/stl.h>

void bind_test(pybind11::module_& m)
{
	namespace py = pybind11;

	py::class_<SymGen::Test>(m, "Test")

		.def(py::init<std::string_view, std::string_view>(),
			py::arg("name"), py::arg("command"))

		.def("get_name",
			&SymGen::Test::getName)

		.def("get_command",
			&SymGen::Test::getCommand)

		.def("get_args",
			[](const SymGen::Test& self)
			{
				const std::span<const std::string> args = self.getArgs();
				return std::vector<std::string>(std::ranges::cbegin(args), std::ranges::cend(args));
			})

		.def("add_arg",
			&SymGen::Test::addArg,
			py::arg("arg"),
			py::return_value_policy::reference_internal)

		.def("add_args",
			[](SymGen::Test& self, const std::vector<std::string>& args) -> SymGen::Test&
			{
				return self.addArgs(std::span(args));
			},
			py::arg("args"),
			py::return_value_policy::reference_internal);
}
