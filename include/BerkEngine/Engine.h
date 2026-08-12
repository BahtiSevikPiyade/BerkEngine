#pragma once

#include "Application.h"
#include "Timer.h"

#include <cstdint>

namespace BerkEngine {

class Engine {
public:
    struct Config {
        Timer::Config timer{};
        std::uint32_t maxFixedUpdatesPerFrame{5};
    };

    Engine()
        : Engine(Config{})
    {}

    explicit Engine(Config config)
        : mConfig(config)
        , mTimer(config.timer)
    {}

    void run(Application& app) {
        mRunning = true;
        if (app.platform() != nullptr) {
            app.platform()->initialize();
        }
        if (app.renderer() != nullptr) {
            app.renderer()->initialize();
        }

        while (mRunning && app.isRunning()) {
            runSingleFrame(app);
        }

        if (app.renderer() != nullptr) {
            app.renderer()->shutdown();
        }
        if (app.platform() != nullptr) {
            app.platform()->shutdown();
        }
    }

    void runSingleFrame(Application& app) {
        mTimer.beginFrame();
        runSingleFrameWithPreparedTimer(app);
    }

    void runSingleFrame(Application& app, double rawDeltaSeconds) {
        mTimer.beginFrame(rawDeltaSeconds);
        runSingleFrameWithPreparedTimer(app);
    }

    void stop() {
        mRunning = false;
    }

    bool isRunning() const {
        return mRunning;
    }

private:
    void runSingleFrameWithPreparedTimer(Application& app) {
        if (app.platform() != nullptr) {
            app.platform()->pollEvents(app.events());
            if (app.platform()->shouldClose()) {
                app.pushEvent(Event{EventType::Quit, false});
                app.requestQuit();
            }
        }

        app.onPollEvents(app.events());

        std::uint32_t fixedSteps = 0;
        while (mTimer.consumeFixedStep() && fixedSteps < mConfig.maxFixedUpdatesPerFrame) {
            app.onFixedUpdate(app.world(), mTimer.fixedTimeStepSeconds());
            ++fixedSteps;
        }

        if (fixedSteps == mConfig.maxFixedUpdatesPerFrame) {
            mTimer.dropAccumulatedLag();
        }

        app.onVariableUpdate(app.world(), mTimer.deltaSeconds());
        app.onRender(app.world(), mTimer.interpolationAlpha());
        app.clearFrameEvents();
    }

    Config mConfig{};
    Timer mTimer{};
    bool mRunning{false};
};

} // namespace BerkEngine
