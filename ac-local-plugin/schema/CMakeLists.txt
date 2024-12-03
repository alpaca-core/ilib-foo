# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
add_library(ac-foo-schema INTERFACE)
add_library(ac::foo-schema ALIAS ac-foo-schema)
target_link_libraries(ac-foo-schema INTERFACE ac::schema)
target_include_directories(ac-foo-schema INTERFACE .)
