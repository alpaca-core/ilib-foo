# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
add_ac_local_plugin(aclp-foo
    AcLocalFoo.cpp
)

target_link_libraries(aclp-foo PRIVATE
    ac::foo
)

configure_file(version.in.h version.h)
target_include_directories(aclp-foo PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

if(AC_FOO_BUILD_TESTS OR AC_FOO_BUILD_EXAMPLES)
    configure_file(ac-foo-plugin-dir.in.h ac-foo-plugin-dir.h @ONLY)
    add_library(ac-foo-plugin-dir INTERFACE)
    add_library(ac-dev::foo-plugin-dir ALIAS ac-foo-plugin-dir)
    target_include_directories(ac-foo-plugin-dir INTERFACE ${CMAKE_CURRENT_BINARY_DIR})
endif()

install(TARGETS aclp-foo
    EXPORT ac-foo-plugins
    COMPONENT plugins
    LIBRARY DESTINATION plugins
)
