// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/schema/Foo.hpp>
#include <ac/schema/GenerateSchemaDict.hpp>
#include <iostream>

int main() {
    auto d = Interface_generateSchemaDict<acnl::ordered_json>(ac::schema::foo::Interface{});
    std::cout << d.dump(2) << std::endl;
    return 0;
}
