########################################################################
####                       tests subdirectory                       ####
########################################################################

########################################################################
####                          Dependencies                          ####
########################################################################

find_package(GTest REQUIRED)

include(FetchContent)


########################################################################
####                            Targets                             ####
########################################################################

add_library(tests_include INTERFACE)

add_executable(SymGen_tests)

########################################################################
####                            Sources                             ####
########################################################################

target_sources(SymGen_tests PRIVATE
	src/test_concatenate_view.cpp)

target_include_directories(tests_include INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

########################################################################
####                              Link                              ####
########################################################################

target_link_libraries(SymGen_tests
	PRIVATE tests_include
	PRIVATE SymGen
	PRIVATE GTest::gtest_main)

########################################################################
####                             Tests                              ####
########################################################################

add_test(NAME test_Symgen COMMAND SymGen_tests)

