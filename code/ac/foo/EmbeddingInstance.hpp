// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include <vector>
#include <string>

namespace ac::foo {
class Model;

class AC_FOO_API EmbeddingInstance {
public:
    struct InitParams {
        int cutoff = -1; // cut off model data to n-th element (or don't cut if -1)
    };

    EmbeddingInstance(Model& model, InitParams params);
    ~EmbeddingInstance() = default;

    std::vector<float> getEmbedding(std::string_view input);

    const Model& model() const noexcept { return m_model; }
private:
    const InitParams m_params;
    Model& m_model;
};

} // namespace ac::foo
