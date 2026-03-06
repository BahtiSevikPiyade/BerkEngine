#pragma once

#include "Types.h"

#include <array>
#include <cassert>
#include <unordered_map>

namespace BerkEngine {

// Interface so ComponentManager can erase components of any type when an
// entity is destroyed, without knowing the concrete component type.
class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(Entity entity) = 0;
};

// ComponentArray<T> stores every component of type T in a packed (contiguous)
// array.  It maintains two maps so that components can be looked up by entity
// and elements can be swapped out on removal to keep the array dense.
template <typename T>
class ComponentArray : public IComponentArray {
public:
    void insertData(Entity entity, T component) {
        assert(mEntityToIndex.find(entity) == mEntityToIndex.end() &&
               "Component added to the same entity more than once.");

        std::size_t newIndex      = mSize;
        mEntityToIndex[entity]    = newIndex;
        mIndexToEntity[newIndex]  = entity;
        mComponents[newIndex]     = component;
        ++mSize;
    }

    void removeData(Entity entity) {
        assert(mEntityToIndex.find(entity) != mEntityToIndex.end() &&
               "Removing a non-existent component.");

        // Swap the removed element with the last element to keep the array packed
        std::size_t indexOfRemoved  = mEntityToIndex[entity];
        std::size_t indexOfLast     = mSize - 1;
        mComponents[indexOfRemoved] = mComponents[indexOfLast];

        Entity entityOfLast               = mIndexToEntity[indexOfLast];
        mEntityToIndex[entityOfLast]      = indexOfRemoved;
        mIndexToEntity[indexOfRemoved]    = entityOfLast;

        mEntityToIndex.erase(entity);
        mIndexToEntity.erase(indexOfLast);
        --mSize;
    }

    T& getData(Entity entity) {
        assert(mEntityToIndex.find(entity) != mEntityToIndex.end() &&
               "Retrieving a non-existent component.");
        return mComponents[mEntityToIndex[entity]];
    }

    bool hasData(Entity entity) const {
        return mEntityToIndex.find(entity) != mEntityToIndex.end();
    }

    void entityDestroyed(Entity entity) override {
        if (mEntityToIndex.find(entity) != mEntityToIndex.end()) {
            removeData(entity);
        }
    }

private:
    std::array<T, MAX_ENTITIES>              mComponents{};
    std::unordered_map<Entity, std::size_t>  mEntityToIndex{};
    std::unordered_map<std::size_t, Entity>  mIndexToEntity{};
    std::size_t                              mSize{0};
};

} // namespace BerkEngine
