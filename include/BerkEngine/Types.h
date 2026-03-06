#pragma once

#include <bitset>
#include <cstdint>

namespace BerkEngine {

// An Entity is simply a unique numeric ID
using Entity = std::uint32_t;

// Maximum number of entities alive at once
constexpr Entity MAX_ENTITIES = 5000;

// A ComponentType identifies a registered component type
using ComponentType = std::uint8_t;

// Maximum number of distinct component types
constexpr ComponentType MAX_COMPONENTS = 32;

// A Signature represents which components an entity (or system) uses
using Signature = std::bitset<MAX_COMPONENTS>;

} // namespace BerkEngine
