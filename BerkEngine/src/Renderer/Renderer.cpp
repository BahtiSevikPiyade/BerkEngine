#include "bepch.h"
#include "Renderer.h"
#include "Backend/OpenGL/OpenGLBackend.h"
#include "Backend/Vulkan/VulkanBackend.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "Core/Log.h"

namespace BerkEngine {

std::unique_ptr<RenderBackend> Renderer::s_Backend = nullptr;
RenderBackendType Renderer::s_BackendType = RenderBackendType::OpenGL;
Camera* Renderer::s_Camera = nullptr;
Shader* Renderer::s_Shader = nullptr;
bool Renderer::s_Initialized = false;

void Renderer::Init(RenderBackendType type, uint32_t width, uint32_t height) {
    s_BackendType = type;

    switch (type) {
        case RenderBackendType::OpenGL:
            s_Backend = std::make_unique<OpenGLBackend>();
            break;
        case RenderBackendType::Vulkan:
            s_Backend = std::make_unique<VulkanBackend>();
            break;
        default:
            BE_CORE_ERROR("Unknown render backend type!");
            return;
    }

    if (s_Backend) {
        s_Backend->Init(width, height);
        s_Backend->SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        s_Backend->EnableDepthTest(true);
        s_Backend->EnableFaceCulling(false);
        s_Initialized = true;
        BE_CORE_INFO("Renderer initialized with {} backend", type == RenderBackendType::OpenGL ? "OpenGL" : "Vulkan");
    }
}

void Renderer::Shutdown() {
    if (s_Backend) {
        s_Backend->Shutdown();
    }
    s_Initialized = false;
    BE_CORE_INFO("Renderer shutdown");
}

void Renderer::BeginFrame() {
    if (s_Backend)
        s_Backend->BeginFrame();
}

void Renderer::EndFrame() {
    if (s_Backend)
        s_Backend->EndFrame();
}

void Renderer::Clear() {
    if (s_Backend)
        s_Backend->Clear(0.1f, 0.1f, 0.1f, 1.0f);
}

void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    if (s_Backend)
        s_Backend->SetViewport(x, y, w, h);
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    if (s_Backend)
        s_Backend->SetClearColor(r, g, b, a);
}

void Renderer::Submit(const Mesh& mesh, const glm::mat4& transform, const Shader& shader, const Camera& camera) {
    if (!s_Backend) {
        BE_CORE_ERROR("Renderer::Submit - Backend not initialized!");
        return;
    }
    s_Backend->Submit(mesh, transform, shader, camera);
}

void Renderer::SetCamera(Camera* camera) {
    s_Camera = camera;
}

void Renderer::SetShader(Shader* shader) {
    s_Shader = shader;
}

RenderBackendType Renderer::GetBackendType() {
    return s_BackendType;
}

bool Renderer::IsInitialized() {
    return s_Initialized;
}

} // namespace BerkEngine
