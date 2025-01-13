// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#pragma once
#include <ac/foo/VectorStore.hpp>

#include <ac/local/VectorStore.hpp>

#include <astl/move.hpp>
#include <astl/workarounds.h>
#include <itlib/throw_ex.hpp>
#include <stdexcept>

namespace ac::local {

using throw_ex = itlib::throw_ex<std::runtime_error>;

class FooVectorStore final : public VectorStore {
public:
    FooVectorStore();
    ~FooVectorStore() override {}

    virtual void addRecords(ac::Dict records) override;

    virtual void removeRecords(ac::Dict) override {}

    virtual ac::Dict get(ac::Dict param) override;

    virtual ac::Dict search(ac::Dict params) override;
private:
    foo::VectorStore m_store;
};

} // namespace ac::local
