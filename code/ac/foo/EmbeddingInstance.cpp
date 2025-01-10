// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include "EmbeddingInstance.hpp"
#include "api.h"
#include <itlib/generator.hpp>

namespace ac::foo {
EmbeddingInstance::EmbeddingInstance(Model& model, InitParams params)
    : m_params(params)
    , m_model(model)
{}

std::vector<float> EmbeddingInstance::getEmbedding(std::string_view input) {
    const uint32_t vectorSize = 64;
    std::vector<float> ret;
    ret.resize(vectorSize);

    for (int32_t i = 0; i < input.size(); ++i) {
        ret[i % vectorSize] = static_cast<float>(i);
    }

    return ret;
}
}
