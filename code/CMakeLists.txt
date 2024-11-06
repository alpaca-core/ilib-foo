# Copyright (c) Alpaca Core
# SPDX-License-Identifier: MIT
#
icm_add_lib(ac-foo AC_FOO)
add_library(ac::foo ALIAS ac-foo)
target_link_libraries(ac-foo
    PRIVATE
        ac::jalog
    PUBLIC
        splat::splat
        itlib::itlib
)
target_sources(ac-foo
    INTERFACE FILE_SET HEADERS FILES
        ac/foo/api.h
        ac/foo/Model.hpp
        ac/foo/Instance.hpp
    PRIVATE
        ac/foo/Logging.hpp
        ac/foo/Logging.cpp
        ac/foo/Model.cpp
        ac/foo/Instance.cpp
)
