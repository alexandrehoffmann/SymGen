#ifndef SYMGEN_PYTHON_BINDINGS_BIND_DIRECTORY_METHODS_HPP
#define SYMGEN_PYTHON_BINDINGS_BIND_DIRECTORY_METHODS_HPP

#include <pybind11/pybind11.h>

#include <core.hpp>

#include <SymGen/SubDirectory.hpp>

template <typename Derived>
inline void bind_directory_methods(pybind11::class_<Derived>& derived)
{
    namespace py = pybind11;

    derived.def("get_name",
        &SymGen::Directory<Derived>::getName);

    derived.def("get_cmake_prefix",
        &SymGen::Directory<Derived>::getCmakePrefix);

    derived.def("set_cmake_prefix",
        &SymGen::Directory<Derived>::setCmakePrefix,
        py::arg("cmake_prefix"),
        py::return_value_policy::reference_internal);

    derived.def("set_include_dir",
        &SymGen::Directory<Derived>::setIncludeDir,
        py::arg("include_dir"),
        py::return_value_policy::reference_internal);

    derived.def("add_dependency",
        py::overload_cast<const SymGen::Package&>(&SymGen::Directory<Derived>::addDependency),
        py::arg("dependency"),
        py::return_value_policy::reference_internal);

    derived.def("add_dependency",
        py::overload_cast<const std::string_view>(&SymGen::Directory<Derived>::addDependency),
        py::arg("dependency"),
        py::return_value_policy::reference_internal);

    derived.def("add_library",
        py::overload_cast<const SymGen::Library&>(&SymGen::Directory<Derived>::addLibrary),
        py::arg("library"),
        py::return_value_policy::reference_internal);

    derived.def("add_executable",
        py::overload_cast<const SymGen::Executable&>(&SymGen::Directory<Derived>::addExecutable),
        py::arg("executable"),
        py::return_value_policy::reference_internal);

    derived.def("add_pybind11_module",
        py::overload_cast<const SymGen::Pybind11Module&>(&SymGen::Directory<Derived>::addPybind11Module),
        py::arg("module"),
        py::return_value_policy::reference_internal);

    derived.def("add_test",
        py::overload_cast<const SymGen::Test&>(&SymGen::Directory<Derived>::addTest),
        py::arg("test"),
        py::return_value_policy::reference_internal);

    // Convenience overload: add_test("name", "command")
    derived.def("add_test",
        [](Derived& self, std::string_view name, std::string_view command) -> Derived&
        {
            return self.addTest(SymGen::Test(name, command));
        },
        py::arg("name"), py::arg("command"),
        py::return_value_policy::reference_internal);

    // String path overload FIRST to avoid dispatch confusion
    derived.def("to_cmake_lists",
        [](Derived& self, py::object arg)
        {
            if (py::isinstance<py::str>(arg))
            {
                self.toCMakeLists(arg.cast<std::string_view>());
            }
            else
            {
                const int fd = PyObject_AsFileDescriptor(arg.ptr());
                if (fd < 0) { throw py::error_already_set(); }
                std::FILE* fout = fdopen(dup(fd), "w");
                if (!fout) { throw std::runtime_error("fdopen failed"); }
                self.toCMakeLists(fout);
                std::fclose(fout);
            }
        },
        py::arg("file_or_path"),
        "Write CMakeLists.txt to a file object or path string");
}

#endif // SYMGEN_PYTHON_BINDINGS_BIND_DIRECTORY_METHODS_HPP
