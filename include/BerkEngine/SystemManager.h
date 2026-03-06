#pragma once

#include "System.h"
#include "Types.h"

#include <cassert>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace BerkEngine {

// SystemManager registers systems and keeps their required signatures in sync
// with the live entity population.
class SystemManager {
public:
    template <typename T>
    std::shared_ptr<T> registerSystem() {
        std::type_index typeName = typeid(T);
        assert(mSystems.find(typeName) == mSystems.end() &&
               "Registering system more than once.");

        auto system = std::make_shared<T>();
        mSystems.insert({typeName, system});
        return system;
    }

    // Set the component signature that entities must satisfy to be tracked by T
    template <typename T>
    void setSignature(Signature signature) {
        std::type_index typeName = typeid(T);
        assert(mSystems.find(typeName) != mSystems.end() &&
               "System used before being registered.");
        mSignatures[typeName] = signature;
    }

    // Remove a destroyed entity from every system's entity set
    void entityDestroyed(Entity entity) {
        for (auto const& pair : mSystems) {
            pair.second->mEntities.erase(entity);
        }
    }

    // Re-evaluate whether an entity belongs to each system after its signature changes
    void entitySignatureChanged(Entity entity, Signature entitySignature) {
        for (auto const& pair : mSystems) {
            auto const& type            = pair.first;
            auto const& system          = pair.second;
            auto const& systemSignature = mSignatures[type];

            if ((entitySignature & systemSignature) == systemSignature) {
                system->mEntities.insert(entity);
            } else {
                system->mEntities.erase(entity);
            }
        }
    }

private:
    std::unordered_map<std::type_index, Signature>            mSignatures{};
    std::unordered_map<std::type_index, std::shared_ptr<System>> mSystems{};
};

} // namespace BerkEngine
