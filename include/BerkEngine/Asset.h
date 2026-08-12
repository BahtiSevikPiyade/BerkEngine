#pragma once

#include <cstdint>
#include <string>

namespace BerkEngine {

struct TextureAsset {
    std::string name{};
    std::string sourcePath{};
    std::uint32_t width{0};
    std::uint32_t height{0};
};

} // namespace BerkEngine
