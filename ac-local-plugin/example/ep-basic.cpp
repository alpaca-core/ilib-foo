// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/local/Lib.hpp>
#include <ac/local/IoCtx.hpp>
#include <ac/frameio/local/BlockingIo.hpp>
#include <ac/Frame.hpp>

#include <ac/jalog/Instance.hpp>
#include <ac/jalog/sinks/DefaultSink.hpp>

#include <iostream>

#include "ac-test-data-foo-models.h"
#include "aclp-foo-info.h"

int main() {
    ac::jalog::Instance jl;
    jl.setup().add<ac::jalog::sinks::DefaultSink>();

    ac::local::IoCtx io;

    ac::local::Lib::loadPlugin(ACLP_foo_PLUGIN_FILE);

    auto& fooProvider = ac::local::Lib::getProvider("foo");
    ac::frameio::BlockingIo foo{io.connect(fooProvider)};

    foo.poll(); // state info from plugin (foo)
    foo.push({"load_model", {{"file_path", AC_FOO_MODEL_LARGE}}});
    foo.poll(); // load_model response
    foo.poll(); // state info from plugin (model loaded)
    foo.push({"create_instance", {}});
    foo.poll(); // creat_instance response
    foo.poll(); // state info from plugin (instance created)
    foo.push({"run", {{"input", {"JFK", "said:"}}, {"splice", false}}});

    auto result = foo.poll(); // run response
    std::cout << result.value.data << "\n";

    foo.close();

    return 0;
}
