#include <bind_option.hpp>

#include <SymGen/Option.hpp>

void bind_option(pybind11::module_& m)
{
	namespace py = pybind11;

	py::class_<SymGen::Option>(m, "Option")

		.def(py::init<std::string_view>(),
			py::arg("name"))

		.def(py::init<std::string_view, std::string_view>(),
			py::arg("name"), py::arg("description"))

		.def(py::init<std::string_view, std::string_view, int>(),
			py::arg("name"), py::arg("description"), py::arg("default_value"))

		.def("get_name",
			&SymGen::Option::getName)

		.def("get_description",
			&SymGen::Option::getDescription)

		.def("get_default_value",
			&SymGen::Option::getDefaultValue)

		.def("__repr__",
			[](const SymGen::Option& self)
			{
				return fmt::format("{}", self);
			});
}
