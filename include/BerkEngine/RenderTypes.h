#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace BerkEngine {

struct Vec2 {
    float x{0.0F};
    float y{0.0F};
};

struct Vec3 {
    float x{0.0F};
    float y{0.0F};
    float z{0.0F};
};

struct Color {
    std::uint8_t r{255};
    std::uint8_t g{255};
    std::uint8_t b{255};
    std::uint8_t a{255};
};

struct Camera {
    enum class Projection : std::uint8_t {
        Orthographic = 0,
        Perspective
    };

    Projection projection{Projection::Orthographic};
    Vec3 position{};
    Vec3 target{0.0F, 0.0F, -1.0F};
    Vec3 up{0.0F, 1.0F, 0.0F};
    float orthographicHeight{10.0F};
    float fieldOfViewDegrees{60.0F};
    float nearClip{0.1F};
    float farClip{1000.0F};
};

struct SpriteDrawCommand {
    std::string textureName{};
    std::string materialName{};
    Vec2 position{};
    Vec2 size{1.0F, 1.0F};
    float rotationRadians{0.0F};
    std::int32_t sortLayer{0};
    Color tint{};
};

struct MeshDrawCommand {
    std::string meshName{};
    std::string materialName{};
    Vec3 position{};
    Vec3 rotationEulerRadians{};
    Vec3 scale{1.0F, 1.0F, 1.0F};
    std::int32_t sortLayer{0};
    Color tint{};
};

using RenderCommand = std::variant<SpriteDrawCommand, MeshDrawCommand>;

class RenderQueue {
public:
    void setCamera(Camera camera) {
        mCamera = std::move(camera);
    }

    const Camera& camera() const {
        return mCamera;
    }

    void submitSprite(const SpriteDrawCommand& command) {
        mCommands.emplace_back(command);
    }

    void submitSprite(SpriteDrawCommand&& command) {
        mCommands.emplace_back(std::move(command));
    }

    void submitMesh(const MeshDrawCommand& command) {
        mCommands.emplace_back(command);
    }

    void submitMesh(MeshDrawCommand&& command) {
        mCommands.emplace_back(std::move(command));
    }

    const std::vector<RenderCommand>& commands() const {
        return mCommands;
    }

    std::size_t size() const {
        return mCommands.size();
    }

    bool empty() const {
        return mCommands.empty();
    }

    void clear() {
        mCommands.clear();
    }

private:
    Camera mCamera{};
    std::vector<RenderCommand> mCommands{};
};

} // namespace BerkEngine
