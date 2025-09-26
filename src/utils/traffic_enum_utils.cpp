#include "traffic_enum_utils.h"
#include "traffic_events.h"
#include "traffic_states.h"

// Traffic light implementations (existing)
std::string TrafficEnumUtils::state_to_string(TrafficState state) {
    switch (state) {
    case TrafficState::CAR_GREEN:
        return "CAR_GREEN";
    case TrafficState::CAR_YELLOW:
        return "CAR_YELLOW";
    case TrafficState::CAR_RED:
        return "CAR_RED";
    case TrafficState::WALK_PREP:
        return "WALK_PREP";
    case TrafficState::WALK:
        return "WALK";
    case TrafficState::WALK_FINISH:
        return "WALK_FINISH";
    case TrafficState::CAR_RED_YELLOW:
        return "CAR_RED_YELLOW";
    default:
        return "UNKNOWN_STATE";
    }
}

std::string TrafficEnumUtils::event_to_string(TrafficEvent event) {
    switch (event) {
    case TrafficEvent::TIME_EXPIRED:
        return "TIME_EXPIRED";
    case TrafficEvent::BUTTON_PRESSED:
        return "BUTTON_PRESSED";
    default:
        return "UNKNOWN_EVENT";
    }
}
