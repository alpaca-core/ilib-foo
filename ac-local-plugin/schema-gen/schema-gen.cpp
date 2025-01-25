// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/schema/Foo.hpp>
#include <ac/schema/GenerateProviderSchemaDict.hpp>
#include <iostream>

int main() {
    auto d = ac::local::schema::generateProviderSchema<acnl::ordered_json, ac::local::schema::FooProvider>();
    std::cout << d.dump(2) << std::endl;
    return 0;
}
