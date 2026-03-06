#pragma once

#include "Types.h"

#include <array>
#include <cassert>
#include <queue>

namespace BerkEngine {

// EntityManager is responsible for distributing entity IDs and tracking
// which component signatures belong to each living entity.
class EntityManager {
public:
    EntityManager() {
        // Pre-fill the queue with all available IDs (starting at 1; 0 is reserved)
        for (Entity entity = 1; entity < MAX_ENTITIES; ++entity) {
            mAvailableEntities.push(entity);
        }
    }

    Entity createEntity() {
        assert(mLivingEntityCount < MAX_ENTITIES - 1 && "Too many entities in existence.");

        Entity id = mAvailableEntities.front();
        mAvailableEntities.pop();
        ++mLivingEntityCount;
        return id;
    }

    void destroyEntity(Entity entity) {
        assert(entity > 0 && entity < MAX_ENTITIES && "Entity out of range.");

        mSignatures[entity].reset();
        mAvailableEntities.push(entity);
        --mLivingEntityCount;
    }

    void setSignature(Entity entity, Signature signature) {
        assert(entity > 0 && entity < MAX_ENTITIES && "Entity out of range.");
        mSignatures[entity] = signature;
    }

    Signature getSignature(Entity entity) const {
        assert(entity > 0 && entity < MAX_ENTITIES && "Entity out of range.");
        return mSignatures[entity];
    }

    std::uint32_t getLivingEntityCount() const {
        return mLivingEntityCount;
    }

private:
    std::queue<Entity>               mAvailableEntities{};
    std::array<Signature, MAX_ENTITIES> mSignatures{};
    std::uint32_t                    mLivingEntityCount{0};
};

} // namespace BerkEngine
