#pragma once

#include "../../Mesh.h"

namespace BerkEngine {

class OpenGLMesh : public Mesh {
public:
    OpenGLMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    OpenGLMesh(OpenGLMesh&& other) noexcept;
    OpenGLMesh& operator=(OpenGLMesh&& other) noexcept;
    ~OpenGLMesh() override;

    void Draw() const override;
    uint32_t GetVertexCount() const override { return m_VertexCount; }
    uint32_t GetIndexCount() const override { return m_IndexCount; }

private:
    uint32_t m_VAO = 0;
    uint32_t m_VBO = 0;
    uint32_t m_EBO = 0;
    uint32_t m_VertexCount = 0;
    uint32_t m_IndexCount = 0;

    void SetupMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    void Release();
};

} // namespace BerkEngine
