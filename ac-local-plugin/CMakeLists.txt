# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
include(../../ac-local/out/build/debug/ac-local-targets.cmake)
add_custom_target(copy_dlls
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ac-local/out/build/debug/bin/ac-local-d.dll
        ${CMAKE_CURRENT_SOURCE_DIR}/../../ac-local/out/build/debug/bin/ac-jalog-d.dll
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_subdirectory(code)

if(AC_FOO_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
