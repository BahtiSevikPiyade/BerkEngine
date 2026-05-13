#include "bepch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

#include "../Renderer/Renderer.h"
#include "../Renderer/Camera.h"
#include "../Renderer/Mesh.h"
#include "../Core/Log.h"

#include <glad/glad.h>

#include <glm/glm.hpp>

namespace BerkEngine {

    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        
        // Her entity'nin bir ismi ve transformu olması standarttır
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;

        entity.AddComponent<TransformComponent>();

        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_Registry.destroy(entity);
    }

    void Scene::OnUpdate(float ts)
    {
        m_FrameCount++;
        if (m_FrameCount <= 5) {
            BE_CORE_INFO("Scene::OnUpdate frame {}, dt={}", m_FrameCount, ts);
        }

        // 1. Scripting / Logic (İleride buraya script sistemini bağlayacağız)

        // 2. Render Sistemi
        Renderer::BeginFrame();
        // Debug: ilk birkaç frame'de wireframe mod ve culling kapatılarak
        // geometrinin görünürlüğü test edilir.
        if (m_FrameCount <= 5) {
            glDisable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        
        // Camera'yı Renderer'a kayıt et
        if (m_Camera)
            Renderer::SetCamera(m_Camera);
        else
            BE_CORE_ERROR("Scene::OnUpdate - Camera is nullptr!");

        // Mesh ve Transform'u olan tüm entity'leri çiz
        auto view = m_Registry.view<TransformComponent, MeshComponent>();
        
        int entityCount = 0;
        view.each([&](auto entity, auto& transform, auto& mesh) {
            entityCount++;
            if (m_FrameCount <= 5) {
                BE_CORE_INFO("  Rendering entity {} (mesh: {})", entityCount, mesh.MeshPtr != nullptr ? "valid" : "nullptr");
            }
            
            if (mesh.MeshPtr && m_Camera && m_Shader) {
                if (m_FrameCount <= 5) {
                    BE_CORE_INFO("    Calling Renderer::Submit for entity {}", entityCount);
                }
                Renderer::Submit(*mesh.MeshPtr, transform.Transform, *m_Shader, *m_Camera);
            }
        });

        if (m_FrameCount <= 5) {
            BE_CORE_INFO("  Total entities with mesh: {}", entityCount);
        }

        Renderer::EndFrame();

        // Restore polygon mode after debug frames
        if (m_FrameCount <= 5) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_CULL_FACE);
        }
    }

}