# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
set(tgt example-ac-foo-plugin)
add_executable(${tgt} ep-basic.cpp)
target_link_libraries(${tgt} PRIVATE
    ac::local
    ac::jalog
    ac-test-data::foo
)
set_target_properties(${tgt} PROPERTIES FOLDER example)
