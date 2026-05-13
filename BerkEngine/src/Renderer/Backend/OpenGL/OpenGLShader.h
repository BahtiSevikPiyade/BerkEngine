#pragma once

#include "../../Shader.h"
#include <string>
#include <glm/glm.hpp>

namespace BerkEngine {

class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);
    ~OpenGLShader() override;

    void Bind() const override;
    void Unbind() const override;
    void SetMat4(const std::string& name, const glm::mat4& value) const override;

private:
    uint32_t m_RendererID = 0;

    std::string ReadFile(const std::string& filepath);
    void Compile(const std::string& vertexSrc, const std::string& fragmentSrc);
};

} // namespace BerkEngine
