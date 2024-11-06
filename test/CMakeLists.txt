# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
macro(foo_test test)
    add_doctest_lib_test(${test} ac-foo t-${test}.cpp LIBRARIES ac-test-data::foo)
endmacro()

foo_test(Model)
foo_test(Instance)
