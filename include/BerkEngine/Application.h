#pragma once

#include "World.h"

namespace BerkEngine {

class Application {
public:
    virtual ~Application() = default;

    virtual void onPollEvents() {}
    virtual void onFixedUpdate(World& /*world*/, double /*fixedDeltaSeconds*/) {}
    virtual void onVariableUpdate(World& /*world*/, double /*deltaSeconds*/) {}
    virtual void onRender(World& /*world*/, double /*interpolationAlpha*/) {}

    void requestQuit() {
        mRunning = false;
    }

    bool isRunning() const {
        return mRunning;
    }

    World& world() {
        return mWorld;
    }

    const World& world() const {
        return mWorld;
    }

private:
    World mWorld{};
    bool mRunning{true};
};

} // namespace BerkEngine
