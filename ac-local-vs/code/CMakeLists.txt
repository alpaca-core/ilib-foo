# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
add_library(ac-foo-vs SHARED AcLocalFooVS.hpp AcLocalFooVS.cpp)
add_library(ac::foo-vs ALIAS ac-foo-vs)
target_link_libraries(ac-foo-vs
    PUBLIC
        ac::foo
        ac::local
        splat::splat
        itlib::itlib
        ac::jalog
)

target_sources(ac-foo-vs
    INTERFACE FILE_SET HEADERS FILES
        AcLocalFooVS.hpp
    PRIVATE
        AcLocalFooVS.cpp
)
