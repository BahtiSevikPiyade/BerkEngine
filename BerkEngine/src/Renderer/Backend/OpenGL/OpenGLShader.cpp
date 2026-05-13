#include "OpenGLShader.h"
#include "../../../Core/Log.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>

namespace BerkEngine {

std::string OpenGLShader::ReadFile(const std::string& filepath) {
    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in) {
        std::ostringstream contents;
        contents << in.rdbuf();
        result = contents.str();
    } else {
        BE_CORE_ERROR("Shader dosyasi bulunamadi: {}", filepath);
        return std::string();
    }
    return result;
}

OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vSource = ReadFile(vertexPath);
    std::string fSource = ReadFile(fragmentPath);
    Compile(vSource, fSource);
}

OpenGLShader::~OpenGLShader() {
    if (m_RendererID)
        glDeleteProgram(m_RendererID);
}

void OpenGLShader::Compile(const std::string& vertexSrc, const std::string& fragmentSrc) {
    if (vertexSrc.empty() || fragmentSrc.empty()) {
        BE_CORE_ERROR("Shader kaynak kodu yüklenemedi");
        m_RendererID = 0;
        return;
    }

    BE_CORE_INFO("Shader sources loaded: v={} bytes, f={} bytes", vertexSrc.length(), fragmentSrc.length());

    const char* vSrc = vertexSrc.c_str();
    const char* fSrc = fragmentSrc.c_str();

    // Vertex Shader
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vSrc, nullptr);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
        BE_CORE_ERROR("Vertex shader compile error: {}", infoLog);
        glDeleteShader(vertexShader);
        m_RendererID = 0;
        return;
    }
    BE_CORE_INFO("Vertex shader compiled successfully");

    // Fragment Shader
    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fSrc, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
        BE_CORE_ERROR("Fragment shader compile error: {}", infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        m_RendererID = 0;
        return;
    }
    BE_CORE_INFO("Fragment shader compiled successfully");

    // Link
    m_RendererID = glCreateProgram();
    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);
    glLinkProgram(m_RendererID);

    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(m_RendererID, 1024, NULL, infoLog);
        BE_CORE_ERROR("Shader link error: {}", infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(m_RendererID);
        m_RendererID = 0;
        return;
    }
    BE_CORE_INFO("Shader program linked successfully, ID={}", m_RendererID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenGLShader::Bind() const {
    if (m_RendererID)
        glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const {
    glUseProgram(0);
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) const {
    if (!m_RendererID) {
        BE_CORE_ERROR("Shader::SetMat4 - RendererID is 0!");
        return;
    }
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) {
        BE_CORE_ERROR("Shader::SetMat4 - Uniform '{}' not found!", name);
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

} // namespace BerkEngine
