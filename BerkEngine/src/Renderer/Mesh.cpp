#include "bepch.h"
#include "Mesh.h"
#include "Backend/OpenGL/OpenGLMesh.h"

namespace BerkEngine {

std::unique_ptr<Mesh> Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    // For now, hardcoded to OpenGL. Later, can be selected via Renderer::GetBackendType()
    return std::make_unique<OpenGLMesh>(vertices, indices);
}

std::unique_ptr<Mesh> Mesh::CreateCube() {
    // Küp vertex'leri (positions ve normals)
    std::vector<Vertex> vertices = {
        // Front face
        {{ -0.5f, -0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }},  // 0
        {{  0.5f, -0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }},  // 1
        {{  0.5f,  0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }},  // 2
        {{ -0.5f,  0.5f,  0.5f }, { 0.0f,  0.0f,  1.0f }},  // 3

        // Back face
        {{ -0.5f, -0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }},  // 4
        {{ -0.5f,  0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }},  // 5
        {{  0.5f,  0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }},  // 6
        {{  0.5f, -0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }},  // 7

        // Top face
        {{ -0.5f,  0.5f, -0.5f }, { 0.0f,  1.0f,  0.0f }},  // 8
        {{ -0.5f,  0.5f,  0.5f }, { 0.0f,  1.0f,  0.0f }},  // 9
        {{  0.5f,  0.5f,  0.5f }, { 0.0f,  1.0f,  0.0f }},  // 10
        {{  0.5f,  0.5f, -0.5f }, { 0.0f,  1.0f,  0.0f }},  // 11

        // Bottom face
        {{ -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f,  0.0f }},  // 12
        {{  0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f,  0.0f }},  // 13
        {{  0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f,  0.0f }},  // 14
        {{ -0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f,  0.0f }},  // 15

        // Right face
        {{  0.5f, -0.5f, -0.5f }, { 1.0f,  0.0f,  0.0f }},  // 16
        {{  0.5f,  0.5f, -0.5f }, { 1.0f,  0.0f,  0.0f }},  // 17
        {{  0.5f,  0.5f,  0.5f }, { 1.0f,  0.0f,  0.0f }},  // 18
        {{  0.5f, -0.5f,  0.5f }, { 1.0f,  0.0f,  0.0f }},  // 19

        // Left face
        {{ -0.5f, -0.5f, -0.5f }, {-1.0f,  0.0f,  0.0f }},  // 20
        {{ -0.5f, -0.5f,  0.5f }, {-1.0f,  0.0f,  0.0f }},  // 21
        {{ -0.5f,  0.5f,  0.5f }, {-1.0f,  0.0f,  0.0f }},  // 22
        {{ -0.5f,  0.5f, -0.5f }, {-1.0f,  0.0f,  0.0f }},  // 23
    };

    // Küp index'leri (her yüz için 2 triangle = 6 index)
    std::vector<uint32_t> indices = {
        // Front face
        0, 1, 2,  0, 2, 3,
        // Back face
        4, 6, 5,  4, 7, 6,
        // Top face
        8, 10, 9,  8, 11, 10,
        // Bottom face
        12, 14, 13,  12, 15, 14,
        // Right face
        16, 18, 17,  16, 19, 18,
        // Left face
        20, 22, 21,  20, 23, 22,
    };

    return Mesh::Create(vertices, indices);
}

} // namespace BerkEngine

// Out-of-line destructor definition must be in the BerkEngine namespace
namespace BerkEngine {
    Mesh::~Mesh() {}
}
