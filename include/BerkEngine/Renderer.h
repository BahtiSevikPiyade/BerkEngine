#pragma once

#include "AssetManager.h"
#include "RenderTypes.h"
#include "World.h"

namespace BerkEngine {

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void initialize() {}
    virtual void beginFrame() {}
    virtual void draw(
        World& /*world*/,
        const RenderQueue& /*queue*/,
        AssetManager& /*assets*/,
        double /*interpolationAlpha*/
    ) {}
    virtual void endFrame() {}
    virtual void shutdown() {}
};

} // namespace BerkEngine
