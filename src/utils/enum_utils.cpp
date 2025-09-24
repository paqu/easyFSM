#include "enum_utils.h"
#include "elevator_events.h"
#include "elevator_states.h"
#include "system_events.h"
#include "traffic_states.h"

// Traffic light implementations (existing)
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

// Elevator implementations (new)
std::string EnumUtils::elevator_state_to_string(ElevatorState state) {
    switch (state) {
    case ElevatorState::IDLE:
        return "IDLE";
    case ElevatorState::DOORS_OPENING:
        return "DOORS_OPENING";
    case ElevatorState::DOORS_OPEN:
        return "DOORS_OPEN";
    case ElevatorState::DOORS_CLOSING:
        return "DOORS_CLOSING";
    case ElevatorState::MOVING_UP:
        return "MOVING_UP";
    case ElevatorState::MOVING_DOWN:
        return "MOVING_DOWN";
    case ElevatorState::EMERGENCY_STOP:
        return "EMERGENCY_STOP";
    default:
        return "UNKNOWN_ELEVATOR_STATE";
    }
}

std::string EnumUtils::elevator_event_to_string(ElevatorEvent event) {
    switch (event) {
    case ElevatorEvent::FLOOR_REQUESTED:
        return "FLOOR_REQUESTED";
    case ElevatorEvent::DOORS_OPEN_REQUESTED:
        return "DOORS_OPEN_REQUESTED";
    case ElevatorEvent::DOORS_CLOSE_REQUESTED:
        return "DOORS_CLOSE_REQUESTED";
    case ElevatorEvent::TIMER_EXPIRED:
        return "TIMER_EXPIRED";
    case ElevatorEvent::FLOOR_REACHED:
        return "FLOOR_REACHED";
    case ElevatorEvent::EMERGENCY_BUTTON:
        return "EMERGENCY_BUTTON";
    case ElevatorEvent::OBSTACLE_DETECTED:
        return "OBSTACLE_DETECTED";
    default:
        return "UNKNOWN_ELEVATOR_EVENT";
    }
}
