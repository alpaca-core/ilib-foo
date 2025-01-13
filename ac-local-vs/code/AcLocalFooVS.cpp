// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//

#include "AcLocalFooVS.hpp"

namespace ac::local {

FooVectorStore::FooVectorStore()
    : m_store({})
{}

void FooVectorStore::addRecords(ac::Dict record) {
    // for (auto& [id, record] : records.items()) {
        m_store.addItem({
            .text = Dict_optValueAt(record, "text", std::string{}),
            .vector = Dict_optValueAt(record, "vector", std::vector<float>{})
        }, Dict_optValueAt(record, "id", 0));
    // }
}

ac::Dict FooVectorStore::get(ac::Dict param) {
    auto res = m_store.getRecordById(Dict_optValueAt(param, "id", 0));

    return {
            {"text", res.text},
            {"vector", res.vector}
        };
}

ac::Dict FooVectorStore::search(ac::Dict params) {
    auto query = Dict_optValueAt(params, "query", std::vector<float>{});
    auto k = Dict_optValueAt(params, "k", -1);
    if (k <= 0) {
        throw_ex{} << "k should be positive" << k;
    }

    auto nearest = m_store.findNearest(query, k);

    auto resArray = ac::Dict::array();
    for (size_t i = 0; i < nearest.size(); i++)
    {
        resArray.push_back({
            {"id", nearest[i].first},
            {"score", nearest[i].second}
        });
    }

    ac::Dict res = {{"results", resArray}};
    return res;
}

} // namespace ac::local
