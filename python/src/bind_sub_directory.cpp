#include <bind_sub_directory.hpp>
#include <bind_directory_methods.hpp>

#include <SymGen/SubDirectory.hpp>

void bind_sub_directory(pybind11::module_& m)
{
	namespace py = pybind11;

	py::class_<SymGen::SubDirectory> subDirectory(m, "SubDirectory");

	subDirectory.def(py::init<std::string_view>(),
		py::arg("name"));

	subDirectory.def(py::init<std::string_view, std::string_view>(),
		py::arg("name"), py::arg("parent_cmake_prefix"));

	bind_directory_methods(subDirectory);
}
