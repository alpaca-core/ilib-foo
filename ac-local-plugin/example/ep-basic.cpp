// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/local/Model.hpp>
#include <ac/local/Instance.hpp>
#include <ac/local/ModelAssetDesc.hpp>
#include <ac/local/Lib.hpp>

#include <ac/jalog/Instance.hpp>
#include <ac/jalog/sinks/DefaultSink.hpp>

#include <iostream>

#include "ac-test-data-foo-models.h"
#include "aclp-foo-info.h"

int main() try {
    ac::jalog::Instance jl;
    jl.setup().add<ac::jalog::sinks::DefaultSink>();

    ac::local::Lib::loadPlugin(ACLP_foo_PLUGIN_FILE);

    auto model = ac::local::Lib::loadModel({
        .type = "foo",
        .assets = {
            {.path = AC_FOO_MODEL_LARGE, .tag = "x"}
        },
        .name = "foo-large"
    }, {});

    auto instance = model->createInstance("general", {});

    auto opResult = instance->runOp("run", {{"input", {"JFK", "said:"}}, {"splice", false}});

    std::cout << opResult << "\n";

    return 0;
}
catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
    return 1;
}
