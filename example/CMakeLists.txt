# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
set(tgt example-ac-foo-basic)
add_executable(${tgt} e-basic.cpp)
target_link_libraries(${tgt} PRIVATE
    ac::foo
    ac::jalog
    ac-test-data::foo
)
set_target_properties(${tgt} PROPERTIES FOLDER example)
