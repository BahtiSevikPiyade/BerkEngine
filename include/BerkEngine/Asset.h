#pragma once

#include "RenderTypes.h"

#include <cstdint>
#include <string>
#include <vector>

namespace BerkEngine {

struct TextureAsset {
    std::string name{};
    std::string sourcePath{};
    std::uint32_t width{0};
    std::uint32_t height{0};
};

struct ShaderAsset {
    std::string name{};
    std::string vertexSourcePath{};
    std::string fragmentSourcePath{};
};

struct MaterialAsset {
    std::string name{};
    std::string shaderName{};
    std::string textureName{};
    Color tint{};
};

struct MeshAsset {
    std::string name{};
    std::vector<Vec3> vertices{};
    std::vector<std::uint32_t> indices{};
};

} // namespace BerkEngine
