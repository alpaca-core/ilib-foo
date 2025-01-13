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
    std::vector<float> ret;
    ret.resize(m_params.vectorSize);

    for (int32_t i = 0; i < input.size(); ++i) {
        ret[i % m_params.vectorSize] = static_cast<float>(i);
    }

    return ret;
}
}
