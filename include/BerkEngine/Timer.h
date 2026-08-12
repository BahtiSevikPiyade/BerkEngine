#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>

namespace BerkEngine {

class Timer {
public:
    struct Config {
        double fixedTimeStepSeconds{1.0 / 60.0};
        double maxFrameDeltaSeconds{0.25};
    };

    struct FrameSample {
        double rawDeltaSeconds{0.0};
        double deltaSeconds{0.0};
        std::uint32_t scheduledFixedTicks{0};
    };

    Timer()
        : Timer(Config{})
    {}

    explicit Timer(Config config)
        : mConfig(config)
        , mLastTick(Clock::now())
    {}

    FrameSample beginFrame() {
        const auto now = Clock::now();
        const std::chrono::duration<double> dt = now - mLastTick;
        mLastTick = now;

        mRawDeltaSeconds = dt.count();
        mDeltaSeconds = std::min(mRawDeltaSeconds, mConfig.maxFrameDeltaSeconds);
        mAccumulatorSeconds += mDeltaSeconds;

        const auto ticks = static_cast<std::uint32_t>(mAccumulatorSeconds / mConfig.fixedTimeStepSeconds);
        return FrameSample{mRawDeltaSeconds, mDeltaSeconds, ticks};
    }

    bool consumeFixedStep() {
        if (mAccumulatorSeconds < mConfig.fixedTimeStepSeconds) {
            return false;
        }
        mAccumulatorSeconds -= mConfig.fixedTimeStepSeconds;
        return true;
    }

    void dropAccumulatedLag() {
        mAccumulatorSeconds = 0.0;
    }

    double fixedTimeStepSeconds() const {
        return mConfig.fixedTimeStepSeconds;
    }

    double deltaSeconds() const {
        return mDeltaSeconds;
    }

    double interpolationAlpha() const {
        if (mConfig.fixedTimeStepSeconds <= 0.0) {
            return 0.0;
        }
        return mAccumulatorSeconds / mConfig.fixedTimeStepSeconds;
    }

private:
    using Clock = std::chrono::steady_clock;

    Config mConfig{};
    Clock::time_point mLastTick{};
    double mRawDeltaSeconds{0.0};
    double mDeltaSeconds{0.0};
    double mAccumulatorSeconds{0.0};
};

} // namespace BerkEngine
