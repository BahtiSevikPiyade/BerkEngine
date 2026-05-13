#pragma once

#include "../RenderBackend.h"

namespace BerkEngine {

class VulkanBackend : public RenderBackend {
public:
    VulkanBackend();
    ~VulkanBackend() override;

    void Init(uint32_t width, uint32_t height) override;
    void Shutdown() override;

    void BeginFrame() override;
    void EndFrame() override;
    void Clear(float r, float g, float b, float a) override;
    void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) override;

    std::unique_ptr<Shader> CreateShader(const std::string& vertPath, const std::string& fragPath) override;
    std::unique_ptr<Mesh> CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) override;

    void Submit(const Mesh& mesh, const glm::mat4& transform, const Shader& shader, const Camera& camera) override;

    void SetClearColor(float r, float g, float b, float a) override;
    void EnableDepthTest(bool enable) override;
    void EnableFaceCulling(bool enable) override;

private:
    bool m_Initialized = false;
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
};

} // namespace BerkEngine
