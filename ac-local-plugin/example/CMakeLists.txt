# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
set(tgt example-ac-foo-plugin)
add_executable(${tgt} ep-basic.cpp)
target_link_libraries(${tgt} PRIVATE
    ac::local
    ac::jalog
    ac-test-data::foo
    aclp::foo-info
)
set_target_properties(${tgt} PROPERTIES FOLDER example)
add_dependencies(${tgt} aclp::foo)

set(tgt example-ac-foo-plib-schema)
add_executable(${tgt} e-plib-schema.cpp)
target_link_libraries(${tgt} PRIVATE
    ac::local
    ac::jalog
    ac-test-data::foo
    aclp::foo-plib
    ac::foo-schema
)
set_target_properties(${tgt} PROPERTIES FOLDER example)
