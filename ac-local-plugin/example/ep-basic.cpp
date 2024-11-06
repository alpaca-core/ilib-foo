// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/local/PluginLoader.hpp>
#include <ac/local/ModelFactory.hpp>
#include <ac/local/Model.hpp>
#include <ac/local/Instance.hpp>

#include <ac/jalog/Instance.hpp>
#include <ac/jalog/sinks/DefaultSink.hpp>

#include <iostream>

#include "ac-test-data-foo-models.h"

int main() try {
    ac::jalog::Instance jl;
    jl.setup().add<ac::jalog::sinks::DefaultSink>();

    ac::local::ModelFactory factory;

    auto pi = ac::local::PluginLoader::loadPlugin("C:/prj/alpaca-core/ilib-foo/out/build/debug/bin", "aclp-foo");
    pi.addLoadersToFactory(factory);

    auto model = factory.createModel({
        .inferenceType = "foo",
        .assets = {
            {.path = AC_FOO_MODEL_LARGE, .tag = "x"}
        }
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
