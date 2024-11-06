# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
add_library(aclp-foo MODULE
    AcLocalFoo.cpp
)
if(NOT WIN32)
    target_compile_options(aclp-foo PRIVATE
        -fvisibility=hidden
        -fvisibility-inlines-hidden
    )
endif()

target_link_libraries(aclp-foo PRIVATE
    ac::foo
    ac::local
    ac::jalog
)

if(AC_FOO_BUILD_TESTS OR AC_FOO_BUILD_EXAMPLES)
    configure_file(ac-foo-plugin-dir.in.h ac-foo-plugin-dir.h @ONLY)
    add_library(ac-foo-plugin-dir INTERFACE)
    add_library(ac-dev::foo-plugin-dir ALIAS ac-foo-plugin-dir)
    target_include_directories(ac-foo-plugin-dir INTERFACE ${CMAKE_CURRENT_BINARY_DIR})
endif()
