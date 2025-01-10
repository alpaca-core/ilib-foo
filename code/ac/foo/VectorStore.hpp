// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include <string>
#include <vector>
#include <span>
#include <string_view>

namespace ac::foo {

class AC_FOO_API VectorStore {
public:
    struct Params {
        uint64_t maxItems = 100;
    };

    VectorStore(Params params);
    ~VectorStore() = default;

    using EmbeddingVector = std::vector<float>;
    using Score = float;

    struct RecordData {
        std::string text;
        EmbeddingVector vector;
    };

    void addItem(RecordData query, int64_t id);
    std::vector<std::pair<int64_t, Score>> findNearest(EmbeddingVector query, int32_t k) const;

    RecordData getRecordById(int64_t id) const;
private:
    using EmbeddingVectorList = std::vector<std::pair<int64_t, RecordData>>;

    const Params m_params;
    EmbeddingVectorList m_data;
};

} // namespace ac::foo
