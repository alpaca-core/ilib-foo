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
