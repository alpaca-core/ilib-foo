# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(ilib-foo
    VERSION 1.0.0
    DESCRIPTION "Example plugin for the Alpaca Core SDK"
    LANGUAGES CXX
)

#################
# cpm
if(NOT CPM_SOURCE_CACHE AND NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_SOURCE_CACHE "${CMAKE_CURRENT_SOURCE_DIR}/.cpm")
    message(STATUS "Setting cpm cache dir to: ${CPM_SOURCE_CACHE}")
endif()
include(./get_cpm.cmake)

#################
# cmake lib
CPMAddPackage(gh:iboB/icm@1.5.4)
list(APPEND CMAKE_MODULE_PATH
    "${icm_SOURCE_DIR}"
)

include(icm_dev_mode)
include(icm_add_lib)

#################
# other config
if(ICM_DEV_MODE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # modules to bin
endif()

#################
# options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

option(AC_FOO_BUILD_TESTS "${CMAKE_PROJECT_NAME}: build tests" ${ICM_DEV_MODE})
option(AC_FOO_BUILD_EXAMPLES "${CMAKE_PROJECT_NAME}: build examples" ${ICM_DEV_MODE})
mark_as_advanced(AC_FOO_BUILD_TESTS AC_FOO_BUILD_EXAMPLES)

option(AC_FOO_BUILD_PLUGIN "${CMAKE_PROJECT_NAME}: build AC Local plugin" ${ICM_DEV_MODE})

if(BUILD_SHARED_LIBS AND AC_FOO_BUILD_PLUGIN)
    message(AUTHOR_WARNING
        "Building AC Local plugins with BUILD_SHARED_LIBS ON is dangerous! "
        "https://github.com/alpaca-core/ac-local/blob/master/doc/dev/plugin%2Bshared-lib.md"
    )
endif()

#######################################
# packages
CPMAddPackage(gh:iboB/splat@1.3.3)
CPMAddPackage(gh:iboB/itlib@1.11.4)

include(../ac-local/out/build/debug/ac-local-targets.cmake)
if(WIN32)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_CURRENT_SOURCE_DIR}/../ac-local/out/build/debug/bin/ac-local.dll
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ac-local.dll
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_CURRENT_SOURCE_DIR}/../ac-local/out/build/debug/bin/ac-jalog.dll
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ac-jalog.dll
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

#######################################
# subdirs
add_subdirectory(code)

if(AC_FOO_BUILD_TESTS OR AC_FOO_BUILD_EXAMPLES)
    add_subdirectory(test-models)
endif()

if(AC_FOO_BUILD_TESTS)
    CPMAddPackage(gh:iboB/doctest-util@0.1.2)
    set_target_properties(doctest PROPERTIES FOLDER test)
    set_target_properties(doctest-main PROPERTIES FOLDER test)
    enable_testing()
    add_subdirectory(test)
endif()

if(AC_FOO_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(AC_FOO_BUILD_PLUGIN)
    add_subdirectory(ac-local-plugin)
endif()
