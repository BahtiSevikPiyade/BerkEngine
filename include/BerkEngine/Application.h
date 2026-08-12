#pragma once

#include "AssetManager.h"
#include "EventQueue.h"
#include "Platform.h"
#include "Renderer.h"
#include "Scene.h"
#include "World.h"

#include <memory>
#include <utility>

namespace BerkEngine {

class Application {
public:
    virtual ~Application() = default;

    virtual void onPollEvents(EventQueue& events) {
        if (mActiveScene) {
            mActiveScene->onPollEvents(*this, events);
        }
    }

    virtual void onFixedUpdate(World& world, double fixedDeltaSeconds) {
        if (mActiveScene) {
            mActiveScene->onFixedUpdate(*this, world, fixedDeltaSeconds);
        }
    }

    virtual void onVariableUpdate(World& world, double deltaSeconds) {
        if (mActiveScene) {
            mActiveScene->onVariableUpdate(*this, world, deltaSeconds);
        }
    }

    virtual void onRender(World& world, double interpolationAlpha) {
        if (mRenderer == nullptr) {
            return;
        }

        mRenderer->beginFrame();
        if (mActiveScene) {
            mActiveScene->onRender(*this, world, *mRenderer, interpolationAlpha);
        } else {
            mRenderer->draw(world, interpolationAlpha);
        }
        mRenderer->endFrame();
    }

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

    EventQueue& events() {
        return mEvents;
    }

    const EventQueue& events() const {
        return mEvents;
    }

    bool pollEvent(Event& event) {
        return mEvents.poll(event);
    }

    void pushEvent(const Event& event) {
        mEvents.push(event);
    }

    void pushEvent(Event&& event) {
        mEvents.push(std::move(event));
    }

    void clearFrameEvents() {
        mEvents.clear();
    }

    void setRenderer(IRenderer* renderer) {
        mRenderer = renderer;
    }

    IRenderer* renderer() const {
        return mRenderer;
    }

    void setPlatform(IPlatform* platform) {
        mPlatform = platform;
    }

    IPlatform* platform() const {
        return mPlatform;
    }

    AssetManager& assets() {
        return mAssets;
    }

    const AssetManager& assets() const {
        return mAssets;
    }

    void setScene(std::shared_ptr<Scene> nextScene) {
        if (mActiveScene) {
            mActiveScene->onExit(*this);
        }
        mActiveScene = std::move(nextScene);
        if (mActiveScene) {
            mActiveScene->onEnter(*this);
        }
    }

    std::shared_ptr<Scene> scene() const {
        return mActiveScene;
    }

private:
    World mWorld{};
    EventQueue mEvents{};
    AssetManager mAssets{};
    IPlatform* mPlatform{nullptr};
    IRenderer* mRenderer{nullptr};
    std::shared_ptr<Scene> mActiveScene{};
    bool mRunning{true};
};

} // namespace BerkEngine
