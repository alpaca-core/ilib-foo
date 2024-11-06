// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#pragma once
#include <ac/jalog/Scope.hpp>
#include <ac/jalog/Log.hpp>

namespace ac::foo::log {
extern ac::jalog::Scope scope;
}

#define FOO_LOG(lvl, ...) AC_JALOG_SCOPE(::ac::foo::log::scope, lvl, __VA_ARGS__)
