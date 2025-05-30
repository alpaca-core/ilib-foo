// Copyright (c) Alpaca Core
// SPDX-License-Identifier: MIT
//
#include <ac/foo/Model.hpp>
#include <ac/foo/Instance.hpp>

#include "aclp-foo-version.h"
#include "aclp-foo-interface.hpp"

#include <ac/schema/Foo.hpp>

#include <ac/local/Service.hpp>
#include <ac/local/ServiceFactory.hpp>
#include <ac/local/ServiceInfo.hpp>
#include <ac/local/Backend.hpp>
#include <ac/local/BackendWorkerStrand.hpp>

#include <ac/schema/OpDispatchHelpers.hpp>

#include <ac/FrameUtil.hpp>
#include <ac/frameio/IoEndpoint.hpp>

#include <ac/xec/coro.hpp>
#include <ac/io/exception.hpp>

#include <astl/move.hpp>
#include <astl/throw_stdex.hpp>

#include <deque>

namespace ac::local {

namespace {

namespace sc = schema::foo;

using namespace ac::frameio;

struct BasicRunner {
    schema::OpDispatcherData m_dispatcherData;

    Frame dispatch(Frame& f) {
        try {
            auto ret = m_dispatcherData.dispatch(f.op, std::move(f.data));
            if (!ret) {
                throw_ex{} << "foo: unknown op: " << f.op;
            }
            return {f.op, *ret};
        }
        catch (io::stream_closed_error&) {
            throw;
        }
        catch (std::exception& e) {
            return { "error", e.what() };
        }
    }
};

xec::coro<void> Foo_runInstance(IoEndpoint& io, std::unique_ptr<foo::Instance> instance) {
    using Schema = sc::StateInstance;

    struct Runner : public BasicRunner {
        foo::Instance& m_instance;

        explicit Runner(foo::Instance& instance) : m_instance(instance) {
            schema::registerHandlers<Schema::Ops>(m_dispatcherData, *this);
        }
        Schema::OpRun::Return on(Schema::OpRun, Schema::OpRun::Params params) {
            foo::Instance::SessionParams sparams;
            sparams.splice = params.splice;
            sparams.throwOn = params.throwOn;

            auto s = m_instance.newSession(std::move(params.input), sparams);

            Schema::OpRun::Return ret;
            auto& res = ret.result.materialize();
            for (auto& w : s) {
                res += w;
                res += ' ';
            }
            if (!res.empty()) {
                // remove last space
                res.pop_back();
            }

            return ret;
        }
        Schema::OpGenBlob::Return on(Schema::OpGenBlob, Schema::OpGenBlob::Params params) {
            Schema::OpGenBlob::Return ret;
            auto& blob = ret.blob.materialize();
            for (auto& w : m_instance.model().data()) {
                std::string_view token = w;
                while (blob.size() < params.maxSize.value() && !token.empty()) {
                    blob.push_back(uint8_t(token.front()));
                    token.remove_prefix(1);
                }
            }
            return ret;
        }
    };

    co_await io.push(Frame_stateChange(Schema::id));

    Runner runner(*instance);

    while (true) {
        auto f = co_await io.poll();
        co_await io.push(runner.dispatch(f.value));
    }
}

xec::coro<void> Foo_runModel(IoEndpoint& io, std::unique_ptr<foo::Model> model) {
    using Schema = sc::StateModelLoaded;

    struct Runner : public BasicRunner {
        foo::Model& model;

        explicit Runner(foo::Model& m) : model(m) {
            schema::registerHandlers<Schema::Ops>(m_dispatcherData, *this);
        }

        std::unique_ptr<foo::Instance> instance;

        static foo::Instance::InitParams InitParams_fromSchema(sc::StateModelLoaded::OpCreateInstance::Params schemaParams) {
            foo::Instance::InitParams ret;
            ret.cutoff = schemaParams.cutoff;
            return ret;
        }

        Schema::OpCreateInstance::Return on(Schema::OpCreateInstance, Schema::OpCreateInstance::Params params) {
            instance = std::make_unique<foo::Instance>(model, InitParams_fromSchema(params));
            return {};
        }
    };

    co_await io.push(Frame_stateChange(Schema::id));

    Runner runner(*model);

    while (true) {
        auto f = co_await io.poll();
        co_await io.push(runner.dispatch(f.value));
        if (runner.instance) {
            co_await Foo_runInstance(io, astl::move(runner.instance));
        }
    }
}

xec::coro<void> Foo_runSession(StreamEndpoint ep) {
    using Schema = sc::StateInitial;

    struct Runner : public BasicRunner {
        Runner() {
            schema::registerHandlers<Schema::Ops>(m_dispatcherData, *this);
        }

        std::unique_ptr<foo::Model> model;

        static foo::Model::Params ModelParams_fromSchema(sc::StateInitial::OpLoadModel::Params schemaParams) {
            foo::Model::Params ret;
            ret.path = schemaParams.filePath.valueOr("");
            ret.splice = astl::move(schemaParams.spliceString.valueOr(""));
            return ret;
        }

        Schema::OpLoadModel::Return on(Schema::OpLoadModel, Schema::OpLoadModel::Params params) {
            model = std::make_unique<foo::Model>(ModelParams_fromSchema(params));
            return {};
        }
    };

    try {
        auto ex = co_await xec::executor{};
        IoEndpoint io(std::move(ep), ex);

        co_await io.push(Frame_stateChange(Schema::id));

        Runner runner;

        while (true) {
            auto f = co_await io.poll();
            co_await io.push(runner.dispatch(f.value));
            if (runner.model) {
                co_await Foo_runModel(io, astl::move(runner.model));
            }
        }
    }
    catch (io::stream_closed_error&) {
        co_return;
    }
}

ServiceInfo g_serviceInfo = {
    .name = "ac-local foo",
    .vendor = "Alpaca Core",
};

struct FooService final : public Service {
    FooService(BackendWorkerStrand& ws) : m_workerStrand(ws) {}

    BackendWorkerStrand& m_workerStrand;

    virtual const ServiceInfo& info() const noexcept override {
        return g_serviceInfo;
    }

    virtual void createSession(frameio::StreamEndpoint ep, Dict) override {
        co_spawn(m_workerStrand.executor(), Foo_runSession(std::move(ep)));
    }
};

struct FooServiceFactory final : public ServiceFactory {
    virtual const ServiceInfo& info() const noexcept override {
        return g_serviceInfo;
    }
    virtual std::unique_ptr<Service> createService(Backend& backend) const override {
        auto svc = std::make_unique<FooService>(backend.gpuWorkerStrand());
        return svc;
    }
};

} // namespace

} // namespace ac::local

namespace ac::foo {

std::vector<const local::ServiceFactory*> getFactories() {
    static local::FooServiceFactory factory;
    return {&factory};
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
        .getServiceFactories = getFactories,
    };
}

} // namespace ac::foo
