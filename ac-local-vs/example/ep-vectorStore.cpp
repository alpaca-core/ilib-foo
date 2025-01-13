// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/local/Model.hpp>
#include <ac/local/Instance.hpp>
#include <ac/local/ModelAssetDesc.hpp>
#include <ac/local/Lib.hpp>

#include <AcLocalFooVS.hpp>

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

    auto instance = model->createInstance("embedding", {});

    ac::local::FooVectorStore vs;
    std::string animals[] = {"dog", "cat", "tiger", "lion"};
    int i = 0;
    for (auto& animal : animals) {
        auto opResult = instance->runOp("run", {{"input", animal}});
        vs.addRecords({
                {"text", animal},
                {"vector", opResult["result"]},
                {"id", i++}
            });
    }

    auto doggyEmbedding = instance->runOp("run", {{"input", "doggy"}});
    auto queryResult = vs.search({{"query", doggyEmbedding["result"]}, {"k", 2}});

    std::cout << queryResult << "\n";

    auto res = ac::Dict_optValueAt(queryResult, "results", ac::Dict::array());
    for (auto& [i, el] : res.items()) {
        int id = ac::Dict_optValueAt(el, "id", -1);
        int score = ac::Dict_optValueAt(el, "score", -1);
        auto str = vs.get({{"id", ac::Dict_optValueAt(el, "id", -1)}});

        std::cout << "id: " << id << ", score: " << score << " text: "<< ac::Dict_optValueAt(str, "text", std::string()) << "\n";
    }

    return 0;
}
catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
    return 1;
}
