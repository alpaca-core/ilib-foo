# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(ilib-foo
    VERSION 0.1.5
    DESCRIPTION "Example plugin for the Alpaca Core Local SDK"
    LANGUAGES CXX
)

include(./get-ac-build.cmake)

#################
# cmake lib
CPMAddPackage(gh:iboB/icm@1.5.4)
list(APPEND CMAKE_MODULE_PATH
    "${icm_SOURCE_DIR}"
)

include(icm_add_lib)

include(init_ac_prj)

include(ac_build_prj_util)
include(ac_build_plugin_util)

#################
# options

option(AC_FOO_BUILD_TESTS "${PROJECT_NAME}: build tests" ${testsDefault})
option(AC_FOO_BUILD_EXAMPLES "${PROJECT_NAME}: build examples" ${examplesDefault})
mark_as_advanced(AC_FOO_BUILD_TESTS AC_FOO_BUILD_EXAMPLES)

init_ac_plugin_option(FOO)

#######################################
# packages

add_ac_local(0.1.20)
CPMAddPackage(gh:iboB/splat@1.3.3)
CPMAddPackage(gh:iboB/itlib@1.11.5)

#######################################
# subdirs
add_subdirectory(code)

if(AC_FOO_BUILD_TESTS OR AC_FOO_BUILD_EXAMPLES)
    CPMAddPackage(gh:alpaca-core/test-data-foo@1.0.0)
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

if(BUILD_AC_FOO_PLUGIN)
    add_subdirectory(ac-local-plugin)
endif()
