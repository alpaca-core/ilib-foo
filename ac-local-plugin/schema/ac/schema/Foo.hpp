// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#pragma once
#include <ac/schema/Field.hpp>
#include <vector>
#include <string>
#include <tuple>

namespace ac::local::schema {

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

struct FooEmbeddingInterface {
    static inline constexpr std::string_view id = "foo/v1";
    static inline constexpr std::string_view description = "Foo embedding interface";

    struct OpRun {
        static inline constexpr std::string_view id = "run";
        static inline constexpr std::string_view description = "Run the foo inference and produce some output";

        struct Params {
            Field<std::string> input;

            template <typename Visitor>
            void visitFields(Visitor& v) {
                v(input, "input", "Input items");
            }
        };
        struct Return {
            Field<std::vector<float>> result;

            template <typename Visitor>
            void visitFields(Visitor& v) {
                v(result, "result", "Output text (tokens joined with space)");
            }
        };
    };

    using Ops = std::tuple<OpRun>;
};

struct FooLoader {
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

    struct InstanceEmbedding {
        static inline constexpr std::string_view id = "embedding";
        static inline constexpr std::string_view description = "embedding instance";

        struct Params {
            Field<int> vectorSize = Default(64);

            template <typename Visitor>
            void visitFields(Visitor& v) {
                v(vectorSize, "vectorSize", "The size of the embedding vector");
            }
        };

        using Interfaces = std::tuple<FooEmbeddingInterface>;
    };

    using Instances = std::tuple<InstanceGeneral>;
};

} // namespace ac::local::schema
