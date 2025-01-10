// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/foo/Model.hpp>
#include <ac/foo/EmbeddingInstance.hpp>
#include <ac/foo/VectorStore.hpp>

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
    ac::foo::EmbeddingInstance instance(model, {});
    ac::foo::VectorStore vs({});

    const char* texts[] = {
        "dog",
        "cat",
        "tiger",
        "lion",
        "elephant",
        "giraffe",
        "rhinoceros",
        "hippopotamus",
    };

    for (uint32_t i = 0; i < sizeof(texts) /sizeof(const char*); ++i) {
        auto vec = instance.getEmbedding(texts[i]);
        vs.addItem({texts[i], vec}, i);
    }

    auto query = instance.getEmbedding("doggy");
    auto res = vs.findNearest(query, 2);

    for (const auto& [id, score] : res) {
        auto record = vs.getRecordById(id);
        std::cout << "id: " << id << ", score: " << score << ", text: " << record.text << std::endl;
    }

    std::cout << std::endl;

    return 0;
}
catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
}
