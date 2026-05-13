#pragma once
#include "entt/entt.hpp"

namespace BerkEngine {

    class Entity; // Forward declaration
    class Camera;
    class Shader;

    class Scene {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnUpdate(float ts);

        // Camera management
        Camera* GetCamera() { return m_Camera; }
        void SetCamera(Camera* camera) { m_Camera = camera; }

        // Shader management
        Shader* GetShader() { return m_Shader; }
        void SetShader(Shader* shader) { m_Shader = shader; }

    private:
        entt::registry m_Registry;
        Camera* m_Camera = nullptr;
        Shader* m_Shader = nullptr;
        int m_FrameCount = 0;  // Teşhis için frame sayısı
        friend class Entity; // Entity sınıfının registry'ye erişmesi için
    };
}