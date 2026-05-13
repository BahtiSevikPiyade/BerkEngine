#include "OpenGLBackend.h"
#include "OpenGLShader.h"
#include "OpenGLMesh.h"
#include "../../Camera.h"
#include "../../../Core/Log.h"
#include <glad/glad.h>

namespace BerkEngine {

OpenGLBackend::OpenGLBackend() {
    BE_CORE_INFO("OpenGL Backend created");
}

OpenGLBackend::~OpenGLBackend() {
    Shutdown();
}

void OpenGLBackend::Init(uint32_t width, uint32_t height) {
    m_Width = width;
    m_Height = height;

    glViewport(0, 0, (GLint)width, (GLint)height);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    m_Initialized = true;
    BE_CORE_INFO("OpenGL Backend initialized (viewport: {}x{})", width, height);
}

void OpenGLBackend::Shutdown() {
    m_Initialized = false;
    BE_CORE_INFO("OpenGL Backend shutdown");
}

void OpenGLBackend::BeginFrame() {
    Clear(0.1f, 0.1f, 0.1f, 1.0f);
}

void OpenGLBackend::EndFrame() {
    // Frame post-processing (future)
}

void OpenGLBackend::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLBackend::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    glViewport(x, y, w, h);
}

void OpenGLBackend::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void OpenGLBackend::EnableDepthTest(bool enable) {
    if (enable)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void OpenGLBackend::EnableFaceCulling(bool enable) {
    if (enable) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

std::unique_ptr<Shader> OpenGLBackend::CreateShader(const std::string& vertPath, const std::string& fragPath) {
    return std::make_unique<OpenGLShader>(vertPath, fragPath);
}

std::unique_ptr<Mesh> OpenGLBackend::CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    return std::make_unique<OpenGLMesh>(vertices, indices);
}

void OpenGLBackend::Submit(const Mesh& mesh, const glm::mat4& transform, const Shader& shader, const Camera& camera) {
    shader.Bind();
    
    // Cast to OpenGL specific types
    const OpenGLShader& glShader = static_cast<const OpenGLShader&>(shader);
    const OpenGLMesh& glMesh = static_cast<const OpenGLMesh&>(mesh);
    
    glShader.SetMat4("u_Transform", transform);
    glShader.SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

    glMesh.Draw();
}

} // namespace BerkEngine
