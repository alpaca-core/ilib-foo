// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include "VectorStore.hpp"
#include "Throw.hpp"

namespace ac::foo {

namespace {

// Compute Euclidean distance
float computeDistance(const VectorStore::EmbeddingVector& a, const VectorStore::EmbeddingVector& b) {
    if (a.size() != b.size()) {
        Throw{} << "VectorStore: Vectors must have the same dimensions. ";
    }

    float sum = 0.0f;
    for (size_t i = 0; i < a.size(); ++i) {
        float diff = a[i] - b[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

}

VectorStore::VectorStore(Params params)
    : m_params(std::move(params))
{
    m_data.reserve(m_params.maxItems);
}

void VectorStore::addItem(VectorStore::RecordData query, int64_t id) {
    if (m_data.size() >= m_params.maxItems) {
        Throw{} << "VectorStore: max items reached ";
    }

    m_data.push_back({id, query});
}

std::vector<std::pair<int64_t, VectorStore::Score>> VectorStore::findNearest(VectorStore::EmbeddingVector query, int32_t k) const{
    std::vector<std::pair<int64_t, VectorStore::Score>> result;

    if (k <= 0) {
        Throw{} << "Number of neighbors (k) must be positive.";
    }

    for (const auto& [id, record] : m_data) {
        float dist = computeDistance(query, record.vector);
        result.emplace_back(id, dist);
    }

    // Sort distances by ascending order
    std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    // Return top K results
    if (result.size() > static_cast<size_t>(k)) {
        result.resize(k);
    }
    return result;
}

VectorStore::RecordData VectorStore::getRecordById(int64_t id) const {
    for (auto& [storedId, record] : m_data) {
        if (storedId == id) {
            return record;
        }
    }

    Throw{} << "VectorStore: id not found";
}

} // namespace ac::foo
