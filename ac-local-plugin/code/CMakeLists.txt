# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
add_ac_plugin(
    NAME foo
    SOURCES
        AcLocalFoo.cpp
    LIBRARIES
        ac::foo
)
