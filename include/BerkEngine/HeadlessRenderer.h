#pragma once

#include "Renderer.h"

#include <cstdint>

namespace BerkEngine {

class HeadlessRenderer : public IRenderer {
public:
    void initialize() override {
        mInitialized = true;
    }

    void beginFrame() override {
        if (mInitialized) {
            ++mFrameCount;
        }
    }

    void draw(World&, double interpolationAlpha) override {
        mLastInterpolationAlpha = interpolationAlpha;
    }

    void shutdown() override {
        mInitialized = false;
    }

    bool isInitialized() const {
        return mInitialized;
    }

    std::uint64_t frameCount() const {
        return mFrameCount;
    }

    double lastInterpolationAlpha() const {
        return mLastInterpolationAlpha;
    }

private:
    bool mInitialized{false};
    std::uint64_t mFrameCount{0};
    double mLastInterpolationAlpha{0.0};
};

} // namespace BerkEngine
