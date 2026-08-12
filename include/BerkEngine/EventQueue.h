#pragma once

#include "Event.h"

#include <cstddef>
#include <queue>
#include <utility>

namespace BerkEngine {

class EventQueue {
public:
    void push(const Event& event) {
        mEvents.push(event);
    }

    void push(Event&& event) {
        mEvents.push(std::move(event));
    }

    bool poll(Event& outEvent) {
        if (mEvents.empty()) {
            return false;
        }
        outEvent = mEvents.front();
        mEvents.pop();
        return true;
    }

    void clear() {
        while (!mEvents.empty()) {
            mEvents.pop();
        }
    }

    bool empty() const {
        return mEvents.empty();
    }

    std::size_t size() const {
        return mEvents.size();
    }

private:
    std::queue<Event> mEvents{};
};

} // namespace BerkEngine
