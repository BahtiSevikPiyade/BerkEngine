#pragma once

#include "Types.h"

#include <set>

namespace BerkEngine {

// Base class for all user-defined systems.
// A system operates on the subset of entities whose component signature
// satisfies the system's own registered signature.
class System {
public:
    virtual ~System() = default;

    // Called every frame with the elapsed time in seconds.
    // Override this in concrete systems to implement per-frame logic.
    virtual void update(float /*deltaTime*/) {}

    // The set of entities that currently meet this system's signature.
    // Maintained automatically by the World / SystemManager.
    std::set<Entity> mEntities;
};

} // namespace BerkEngine
