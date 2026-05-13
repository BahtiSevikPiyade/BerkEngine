#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace BerkEngine {

class Shader {
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetMat4(const std::string& name, const glm::mat4& value) const = 0;

    // Factory method - creates backend-specific shader
    static std::unique_ptr<Shader> Create(const std::string& vertPath, const std::string& fragPath);
};

} // namespace BerkEngine