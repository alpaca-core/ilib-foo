// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/foo/Model.hpp>
#include "ac-test-data-foo-models.h"
#include <doctest/doctest.h>
#include <algorithm>

TEST_CASE("simple") {
    ac::foo::Model model({.path = AC_FOO_MODEL_LARGE});
    auto& data = model.data();
    CHECK(data.size() == 25);
    CHECK(data[0] == "We");
    CHECK(data[1] == "choose");
    CHECK(data[2] == "to");
    CHECK(data[3] == "go");
    CHECK(data[4] == "to");

    CHECK(data[23] == "are");
    CHECK(data[24] == "hard");
}

TEST_CASE("splice") {
    ac::foo::Model model({.path = AC_FOO_MODEL_SMALL, .splice = "Soco"});
    auto& data = model.data();
    CHECK(data.size() == 6);
    CHECK(data[0] == "Soco");
    CHECK(data[1] == "soco");
    CHECK(data[2] == "Soco");
    CHECK(data[3] == "bate");
    CHECK(data[4] == "Soco");
    CHECK(data[5] == "vira");
}

TEST_CASE("synthetic") {
    ac::foo::Model model({});
    auto& data = model.data();

    auto rawData = ac::foo::Model::rawSyntheticModelData();
    CHECK(std::equal(data.begin(), data.end(), rawData.begin(), rawData.end()));
}

TEST_CASE("exceptions") {
    CHECK_THROWS_WITH_AS(ac::foo::Model({.path = "nope nope"}), "Failed to open file: nope nope", std::runtime_error);
}
