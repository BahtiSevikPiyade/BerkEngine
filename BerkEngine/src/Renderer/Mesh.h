#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "RenderTypes.h"

namespace BerkEngine {

class Mesh {
public:
    virtual ~Mesh();

    // Prevent copying; only move allowed (resource ownership)
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    virtual void Draw() const = 0;
    virtual uint32_t GetVertexCount() const = 0;
    virtual uint32_t GetIndexCount() const = 0;

    // Factory method - creates backend-specific mesh
    static std::unique_ptr<Mesh> Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

    // Static factory for common meshes
    static std::unique_ptr<Mesh> CreateCube();

protected:
    // Protected constructor for derived classes
    Mesh() = default;
};

} // namespace BerkEngine
