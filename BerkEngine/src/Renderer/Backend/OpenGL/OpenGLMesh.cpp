#include "OpenGLMesh.h"
#include "../../../Core/Log.h"
#include <glad/glad.h>
#include <iostream>

namespace BerkEngine {

OpenGLMesh::OpenGLMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    SetupMesh(vertices, indices);
}

OpenGLMesh::OpenGLMesh(OpenGLMesh&& other) noexcept
    : m_VAO(other.m_VAO),
      m_VBO(other.m_VBO),
      m_EBO(other.m_EBO),
      m_VertexCount(other.m_VertexCount),
      m_IndexCount(other.m_IndexCount)
{
    other.m_VAO = 0;
    other.m_VBO = 0;
    other.m_EBO = 0;
    other.m_VertexCount = 0;
    other.m_IndexCount = 0;
}

OpenGLMesh& OpenGLMesh::operator=(OpenGLMesh&& other) noexcept {
    if (this != &other) {
        Release();

        m_VAO = other.m_VAO;
        m_VBO = other.m_VBO;
        m_EBO = other.m_EBO;
        m_VertexCount = other.m_VertexCount;
        m_IndexCount = other.m_IndexCount;

        other.m_VAO = 0;
        other.m_VBO = 0;
        other.m_EBO = 0;
        other.m_VertexCount = 0;
        other.m_IndexCount = 0;
    }

    return *this;
}

OpenGLMesh::~OpenGLMesh() {
    Release();
}

void OpenGLMesh::SetupMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    m_VertexCount = static_cast<uint32_t>(vertices.size());
    m_IndexCount = static_cast<uint32_t>(indices.size());

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(uint32_t)), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
}

void OpenGLMesh::Release() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
    if (m_EBO) glDeleteBuffers(1, &m_EBO);

    m_VAO = 0;
    m_VBO = 0;
    m_EBO = 0;
    m_VertexCount = 0;
    m_IndexCount = 0;
}

void OpenGLMesh::Draw() const {
    if (!m_VAO) {
        std::cerr << "ERROR: Mesh::Draw() - VAO is 0!\n";
        return;
    }
    if (m_IndexCount == 0) {
        std::cerr << "ERROR: Mesh::Draw() - IndexCount is 0!\n";
        return;
    }
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

} // namespace BerkEngine
