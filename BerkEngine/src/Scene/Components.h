#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace BerkEngine {

    // Forward declarations
    class Mesh;

    // Her entity'nin bir ismi (etiketi) olması için
    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : Tag(tag) {}
    };

    // Entity'nin dünyadaki konumu, rotasyonu ve ölçeği
    struct TransformComponent
    {
        glm::mat4 Transform{ 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform) : Transform(transform) {}

        // Kolaylık: Transform nesnesini doğrudan mat4 gibi kullanabilmek için
        operator glm::mat4& () { return Transform; }
        operator const glm::mat4& () const { return Transform; }
    };

    // Entity'nin 3D model verisi
    struct MeshComponent
    {
        Mesh* MeshPtr = nullptr;
        
        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;
        MeshComponent(Mesh* mesh) : MeshPtr(mesh) {}
    };

}