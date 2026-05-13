#pragma once

#include "Backend/RenderBackend.h"
#include "RenderTypes.h"
#include <memory>
#include <glm/glm.hpp>

namespace BerkEngine {

class Camera;
class Shader;
class Mesh;

class Renderer {
public:
    static void Init(RenderBackendType type, uint32_t width, uint32_t height);
    static void Shutdown();

    static void BeginFrame();
    static void EndFrame();
    static void Clear();
    static void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
    static void SetClearColor(float r, float g, float b, float a);

    // Drawing
    static void Submit(const Mesh& mesh, const glm::mat4& transform, const Shader& shader, const Camera& camera);

    // State management
    static void SetCamera(Camera* camera);
    static void SetShader(Shader* shader);

    // Queries
    static RenderBackendType GetBackendType();
    static bool IsInitialized();

private:
    static std::unique_ptr<RenderBackend> s_Backend;
    static RenderBackendType s_BackendType;
    static Camera* s_Camera;
    static Shader* s_Shader;
    static bool s_Initialized;
};

} // namespace BerkEngine
