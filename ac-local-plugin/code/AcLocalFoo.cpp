// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/foo/Model.hpp>
#include <ac/foo/Instance.hpp>

#include <ac/local/PluginInterface.hpp>

#include <ac/local/Instance.hpp>
#include <ac/local/Model.hpp>
#include <ac/local/ModelLoader.hpp>

#include <astl/move.hpp>
#include <astl/workarounds.h>
#include <itlib/throw_ex.hpp>
#include <stdexcept>

#include "foo-schema.hpp"
#include "aclp-foo-version.h"
#include "aclp-foo-interface.hpp"

namespace ac::local {

using throw_ex = itlib::throw_ex<std::runtime_error>;

class FooInstance final : public Instance {
    std::shared_ptr<foo::Model> m_model;
    foo::Instance m_instance;
public:
    using Schema = ac::local::schema::Foo::InstanceGeneral;

    static foo::Instance::InitParams InitParams_fromDict(Dict& d) {
        auto schemaParams = Schema::Params::fromDict(d);
        foo::Instance::InitParams ret;
        ret.cutoff = schemaParams.cutoff;
        return ret;
    }

    FooInstance(std::shared_ptr<foo::Model> model, Dict& params)
        : m_model(astl::move(model))
        , m_instance(*m_model, InitParams_fromDict(params))
    {}

    Dict run(Dict& params) {
        const auto schemaParams = Schema::OpRun::Params::fromDict(params);

        foo::Instance::SessionParams sparams;
        sparams.splice = schemaParams.splice;
        sparams.throwOn = schemaParams.throwOn;

        auto s = m_instance.newSession(std::move(schemaParams.input), sparams);

        Schema::OpRun::Return ret;
        for (auto& w : s) {
            ret.result += w;
            ret.result += ' ';
        }
        if (!ret.result.empty()) {
            // remove last space
            ret.result.pop_back();
        }

        return ret.toDict();
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
        auto schemaParams = Schema::Params::fromDict(d);
        foo::Model::Params ret;
        ret.splice = astl::move(schemaParams.spliceString.value_or(""));
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
    virtual const Info& info() const noexcept override {
        static Info i = {
            .name = "ac foo",
            .vendor = "Alpaca Core",
        };
        return i;
    }

    virtual bool canLoadModel(const ModelAssetDesc& desc, const Dict&) const noexcept override {
        return desc.type == "foo";
    }

    virtual ModelPtr loadModel(ModelAssetDesc desc, Dict params, ProgressCb pcb) override {
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

} // namespace ac::local

namespace ac::foo {

std::vector<ac::local::ModelLoaderPtr> getLoaders() {
    std::vector<ac::local::ModelLoaderPtr> ret;
    ret.push_back(std::make_unique<local::FooModelLoader>());
    return ret;
}

local::PluginInterface getPluginInterface() {
    return {
        .label = "ac foo",
        .desc = "Foo plugin for ac-local",
        .vendor = "Alpaca Core",
        .version = astl::version{
            ACLP_foo_VERSION_MAJOR, ACLP_foo_VERSION_MINOR, ACLP_foo_VERSION_PATCH
        },
        .init = nullptr,
        .getLoaders = getLoaders,
    };
}

} // namespace ac::foo