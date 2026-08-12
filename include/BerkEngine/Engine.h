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

        while (mRunning && app.isRunning()) {
            app.onPollEvents();

            mTimer.beginFrame();

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
        }
    }

    void stop() {
        mRunning = false;
    }

    bool isRunning() const {
        return mRunning;
    }

private:
    Config mConfig{};
    Timer mTimer{};
    bool mRunning{false};
};

} // namespace BerkEngine
