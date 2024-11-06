// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/foo/Model.hpp>
#include <ac/foo/Instance.hpp>

// logging
#include <ac/jalog/Instance.hpp>
#include <ac/jalog/sinks/DefaultSink.hpp>

// models
#include "ac-test-data-foo-models.h"

#include <iostream>

int main() try {
    ac::jalog::Instance jl;
    jl.setup().add<ac::jalog::sinks::DefaultSink>();

    ac::foo::Model model(AC_FOO_MODEL_SMALL, {});
    ac::foo::Instance instance(model, {});

    auto s = instance.newSession({"soco", "bate", "soco", "vira"}, {.splice = false});

    for (const auto& token : s) {
        std::cout << token << ' ';
    }

    std::cout << std::endl;

    return 0;
}
catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
}
