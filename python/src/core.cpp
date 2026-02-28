#include <core.hpp>

PYBIND11_MODULE(symgen, m)
{
	m.doc() = "SymGen C++ bindings. SymGen is a little CMakeLists.txt generator.";

	// Order matters — dependencies before the classes that use them
	bind_language(m);
	bind_compiler(m);
	bind_on_off(m);
	bind_compile_options(m);
	bind_dependency(m);
	bind_package(m);
	bind_option(m);
	bind_executable(m);
	bind_library(m);
	bind_pybind11_module(m);
	bind_test(m);
	bind_sub_directory(m);
	bind_project(m);
}
