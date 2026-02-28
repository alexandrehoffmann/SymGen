import symgen
import sys
import os

compileOptions = [
	"-fopenmp-simd",
	"-Wundef",
	"-Wvarargs",
	"-Wall",
	"-Wextra",
	"-Winit-self",
	"-Wpedantic",
	"-Werror",
	"-Wconversion",
	"-Wuninitialized",
	"-Wmissing-declarations",
	"-Wsign-conversion",
	"-Wshadow",
	"-Wcast-align",
	"-Wnull-dereference",
	"-Wformat=2",
	"-flax-vector-conversions",
	"-pedantic",
	"-pedantic-errors",
	"-Wno-error=array-bounds",
	"-Wno-c99-extensions",
	"-Wdouble-promotion",
	"-Wswitch-enum",
	"-Wrange-loop-construct",
	"-Wnon-virtual-dtor",
	"-Wold-style-cast",
	"-Woverloaded-virtual",
	"-Wvexing-parse"]

librarySrcs = [
	"src/SymGen/Project.cpp",
	"src/SymGen/Package.cpp",
	"src/SymGen/SubDirectory.cpp",
	"src/SymGen/utils.cpp"]

bindingsSrcs = [
	"src/bind_compile_options.cpp",
	"src/bind_compiler.cpp",
	"src/bind_dependency.cpp",
	"src/bind_executable.cpp",
	"src/bind_language.cpp",
	"src/bind_library.cpp",
	"src/bind_on_off.cpp",
	"src/bind_option.cpp",
	"src/bind_package.cpp",
	"src/bind_project.cpp",
	"src/bind_pybind11_module.cpp",
	"src/bind_sub_directory.cpp",
	"src/bind_test.cpp",
	"src/core.cpp"]

	
project = symgen.Project("SymGen") \
	.add_language(symgen.Language.CXX) \
	.add_standard(symgen.Language.CXX, 20) \
	.set_description("A little CMakeLists.txt generator with python bindings.") \
	.set_cmake_prefix("SYMGEN") \
	.add_compile_options(symgen.Language.CXX, [symgen.Compiler.GNU, symgen.Compiler.CLANG], compileOptions) \
	.add_dependency(symgen.Package("fmt") \
		.set_version("12.0.0") \
		.set_git("https://github.com/fmtlib/fmt.git", "12.1.0")) \
	.add_dependency(symgen.Package("pybind11") \
		.set_git("https://github.com/pybind/pybind11.git", "2.12.0")) \
	.add_library(symgen.Library("SymGen") \
		.add_sources(librarySrcs) \
		.add_public_dependency("fmt"))

demos = symgen.SubDirectory("demos") \
	.add_executable(symgen.Executable("demo_cmake") \
		.add_source("src/demo_cmake.cpp") \
		.add_dependency("SymGen"))
		
bindings = symgen.SubDirectory("python") \
	.add_pybind11_module(symgen.Pybind11Module("symgen") \
		.add_sources(bindingsSrcs) \
		.add_dependency("SymGen"))
		
tests = symgen.SubDirectory("tests") \
	.add_dependency(symgen.Package("GTest")) \
	.add_executable(symgen.Executable("SymGen_tests") \
		.add_source("src/test_concatenate_view.cpp") \
		.add_dependency("SymGen") \
		.add_dependency("GTest::gtest_main"))\
	.add_test(symgen.Test("test_Symgen", "SymGen_tests"))
		
project.add_optional_sub_directory(demos, symgen.Option("SYMGEN_BUILD_DEMO", "Build demos for SymGen", symgen.OFF))
project.add_optional_sub_directory(tests, symgen.Option("SYMGEN_BUILD_TESTS", "Build tests for SymGen", symgen.OFF))
project.add_sub_directory(bindings)
	
project.to_cmake_lists("CMakeLists.txt") 

for subdir in project.get_sub_directories():
	if not os.path.exists(subdir.get_name()):
		os.makedirs(subdir.get_name())
		print("creating subdirectory " + subdir.get_name())
	subdir.to_cmake_lists(subdir.get_name() + "/CMakeLists.txt") 
