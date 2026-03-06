#pragma once

#include "ComponentArray.h"
#include "Types.h"

#include <cassert>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace BerkEngine {

// ComponentManager owns all ComponentArrays and provides the single interface
// for adding, removing and querying components from the rest of the engine.
class ComponentManager {
public:
    template <typename T>
    void registerComponent() {
        std::type_index typeName = typeid(T);
        assert(mComponentTypes.find(typeName) == mComponentTypes.end() &&
               "Registering component type more than once.");

        mComponentTypes.insert({typeName, mNextComponentType});
        mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
        ++mNextComponentType;
    }

    template <typename T>
    ComponentType getComponentType() {
        std::type_index typeName = typeid(T);
        assert(mComponentTypes.find(typeName) != mComponentTypes.end() &&
               "Component not registered before use.");
        return mComponentTypes[typeName];
    }

    template <typename T>
    void addComponent(Entity entity, T component) {
        getComponentArray<T>()->insertData(entity, component);
    }

    template <typename T>
    void removeComponent(Entity entity) {
        getComponentArray<T>()->removeData(entity);
    }

    template <typename T>
    T& getComponent(Entity entity) {
        return getComponentArray<T>()->getData(entity);
    }

    template <typename T>
    bool hasComponent(Entity entity) {
        return getComponentArray<T>()->hasData(entity);
    }

    void entityDestroyed(Entity entity) {
        for (auto const& pair : mComponentArrays) {
            pair.second->entityDestroyed(entity);
        }
    }

private:
    std::unordered_map<std::type_index, ComponentType>                    mComponentTypes{};
    std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> mComponentArrays{};
    ComponentType                                                          mNextComponentType{0};

    template <typename T>
    std::shared_ptr<ComponentArray<T>> getComponentArray() {
        std::type_index typeName = typeid(T);
        assert(mComponentTypes.find(typeName) != mComponentTypes.end() &&
               "Component not registered before use.");
        return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
    }
};

} // namespace BerkEngine
