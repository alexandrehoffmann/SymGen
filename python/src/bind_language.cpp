#include <bind_language.hpp>

#include <SymGen/Language.hpp>

void bind_language(pybind11::module_& m)
{
	namespace py = pybind11;

	py::enum_<SymGen::Language>(m, "Language")
		.value("C",   SymGen::Language::C)
		.value("CXX", SymGen::Language::CXX)
		.def("__repr__",
			[](const SymGen::Language language)
			{
				return fmt::format("{}", language);
			});
}
