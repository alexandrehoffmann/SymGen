########################################################################
####                      python subdirectory                       ####
########################################################################

########################################################################
####                          Dependencies                          ####
########################################################################

########################################################################
####                            Targets                             ####
########################################################################

add_library(python_include INTERFACE)

pybind11_add_module(symgen MODULE)

########################################################################
####                            Sources                             ####
########################################################################

target_sources(symgen PRIVATE
	src/bind_compile_options.cpp
	src/bind_compiler.cpp
	src/bind_dependency.cpp
	src/bind_executable.cpp
	src/bind_language.cpp
	src/bind_library.cpp
	src/bind_on_off.cpp
	src/bind_option.cpp
	src/bind_package.cpp
	src/bind_project.cpp
	src/bind_pybind11_module.cpp
	src/bind_sub_directory.cpp
	src/bind_test.cpp
	src/core.cpp)

target_include_directories(python_include INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

########################################################################
####                              Link                              ####
########################################################################

target_link_libraries(symgen
	PRIVATE python_include
	PRIVATE SymGen)

