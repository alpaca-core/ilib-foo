# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
add_library(ac-test-data-foo INTERFACE)
add_library(ac-test-data::foo ALIAS ac-test-data-foo)
configure_file(ac-test-data-foo-dir.in.h "${PROJECT_SOURCE_DIR}/gen/ac-test-data-foo-dir.h")
target_include_directories(ac-test-data-foo INTERFACE
    .
    "${PROJECT_SOURCE_DIR}/gen"
)
