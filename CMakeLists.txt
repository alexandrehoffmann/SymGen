cmake_minimum_required(VERSION 3.24)

cmake_policy(SET CMP0135 NEW)

########################################################################
####                        Project metadata                        ####
########################################################################

project(SymGen
	VERSION 1.0.0
	DESCRIPTION "A little CMakeLists.txt generator with python bindings."
	LANGUAGES CXX)

########################################################################
####                           Variables                            ####
########################################################################

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build.")
endif()

set(SYMGEN_SAN_FLAGS -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer)

set(SYMGEN_COMMON_COMPILE_OPTIONS
	-fopenmp-simd
	-Wundef
	-Wvarargs
	-Wall
	-Wextra
	-Winit-self
	-Wpedantic
	-Werror
	-Wconversion
	-Wuninitialized
	-Wmissing-declarations
	-Wsign-conversion
	-Wshadow
	-Wcast-align
	-Wnull-dereference
	-Wformat=2
	-flax-vector-conversions
	-pedantic
	-pedantic-errors
	-Wno-error=array-bounds
	-Wno-c99-extensions
	-Wdouble-promotion
	-Wswitch-enum
	-Wrange-loop-construct
	-Wnon-virtual-dtor
	-Wold-style-cast
	-Woverloaded-virtual
	-Wvexing-parse
	-fopenmp-simd
	-Wundef
	-Wvarargs
	-Wall
	-Wextra
	-Winit-self
	-Wpedantic
	-Werror
	-Wconversion
	-Wuninitialized
	-Wmissing-declarations
	-Wsign-conversion
	-Wshadow
	-Wcast-align
	-Wnull-dereference
	-Wformat=2
	-flax-vector-conversions
	-pedantic
	-pedantic-errors
	-Wno-error=array-bounds
	-Wno-c99-extensions
	-Wdouble-promotion
	-Wswitch-enum
	-Wrange-loop-construct
	-Wnon-virtual-dtor
	-Wold-style-cast
	-Woverloaded-virtual
	-Wvexing-parse)

set(SYMGEN_GNU_CXX_COMPILE_OPTIONS
	${SYMGEN_COMMON_COMPILE_OPTIONS})

set(SYMGEN_CLANG_CXX_COMPILE_OPTIONS
	${SYMGEN_COMMON_COMPILE_OPTIONS})

########################################################################
####                            Options                             ####
########################################################################

option(SYMGEN_ENABLE_SANITIZERS "Enable sanitizers in Debug builds" OFF)
option(SYMGEN_BUILD_DOC "Build Doxygen documentation" OFF)
option(SYMGEN_BUILD_DEMO "Build demos for SymGen" OFF)
option(SYMGEN_BUILD_TESTS "Build tests for SymGen" OFF)

########################################################################
####                          Dependencies                          ####
########################################################################


include(FetchContent)
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 12.1.0 FIND_PACKAGE_ARGS NAMES fmt VERSION 12.0.0)
FetchContent_Declare(pybind11 GIT_REPOSITORY https://github.com/pybind/pybind11.git GIT_TAG 2.12.0 FIND_PACKAGE_ARGS NAMES pybind11)

FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(pybind11)

########################################################################
####                         SubDirectories                         ####
########################################################################

add_subdirectory(python)

if(${SYMGEN_BUILD_DEMO})
	add_subdirectory(demos)
endif()

if(${SYMGEN_BUILD_TESTS})
	add_subdirectory(tests)
endif()

########################################################################
####                            Targets                             ####
########################################################################

add_library(SymGen_include INTERFACE)

add_library(SymGen_options INTERFACE)

add_library(SymGen)

########################################################################
####                            Sources                             ####
########################################################################

target_sources(SymGen PRIVATE
	src/SymGen/Project.cpp
	src/SymGen/Package.cpp
	src/SymGen/SubDirectory.cpp
	src/SymGen/utils.cpp)

target_include_directories(SymGen_include INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

########################################################################
####                              Link                              ####
########################################################################

target_link_libraries(SymGen
	PUBLIC SymGen_include
	PUBLIC SymGen_options
	PUBLIC fmt)

########################################################################
####                        Compile options                         ####
########################################################################

target_compile_features(SymGen_options INTERFACE cxx_std_20)

target_compile_options(SymGen_options INTERFACE
	$<$<CXX_COMPILER_ID:GNU>:${SYMGEN_GNU_CXX_COMPILE_OPTIONS}>
	$<$<CXX_COMPILER_ID:CLANG>:${SYMGEN_CLANG_CXX_COMPILE_OPTIONS}>)

if(SYMGEN_ENABLE_SANITIZERS)
	target_compile_options(SymGen_options INTERFACE $<$<CONFIG:Debug>:${SAN_FLAGS}>)
endif()

if(SYMGEN_USE_NATIVE_OPT)
	target_compile_options(SymGen_options INTERFACE -march=native -mtune=native)
endif()

########################################################################
####                             Tests                              ####
########################################################################

if(${SYMGEN_BUILD_TESTS})
	enable_testing()
endif()
