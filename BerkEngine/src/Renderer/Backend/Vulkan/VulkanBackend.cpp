#include "VulkanBackend.h"
#include "../../../Core/Log.h"

namespace BerkEngine {

VulkanBackend::VulkanBackend() {
    BE_CORE_INFO("Vulkan Backend created (stub - not implemented yet)");
}

VulkanBackend::~VulkanBackend() {
    Shutdown();
}

void VulkanBackend::Init(uint32_t width, uint32_t height) {
    m_Width = width;
    m_Height = height;
    m_Initialized = true;
    BE_CORE_WARN("Vulkan Backend - Init stub - not implemented yet");
}

void VulkanBackend::Shutdown() {
    m_Initialized = false;
    BE_CORE_INFO("Vulkan Backend shutdown");
}

void VulkanBackend::BeginFrame() {
    // TODO: Implement Vulkan frame begin
}

void VulkanBackend::EndFrame() {
    // TODO: Implement Vulkan frame end
}

void VulkanBackend::Clear(float r, float g, float b, float a) {
    // TODO: Implement Vulkan clear
}

void VulkanBackend::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    // TODO: Implement Vulkan viewport
}

void VulkanBackend::SetClearColor(float r, float g, float b, float a) {
    // TODO: Implement Vulkan clear color
}

void VulkanBackend::EnableDepthTest(bool enable) {
    // TODO: Implement Vulkan depth test
}

void VulkanBackend::EnableFaceCulling(bool enable) {
    // TODO: Implement Vulkan face culling
}

std::unique_ptr<Shader> VulkanBackend::CreateShader(const std::string& vertPath, const std::string& fragPath) {
    BE_CORE_WARN("VulkanBackend::CreateShader - not implemented yet");
    return nullptr;
}

std::unique_ptr<Mesh> VulkanBackend::CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    BE_CORE_WARN("VulkanBackend::CreateMesh - not implemented yet");
    return nullptr;
}

void VulkanBackend::Submit(const Mesh& mesh, const glm::mat4& transform, const Shader& shader, const Camera& camera) {
    // TODO: Implement Vulkan submit
}

} // namespace BerkEngine
