// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#pragma once
#include <jalog/Scope.hpp>
#include <jalog/Log.hpp>

namespace ac::foo::log {
extern jalog::Scope scope;
}

#define FOO_LOG(lvl, ...) JALOG_SCOPE(::ac::foo::log::scope, lvl, __VA_ARGS__)
