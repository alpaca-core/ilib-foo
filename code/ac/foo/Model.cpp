// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include "Model.hpp"
#include "Logging.hpp"
#include "Throw.hpp"
#include <itlib/utility.hpp>
#include <fstream>

namespace ac::foo {

static constexpr std::string_view SyntheticModel_Data[] = {
    "one", "two", "three", "four", "five", "once", "I", "caught", "a", "fish", "alive",
    "six", "seven", "eight", "nine", "ten", "then", "I", "let", "it", "go", "again"
};


Model::Model(Params params)
    : m_params(itlib::force_move(params))
{
    auto& path = m_params.path;

    if (path.empty()) {
        for (auto& item : SyntheticModel_Data) {
            addDataItem(std::string(item));
        }
    }
    else {
        std::ifstream file(path);
        if (!file.is_open()) {
            FOO_LOG(Error, "Failed to open file: ", path);
            Throw{} << "Failed to open file: " << path;
        }

        FOO_LOG(Info, "Loading model from ", path);

        while (!file.eof()) {
            std::string word;
            file >> word;
            if (word.empty()) {
                // ignore empty strings
                continue;
            }
            addDataItem(itlib::force_move(word));
        }
    }
}

Model::~Model() = default;

void Model::addDataItem(std::string item) {
    if (!m_params.splice.empty()) {
        m_data.push_back(m_params.splice);
    }
    m_data.push_back(itlib::force_move(item));
}

std::span<const std::string_view> Model::rawSyntheticModelData() noexcept {
    return SyntheticModel_Data;
}

} // namespace ac::foo
