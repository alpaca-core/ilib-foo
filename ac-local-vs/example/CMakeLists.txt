# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#

function(add_example name)
    set(tgt example-ac-foo-vs-${name})
    add_executable(${tgt} ep-${name}.cpp)
    target_link_libraries(${tgt} PRIVATE
        ac::local
        ac::jalog
        ac-test-data::foo
        aclp::foo-info
        ac-foo-vs
    )
    set_target_properties(${tgt} PROPERTIES FOLDER example)
    add_dependencies(${tgt} aclp::foo)
endfunction()

add_example(vectorStore)
