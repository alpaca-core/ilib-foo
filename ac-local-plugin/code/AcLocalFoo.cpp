// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/schema/ModelSchema.hpp>

#include <ac/foo/Model.hpp>
#include <ac/foo/Instance.hpp>

#include <ac/local/PluginInterface.hpp>

#include <ac/local/Instance.hpp>
#include <ac/local/Model.hpp>
#include <ac/local/ModelLoader.hpp>
#include <ac/local/ModelFactory.hpp>

#include <astl/move.hpp>
#include <astl/workarounds.h>
#include <itlib/throw_ex.hpp>
#include <stdexcept>

namespace ac::local {

namespace schema {

struct Foo : public ModelHelper<Foo> {
    static inline constexpr std::string_view id = "foo";
    static inline constexpr std::string_view description = "Example inference plugin";

    struct Params : public Object {
        using Object::Object;
        String spliceString{*this, "splice_string", "String to splice model data with input"};
    };

    struct InstanceGeneral : public InstanceHelper<InstanceGeneral> {
        static inline constexpr std::string_view id = "general";
        static inline constexpr std::string_view description = "General instance";

        struct Params : public Object {
            using Object::Object;
            Int cutoff{*this, "cutoff", "Cut off model data to n-th element (or don't cut if -1)", -1};
        };

        struct OpRun {
            static inline constexpr std::string_view id = "run";
            static inline constexpr std::string_view description = "Run the foo inference and produce some output";

            struct Params : public Object {
                using Object::Object;
                Array<String> input{*this, "input", "Input items"};
                Bool splice{*this, "splice", "Splice input with model data (otherwise append model data to input)", true};
                Int throwOn{*this, "throw_on", "Throw exception on n-th token (or don't throw if -1)", -1};
            };

            struct Return : public Object {
                using Object::Object;
                String result{*this, "result", "Output text (tokens joined with space)", {}, true};
            };
        };

        using Ops = std::tuple<OpRun>;
    };

    using Instances = std::tuple<InstanceGeneral>;
};

} // namespace schema

using throw_ex = itlib::throw_ex<std::runtime_error>;

class FooInstance final : public Instance {
    std::shared_ptr<foo::Model> m_model;
    foo::Instance m_instance;
public:
    using Schema = ac::local::schema::Foo::InstanceGeneral;

    static foo::Instance::InitParams InitParams_fromDict(Dict& d) {
        Schema::Params schemaParams(d);
        foo::Instance::InitParams ret;
        ret.cutoff = schemaParams.cutoff.getValue();
        return ret;
    }

    FooInstance(std::shared_ptr<foo::Model> model, Dict& params)
        : m_model(astl::move(model))
        , m_instance(*m_model, InitParams_fromDict(params))
    {}

    Dict run(Dict& params) {
        const Schema::OpRun::Params schemaParams(params);

        if (!schemaParams.input.hasValue()) {
            throw_ex{} << "Missing input";
        }

        std::vector<std::string> input;
        input.reserve(schemaParams.input.size());
        for (size_t i = 0; i < schemaParams.input.size(); ++i) {
            input.push_back(std::string(schemaParams.input[i].getValue()));
        }

        foo::Instance::SessionParams sparams;
        sparams.splice = schemaParams.splice.getValue();
        sparams.throwOn = schemaParams.throwOn.getValue();

        auto s = m_instance.newSession(std::move(input), sparams);

        Dict ret;
        Schema::OpRun::Return schemaRet(ret);

        std::string result;
        for (auto& w : s) {
            result += w;
            result += ' ';
        }
        if (!result.empty()) {
            // remove last space
            result.pop_back();
        }

        schemaRet.result.setValue(std::move(result));
        return ret;
    }

    virtual Dict runOp(std::string_view op, Dict params, ProgressCb) override {
        switch (Schema::getOpIndexById(op)) {
        case Schema::opIndex<Schema::OpRun>:
            return run(params);
        default:
            throw_ex{} << "foo: unknown op: " << op;
            MSVC_WO_10766806();
        }
    }
};

class FooModel final : public Model {
    std::shared_ptr<foo::Model> m_model;
public:
    using Schema = ac::local::schema::Foo;

    static foo::Model::Params ModelParams_fromDict(Dict& d) {
        Schema::Params schemaParams(d);
        foo::Model::Params ret;
        ret.splice = std::string(schemaParams.spliceString.optGetValue().value_or(""));
        return ret;
    }

    FooModel(const std::string& fname, Dict& params)
        : m_model(std::make_shared<foo::Model>(fname.c_str(), ModelParams_fromDict(params)))
    {}
    explicit FooModel(Dict& params) : m_model(std::make_shared<foo::Model>(ModelParams_fromDict(params))) {}

    virtual std::unique_ptr<Instance> createInstance(std::string_view type, Dict params) override {
        switch (Schema::getInstanceById(type)) {
        case Schema::instanceIndex<Schema::InstanceGeneral>:
            return std::make_unique<FooInstance>(m_model, params);
        default:
            throw_ex{} << "foo: unknown instance type: " << type;
            MSVC_WO_10766806();
        }
    }
};

class FooModelLoader final : public ModelLoader {
public:
    virtual ModelPtr loadModel(ModelDesc desc, Dict params, ProgressCb pcb) override {
        if (desc.assets.size() > 1) throw_ex{} << "foo: expected one or zero assets";

        if (desc.assets.empty()) {
            // synthetic model
            if (pcb) {
                if (!pcb("synthetic", 0.5f)) {
                    throw_ex{} << "foo: loading model aborted";
                }
            }
            return std::make_shared<FooModel>(params);
        }
        else {
            auto& fname = desc.assets.front().path;
            if (pcb) {
                if (!pcb(fname, 0.1f)) {
                    throw_ex{} << "foo: loading model aborted";
                }
            }
            return std::make_shared<FooModel>(fname, params);
        }
    }
};

// plugin interface

void addFooToFactory(ac::local::ModelFactory& factory) {
    static ac::local::FooModelLoader loader;
    factory.addLoader("foo", loader);
}

extern "C" SYMBOL_EXPORT
PluginInterface acLocalPluginLoad() {
    return {
        .acLocalVersion = ac::local::Project_Version,
        .pluginVersion = ac::local::Project_Version, // temp
        .addLoadersToFactory = addFooToFactory,
    };
}
static_assert(std::is_same_v<decltype(&acLocalPluginLoad), PluginInterface::PluginLoadFunc>);

} // namespace ac::local

