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
# options

#######################################
# packages
CPMAddPackage(gh:iboB/splat@1.3.3)
CPMAddPackage(gh:iboB/itlib@1.11.4)
CPMAddPackage(gh:iboB/jalog@0.4.5)

#######################################
# subdirs
add_subdirectory(code)

add_subdirectory(test-models)

CPMAddPackage(gh:iboB/doctest-util@0.1.2)
set_target_properties(doctest PROPERTIES FOLDER test)
set_target_properties(doctest-main PROPERTIES FOLDER test)
enable_testing()
add_subdirectory(test)

add_subdirectory(example)

add_subdirectory(ac-local-plugin)
