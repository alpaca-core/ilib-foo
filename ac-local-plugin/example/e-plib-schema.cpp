// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/local/Model.hpp>
#include <ac/local/Instance.hpp>
#include <ac/local/ModelAssetDesc.hpp>
#include <ac/local/Lib.hpp>

#include <ac/local/schema/CallHelpers.hpp>
#include <ac/schema/Foo.hpp>

#include <ac/jalog/Instance.hpp>
#include <ac/jalog/sinks/DefaultSink.hpp>

#include <iostream>

#include "aclp-foo-plib.h"
#include "ac-test-data-foo-models.h"

int main() try {
    ac::jalog::Instance jl;
    jl.setup().add<ac::jalog::sinks::DefaultSink>();

    add_foo_to_ac_local_global_registry();

    auto model = ac::local::Lib::loadModel({
        .type = "foo",
        .assets = {
            {.path = AC_FOO_MODEL_LARGE, .tag = "x"}
        },
        .name = "foo-large"
    }, {});

    using Instance = ac::schema::FooProvider::InstanceGeneral;
    auto instance = Model_createInstance<Instance>(*model, {});

    using Interface = ac::schema::FooInterface;
    auto opResult = Instance_runOp<Interface::OpRun>(*instance, {
        .input = std::vector<std::string>{"JFK", "said:"},
        .splice = false
    });

    std::cout << opResult.result.value() << "\n";

    return 0;
}
catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
    return 1;
}
