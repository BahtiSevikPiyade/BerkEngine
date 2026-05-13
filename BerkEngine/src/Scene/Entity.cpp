#include "bepch.h"
#include "Entity.h"

namespace BerkEngine {

    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }

    /* Not: AddComponent, GetComponent gibi template fonksiyonlar 
       Entity.h dosyasında kalmalı. Çünkü derleyici, bu fonksiyonların 
       hangi tiplerle (TransformComponent vb.) çağrılacağını bilmek zorundadır.
    */

}