// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/foo/Instance.hpp>
#include <ac/foo/Model.hpp>
#include "ac-test-data-foo-models.h"
#include <doctest/doctest.h>

TEST_CASE("splice") {
    ac::foo::Model small(AC_FOO_MODEL_SMALL, {});
    ac::foo::Instance inst(small, {.cutoff = 2});
    {
        auto s = inst.newSession({"a", "b", "c"}, {});
        CHECK(*s.next() == "a");
        CHECK(*s.next() == "soco");
        CHECK(*s.next() == "b");
        CHECK(*s.next() == "bate");
        CHECK(*s.next() == "c");
        CHECK(*s.next() == "soco");
        CHECK_FALSE(s.next());
    }
}

TEST_CASE("no-splice") {
    ac::foo::Model small(AC_FOO_MODEL_SMALL, {});
    ac::foo::Instance inst(small, {});
    {
        auto s = inst.newSession({"a", "b"}, {.splice = false});
        CHECK(*s.next() == "a");
        CHECK(*s.next() == "b");
        CHECK(*s.next() == "soco");
        CHECK(*s.next() == "bate");
        CHECK(*s.next() == "vira");
        CHECK_FALSE(s.next());
    }
}

TEST_CASE("model splice") {
    ac::foo::Model small(AC_FOO_MODEL_SMALL, {.splice = "x"});
    ac::foo::Instance inst(small, {});
    {
        auto s = inst.newSession({ "a", "b", "c" }, {});
        CHECK(*s.next() == "a");
        CHECK(*s.next() == "x");
        CHECK(*s.next() == "b");
        CHECK(*s.next() == "soco");
        CHECK(*s.next() == "c");
        CHECK(*s.next() == "x");
        CHECK_FALSE(s.next());
    }
}

TEST_CASE("exceptions") {
    ac::foo::Model small(AC_FOO_MODEL_SMALL, {});

    CHECK_THROWS_WITH_AS(ac::foo::Instance(small, {.cutoff = 1000}), "Cutoff 1000 greater than model size 3", std::runtime_error);

    ac::foo::Instance inst(small, {});
    {
        auto s = inst.newSession({ "a", "b" }, {.throwOn = 0});
        CHECK_THROWS_WITH_AS(*s.next(), "Throw on token 0", std::runtime_error);
    }
    {
        auto s = inst.newSession({"a", "b"}, {.throwOn = 2});
        CHECK(*s.next() == "a");
        CHECK(*s.next() == "soco");
        CHECK_THROWS_WITH_AS(*s.next(), "Throw on token 2", std::runtime_error);
    }
    {
        auto s = inst.newSession({ "a", "b" }, {.splice = false, .throwOn = 3});
        CHECK(*s.next() == "a");
        CHECK(*s.next() == "b");
        CHECK(*s.next() == "soco");
        CHECK_THROWS_WITH_AS(*s.next(), "Throw on token 3", std::runtime_error);
    }
}
