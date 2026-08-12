#pragma once

#include <cstdint>

namespace BerkEngine {

enum class EventType : std::uint16_t {
    None = 0,
    Quit,
    WindowResized,
    KeyPressed,
    KeyReleased,
    MouseMoved,
    User
};

struct Event {
    EventType type{EventType::None};
    bool handled{false};
};

} // namespace BerkEngine
