#include "enum_utils.h"
#include "system_events.h"
#include "traffic_states.h"

std::string EnumUtils::state_to_string(TrafficState state) {
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
std::string EnumUtils::event_to_string(SystemEvent event) {
    switch (event) {
    case SystemEvent::TIME_EXPIRED:
        return "TIME_EXPIRED";
    case SystemEvent::BUTTON_PRESSED:
        return "BUTTON_PRESSED";
    default:
        return "UNKNOWN_EVENT";
    }
}
