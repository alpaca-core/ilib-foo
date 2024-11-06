// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#pragma once
#include <splat/symbol_export.h>

#if AC_FOO_SHARED
#   if BUILDING_AC_FOO
#       define AC_FOO_API SYMBOL_EXPORT
#   else
#       define AC_FOO_API SYMBOL_IMPORT
#   endif
#else
#   define AC_FOO_API
#endif
