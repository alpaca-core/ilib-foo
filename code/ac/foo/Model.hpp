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

class AC_FOO_API Model {
public:
    struct Params {
        std::string path;   // path to file with data (or empty string for synthetic model)
        std::string splice; // splice string before each data element
    };

    explicit Model(Params params); // synthetic model
    ~Model();

    const std::vector<std::string>& data() const noexcept { return m_data; }

    static std::span<const std::string_view> rawSyntheticModelData() noexcept;
private:
    void addDataItem(std::string item);

    const Params m_params;
    std::vector<std::string> m_data;
};

} // namespace ac::foo
