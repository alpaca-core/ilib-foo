# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#

function(add_example name)
    set(tgt example-ac-foo-${name})
    add_executable(${tgt} e-${name}.cpp)
    target_link_libraries(${tgt} PRIVATE
        ac::foo
        ac::jalog
        ac-test-data::foo
    )
    set_target_properties(${tgt} PROPERTIES FOLDER example)
endfunction()

add_example(basic)
add_example(vectorStore)
# set(tgt example-ac-foo-basic)
# add_executable(${tgt} e-basic.cpp)
# target_link_libraries(${tgt} PRIVATE
#     ac::foo
#     ac::jalog
#     ac-test-data::foo
# )
# set_target_properties(${tgt} PROPERTIES FOLDER example)
