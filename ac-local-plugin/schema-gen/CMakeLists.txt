# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
add_executable(ac-foo-schema-gen)
target_sources(ac-foo-schema-gen PRIVATE
    schema-gen.cpp
)
target_link_libraries(ac-foo-schema-gen PRIVATE
    ac::foo-schema
)
