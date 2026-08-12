#pragma once

#include "EventQueue.h"
#include "Renderer.h"
#include "World.h"

namespace BerkEngine {

class Application;

class Scene {
public:
    virtual ~Scene() = default;

    virtual void onEnter(Application& /*application*/) {}
    virtual void onExit(Application& /*application*/) {}
    virtual void onPollEvents(Application& /*application*/, EventQueue& /*events*/) {}
    virtual void onFixedUpdate(Application& /*application*/, World& /*world*/, double /*fixedDeltaSeconds*/) {}
    virtual void onVariableUpdate(Application& /*application*/, World& /*world*/, double /*deltaSeconds*/) {}
    virtual void onRender(Application& /*application*/, World& world, IRenderer& renderer, double interpolationAlpha) {
        renderer.draw(world, interpolationAlpha);
    }
};

} // namespace BerkEngine
