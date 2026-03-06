#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

#include <memory>

namespace BerkEngine {

// World is the central coordinator of the ECS.  It owns the three managers and
// provides a single, clean API that game code interacts with.
class World {
public:
    World()
        : mComponentManager(std::make_unique<ComponentManager>())
        , mEntityManager(std::make_unique<EntityManager>())
        , mSystemManager(std::make_unique<SystemManager>())
    {}

    // -------------------------------------------------------------------------
    // Entity API
    // -------------------------------------------------------------------------

    Entity createEntity() {
        return mEntityManager->createEntity();
    }

    void destroyEntity(Entity entity) {
        mEntityManager->destroyEntity(entity);
        mComponentManager->entityDestroyed(entity);
        mSystemManager->entityDestroyed(entity);
    }

    std::uint32_t getLivingEntityCount() const {
        return mEntityManager->getLivingEntityCount();
    }

    // -------------------------------------------------------------------------
    // Component API
    // -------------------------------------------------------------------------

    template <typename T>
    void registerComponent() {
        mComponentManager->registerComponent<T>();
    }

    template <typename T>
    void addComponent(Entity entity, T component) {
        mComponentManager->addComponent<T>(entity, component);

        auto signature = mEntityManager->getSignature(entity);
        signature.set(mComponentManager->getComponentType<T>());
        mEntityManager->setSignature(entity, signature);

        mSystemManager->entitySignatureChanged(entity, signature);
    }

    template <typename T>
    void removeComponent(Entity entity) {
        mComponentManager->removeComponent<T>(entity);

        auto signature = mEntityManager->getSignature(entity);
        signature.reset(mComponentManager->getComponentType<T>());
        mEntityManager->setSignature(entity, signature);

        mSystemManager->entitySignatureChanged(entity, signature);
    }

    template <typename T>
    T& getComponent(Entity entity) {
        return mComponentManager->getComponent<T>(entity);
    }

    template <typename T>
    bool hasComponent(Entity entity) {
        return mComponentManager->hasComponent<T>(entity);
    }

    template <typename T>
    ComponentType getComponentType() {
        return mComponentManager->getComponentType<T>();
    }

    // -------------------------------------------------------------------------
    // System API
    // -------------------------------------------------------------------------

    template <typename T>
    std::shared_ptr<T> registerSystem() {
        return mSystemManager->registerSystem<T>();
    }

    // Define which components an entity must have to be tracked by system T
    template <typename T>
    void setSystemSignature(Signature signature) {
        mSystemManager->setSignature<T>(signature);
    }

private:
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<EntityManager>    mEntityManager;
    std::unique_ptr<SystemManager>    mSystemManager;
};

} // namespace BerkEngine
