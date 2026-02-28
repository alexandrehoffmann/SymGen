#include <bind_compiler.hpp>

#include <SymGen/Compiler.hpp>

void bind_compiler(pybind11::module_& m)
{
	namespace py = pybind11;

	py::enum_<SymGen::Compiler>(m, "Compiler")
		.value("GNU",   SymGen::Compiler::GNU)
		.value("CLANG", SymGen::Compiler::CLANG)
		.value("MSV",   SymGen::Compiler::MSV)
		.def("__repr__",
			[](const SymGen::Compiler compiler)
			{
				return fmt::format("{}", compiler);
			});
}
