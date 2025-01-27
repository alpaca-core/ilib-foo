// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#pragma once
#include <ac/schema/Field.hpp>
#include <vector>
#include <string>
#include <tuple>

namespace ac::schema {

struct FooInterface {
    static inline constexpr std::string_view id = "foo/v1";
    static inline constexpr std::string_view description = "Foo interface";

    struct OpRun {
        static inline constexpr std::string_view id = "run";
        static inline constexpr std::string_view description = "Run the foo inference and produce some output";

        struct Params {
            Field<std::vector<std::string>> input;
            Field<bool> splice = Default(true);
            Field<int> throwOn = Default(-1);

            template <typename Visitor>
            void visitFields(Visitor& v) {
                v(input, "input", "Input items");
                v(splice, "splice", "Splice input with model data (otherwise append model data to input)");
                v(throwOn, "throw_on", "Throw exception on n-th token (or don't throw if -1)");
            }
        };
        struct Return {
            Field<std::string> result;

            template <typename Visitor>
            void visitFields(Visitor& v) {
                v(result, "result", "Output text (tokens joined with space)");
            }
        };
    };

    using Ops = std::tuple<OpRun>;
};

struct FooProvider {
    static inline constexpr std::string_view id = "foo";
    static inline constexpr std::string_view description = "Foo inference for tests, examples, and experiments.";

    struct Params {
        Field<std::string> spliceString = std::nullopt;

        template <typename Visitor>
        void visitFields(Visitor& v) {
            v(spliceString, "splice_string", "String to splice between model data elements");
        }
    };

    struct InstanceGeneral {
        static inline constexpr std::string_view id = "general";
        static inline constexpr std::string_view description = "General instance";

        struct Params {
            Field<int> cutoff = Default(-1);

            template <typename Visitor>
            void visitFields(Visitor& v) {
                v(cutoff, "cutoff", "Cut off model data to n-th element (or don't cut if -1)");
            }
        };

        using Interfaces = std::tuple<FooInterface>;
    };

    using Instances = std::tuple<InstanceGeneral>;
};

} // namespace ac::schema
