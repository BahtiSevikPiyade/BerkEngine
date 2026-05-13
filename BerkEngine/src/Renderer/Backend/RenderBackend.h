#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "../RenderTypes.h"
#include "../Mesh.h"
#include "../Shader.h"
#include "../Camera.h"

namespace BerkEngine {

class RenderBackend {
public:
    virtual ~RenderBackend() = default;

    // Initialization & shutdown
    virtual void Init(uint32_t width, uint32_t height) = 0;
    virtual void Shutdown() = 0;

    // Frame management
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void Clear(float r, float g, float b, float a) = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;

    // Resource creation
    virtual std::unique_ptr<Shader> CreateShader(const std::string& vertPath, const std::string& fragPath) = 0;
    virtual std::unique_ptr<Mesh> CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) = 0;

    // Rendering
    virtual void Submit(const Mesh& mesh, const glm::mat4& transform, const Shader& shader, const Camera& camera) = 0;

    // State management
    virtual void SetClearColor(float r, float g, float b, float a) = 0;
    virtual void EnableDepthTest(bool enable) = 0;
    virtual void EnableFaceCulling(bool enable) = 0;
};

} // namespace BerkEngine
