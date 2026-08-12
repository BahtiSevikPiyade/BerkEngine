#pragma once

#include "EventQueue.h"

namespace BerkEngine {

class IPlatform {
public:
    virtual ~IPlatform() = default;

    virtual void initialize() {}
    virtual void pollEvents(EventQueue& /*events*/) {}
    virtual bool shouldClose() const { return false; }
    virtual void shutdown() {}
};

} // namespace BerkEngine
