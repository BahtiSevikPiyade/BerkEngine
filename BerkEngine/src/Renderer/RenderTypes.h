#pragma once

#include <glm/glm.hpp>
#include <cstdint>

namespace BerkEngine {

// Shared types across all render backends

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
};

enum class RenderBackendType {
    OpenGL,
    Vulkan
};

} // namespace BerkEngine
