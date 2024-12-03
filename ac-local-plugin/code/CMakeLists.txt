# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
add_ac_local_plugin(
    NAME foo
    SOURCES
        AcLocalFoo.cpp
    LIBRARIES
        ac::foo
        ac::foo-schema
)
