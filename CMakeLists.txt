# Main CMake file for compiling the examples and tests.
#
# Copyright (c) 2013-2014 Sebastien Rombauts (sebastien.rombauts@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT) 

cmake_minimum_required(VERSION 2.6)
project(shared_ptr)

# Define useful variables to handle Compiler/IDE differences:
if (MSVC)
    # Flags for linking with multithread static C++ runtime
    set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
else (MSVC)
    if (CMAKE_COMPILER_IS_GNUCXX)
        # GCC flags
        add_definitions(-rdynamic -fstack-protector-all -Wall -Wextra -pedantic -Wformat-security -Winit-self -Wswitch-default -Wswitch-enum -Wfloat-equal -Wshadow -Wcast-qual -Wconversion -Wlogical-op -Winline)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        # Clang flags
        add_definitions(-fstack-protector-all -Wall -Wextra -pedantic -Wformat-security -Winit-self -Wswitch-default -Wswitch-enum -Wfloat-equal -Wshadow -Wcast-qual -Wconversion -Winline)
    endif (CMAKE_COMPILER_IS_GNUCXX)
endif (MSVC)


## Core source code ##

# adding a new file require explicittly modifing the CMakeLists.txt
# so that CMake knows that it should rebuild the project (it is best practice)

# list of header files
set(SHARED_PTR_INC
 ${PROJECT_SOURCE_DIR}/include/shared_ptr.hpp
 ${PROJECT_SOURCE_DIR}/include/unique_ptr.hpp
)
source_group(inc FILES ${SHARED_PTR_INC})

# list of test files
set(SHARED_PTR_TESTS
 tests/shared_ptr_test.cpp
 tests/unique_ptr_test.cpp
)
source_group(tests FILES ${SHARED_PTR_TESTS})

# list of example files
set(SHARED_PTR_EXAMPLES
 examples/main.cpp
)
source_group(examples FILES ${SHARED_PTR_EXAMPLES})

# list of doc files of the library
set(SHARED_PTR_DOC
 README.md
 LICENSE.txt
)
source_group(doc FILES ${SHARED_PTR_DOC})

# All includes are relative to the "include" directory 
include_directories("${PROJECT_SOURCE_DIR}/include")


# Optional additional targets:

option(SHARED_PTR_BUILD_EXAMPLES "Build examples." ON)
if (SHARED_PTR_BUILD_EXAMPLES)
    # add the basic example executable
    add_executable(shared_ptr_example1 ${SHARED_PTR_EXAMPLES} ${SHARED_PTR_INC})
else(SHARED_PTR_BUILD_EXAMPLES)
    message(STATUS "SHARED_PTR_BUILD_EXAMPLES OFF")
endif(SHARED_PTR_BUILD_EXAMPLES)

option(SHARED_PTR_BUILD_TESTS "Build and run tests." ON)
if (SHARED_PTR_BUILD_TESTS)
    # add the subdirectory containing the CMakeLists.txt for the gtest library
    if (NOT MSVC)
        add_definitions(-Wno-variadic-macros -Wno-long-long -Wno-conversion -Wno-switch-enum)
    endif (NOT MSVC)

    add_subdirectory(googletest) 
    include_directories("${PROJECT_SOURCE_DIR}/googletest/include")

    # add the unit test executable
    add_executable(shared_ptr_tests ${SHARED_PTR_TESTS} ${SHARED_PTR_INC})
    target_link_libraries(shared_ptr_tests gtest_main)

    # add a "test" target:
    enable_testing()

    # does the tests pass?
    add_test(UnitTests shared_ptr_tests)

    if (SHARED_PTR_BUILD_EXAMPLES)
        # does the example1 runs successfully?
        add_test(Example1Run shared_ptr_example1)
    endif(SHARED_PTR_BUILD_EXAMPLES)
else (SHARED_PTR_BUILD_TESTS)
    message(STATUS "SHARED_PTR_BUILD_TESTS OFF")
endif (SHARED_PTR_BUILD_TESTS)
