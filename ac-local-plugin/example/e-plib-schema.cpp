// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/local/Lib.hpp>
#include <ac/frameio/local/LocalIoRunner.hpp>
#include <ac/schema/BlockingIoHelper.hpp>
#include <ac/schema/FrameHelpers.hpp>

#include <ac/local/PluginPlibUtil.inl>

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

    ac::frameio::LocalIoRunner io;

    auto fooHandler = ac::local::Lib::createSessionHandler("foo");
    ac::schema::BlockingIoHelper foo(io.connect(std::move(fooHandler)));

    namespace schema = ac::schema::foo;

    foo.expectState<schema::StateInitial>();
    foo.call<schema::StateInitial::OpLoadModel>({});

    foo.expectState<schema::StateModelLoaded>();
    foo.call<schema::StateModelLoaded::OpCreateInstance>({.cutoff = 2});

    foo.expectState<schema::StateInstance>();
    auto result = foo.call<schema::StateInstance::OpRun>({
        .input = std::vector<std::string>{"a", "b", "c"}
    });
    std::cout << result.result.value() << std::endl;

    return 0;
}
catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
    return 1;
}
