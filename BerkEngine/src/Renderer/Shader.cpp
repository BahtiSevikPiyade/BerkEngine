#include "Shader.h"
#include "Backend/OpenGL/OpenGLShader.h"
// #include "Backend/Vulkan/VulkanShader.h" // TODO when implemented

namespace BerkEngine {

std::unique_ptr<Shader> Shader::Create(const std::string& vertPath, const std::string& fragPath) {
    // For now, hardcoded to OpenGL. Later, can be selected via Renderer::GetBackendType()
    return std::make_unique<OpenGLShader>(vertPath, fragPath);
}

} // namespace BerkEngine