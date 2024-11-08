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

include(icm_add_lib)

#################
# ac-cfg
set(acLocalVersion 0.1.0)

option(AC_STANDALONE "project is standalone" ON)
option(AC_MONO_ROOT "project is a monorepo root" OFF)
option(AC_MONO_COMP "project is a monorepo component" OFF)
option(AC_DEPLOY_ROOT "project is a deploy root" OFF)
option(AC_DEPLOY_COMP "project is a deploy component" OFF)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(projectIsSubdir NO)
    set(projectIsRoot YES)
    set(projectIsStandalone ${AC_STANDALONE})
    set(projectIsMonoRoot ${AC_MONO_ROOT})
    set(projectIsMonoComponent ${AC_MONO_COMP})
    set(projectIsDeployRoot ${AC_DEPLOY_ROOT})
    set(projectIsDeployComponent ${AC_DEPLOY_COMP})
    include(./ac_root_setup.cmake)
else()
    set(projectIsSubdir YES)
    set(projectIsRoot NO)
    set(projectIsStandalone NO)
    set(projectIsMonoRoot NO)
    set(projectIsMonoComponent NO)
    set(projectIsDeployRoot NO)
    set(projectIsDeployComponent NO)
endif()

if(NOT TARGET ac::local)
    if(projectIsSubdir OR projectIsStandalone OR projectIsDeployRoot)
        CPMAddPackage(
            NAME ac-local
            VERSION ${acLocalVersion}
            SYSTEM FALSE # not system, so that it's installable
            GITHUB_REPOSITORY alpaca-core/ac-local
            OPTIONS
                "AC_LOCAL_BUILD_TOOLS=${projectIsDeployRoot}"
                "AC_LOCAL_BUILD_TESTS=${projectIsDeployRoot}"
        )
        set(ac-local_ROOT "${ac-local_SOURCE_DIR}")
    elseif(projectIsMonoRoot)
        add_subdirectory(../ac-local ac-local)
        set(ac-local_ROOT "${CMAKE_CURRENT_SOURCE_DIR}../ac-local")
    elseif(projectIsMonoComponent OR projectIsDeployComponent)
        find_package(ac-local ${acLocalVersion} REQUIRED)
    endif()
endif()

if(NOT ac-local_VERSION VERSION_EQUAL acLocalVersion)
    message(FATAL_ERROR "ac-local version mismatch. Expected ${acLocalVersion}. Got ${ac-local_VERSION}")
endif()

#################
# options
if(projectIsRoot OR AC_MONO_ROOT)
    set(testsDefault ON)
    set(toolsDefault ON)
    if(projectIsDeployRoot OR projectIsDeployComponent)
        set(examplesDefault OFF)
    else()
        set(examplesDefault ON)
    endif()
    set(pluginDefault ON)
else()
    set(testsDefault OFF)
    set(toolsDefault OFF)
    set(examplesDefault OFF)
    set(pluginDefault OFF)
endif()


option(AC_FOO_BUILD_TESTS "${CMAKE_PROJECT_NAME}: build tests" ${testsDefault})
option(AC_FOO_BUILD_EXAMPLES "${CMAKE_PROJECT_NAME}: build examples" ${examplesDefault})
mark_as_advanced(AC_FOO_BUILD_TESTS AC_FOO_BUILD_EXAMPLES)

option(AC_FOO_BUILD_PLUGIN "${CMAKE_PROJECT_NAME}: build AC Local plugin" ${pluginDefault})

if(BUILD_SHARED_LIBS AND AC_FOO_BUILD_PLUGIN)
    message(AUTHOR_WARNING
        "${CMAKE_PROJECT_NAME}: Building AC Local plugins with BUILD_SHARED_LIBS ON is dangerous! "
        "https://github.com/alpaca-core/ac-local/blob/master/doc/dev/plugin%2Bshared-lib.md"
    )
endif()

if(NOT BUILD_SHARED_LIBS)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()


#######################################
# packages
CPMAddPackage(gh:iboB/splat@1.3.3)
CPMAddPackage(gh:iboB/itlib@1.11.4)

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

file(GENERATE
    OUTPUT foo-plugins.json
    INPUT foo-plugins.in.json
)
