#include <SymGen/Project.hpp>

int main()
{	
	std::vector<std::string> gnuClangCxxOptions{
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
		"-Wvexing-parse"};
	
	std::vector<std::string> librarySources{
		"src/SymGen/Project.cpp",
		"src/SymGen/Package.cpp",
		"src/SymGen/SubDirectory.cpp",
		"src/SymGen/utils.cpp"};
	
	std::vector<std::string> bindingSources{
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
		"src/bind_subdirectory.cpp",
		"src/bind_test.cpp",
		"src/core.cpp"};
	
	SymGen::Project project("SymGen");
	SymGen::SubDirectory demos("demos");
	SymGen::SubDirectory bindings("python");
	SymGen::SubDirectory tests("tests");
		
	project
		.addLanguage(SymGen::Language::CXX)
		.addStandard(SymGen::Language::CXX, 20)
		.setDescription("A little CMakeLists.txt generator with python bindings.")
		.setCmakePrefix("SYMGEN")
		//
		.addCompileOptions(SymGen::Language::CXX, {SymGen::Compiler::GNU, SymGen::Compiler::CLANG}, gnuClangCxxOptions)
		//
		.addDependency(SymGen::Package("fmt")
			.setVersion("12.0.0")
			.setGit("https://github.com/fmtlib/fmt.git", "12.1.0"))
		.addDependency(SymGen::Package("pybind11")
			.setGit("https://github.com/pybind/pybind11.git", "2.12.0"))
		.addLibrary(SymGen::Library("SymGen")
			.addSources(librarySources)
			.addPublicDependency("fmt"));
	
	
	demos
		.addExecutable(SymGen::Executable("demo_cmake")
			.addSource("src/demo_cmake.cpp")
			.addDependency("SymGen"));
			
	
	bindings
		.addPybind11Module(SymGen::Pybind11Module("symgen")
			.addSources(bindingSources)
			.addDependency("SymGen"));
			
	tests
		.addDependency(SymGen::Package("GTest"))
		.addExecutable(SymGen::Executable("SymGen_tests")
			.addSource("src/test_concatenate_view.cpp")
			.addDependency("SymGen")
			.addDependency("GTest::gtest_main"))
		.addTest(SymGen::Test("test_Symgen", "SymGen_tests"));
		
	project.addOptionalSubDirectory(demos, SymGen::Option("SYMGEN_BUILD_DEMO", "Build demos for SymGen", SymGen::OFF));
	project.addOptionalSubDirectory(tests, SymGen::Option("SYMGEN_BUILD_TESTS", "Build tests for SymGen", SymGen::OFF));
	project.addSubDirectory(bindings);
	
	project.toCMakeLists(stdout);
	
	for (SymGen::SubDirectory& subdirectory : project.getSubDirectories())
	{
		subdirectory.toCMakeLists(stdout);
	}
	
	return EXIT_SUCCESS;
}
