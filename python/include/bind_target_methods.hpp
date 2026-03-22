#ifndef SYMGEN_PYTHON_BINDINGS_BIND_TARGET_METHODS_HPP
#define SYMGEN_PYTHON_BINDINGS_BIND_TARGET_METHODS_HPP

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <SymGen/Target.hpp>

template<typename Derived>
inline void bind_target_methods(pybind11::class_<Derived>& cls)
{
    namespace py = pybind11;

    cls.def("is_interface",
        &SymGen::Target<Derived>::isInterface);

    cls.def("get_name",
        &SymGen::Target<Derived>::getName);

    cls.def("get_sources",
        [](const Derived& self)
        {
            const std::span<const std::string> sources = self.getSources();
            return std::vector<std::string>(std::ranges::cbegin(sources), std::ranges::cend(sources));
        });

    cls.def("get_public_compile_definitions",
        [](const Derived& self)
        {
            const std::span<const std::string> sources = self.getPublicCompileDefinitions();
            return std::vector<std::string>(std::ranges::cbegin(sources), std::ranges::cend(sources));
        });

    cls.def("get_private_compile_definitions",
        [](const Derived& self)
        {
            const std::span<const std::string> sources = self.getPrivateCompileDefinitions();
            return std::vector<std::string>(std::ranges::cbegin(sources), std::ranges::cend(sources));
        });

    cls.def("add_source",
        &SymGen::Target<Derived>::addSource,
        py::arg("source"),
        py::return_value_policy::reference_internal);

    cls.def("add_sources",
        [](Derived& self, const std::vector<std::string>& sources) -> Derived&
        {
            return self.addSources(std::span(sources));
        },
        py::arg("sources"),
        py::return_value_policy::reference_internal);

    cls.def("add_public_compile_option",
        &SymGen::Target<Derived>::addPublicCompileDefinition,
        py::arg("compile_option"),
        py::return_value_policy::reference_internal);

    cls.def("add_public_compile_options",
        [](Derived& self, const std::vector<std::string>& sources) -> Derived&
        {
            return self.addPublicCompileDefinitions(std::span(sources));
        },
        py::arg("compile_options"),
        py::return_value_policy::reference_internal);

    cls.def("add_private_compile_option",
        &SymGen::Target<Derived>::addPrivateCompileDefinition,
        py::arg("compile_option"),
        py::return_value_policy::reference_internal);

    cls.def("add_private_compile_options",
        [](Derived& self, const std::vector<std::string>& sources) -> Derived&
        {
            return self.addPrivateCompileDefinitions(std::span(sources));
        },
        py::arg("compile_options"),
        py::return_value_policy::reference_internal);
}

#endif // SYMGEN_PYTHON_BINDINGS_BIND_TARGET_METHODS_HPP
