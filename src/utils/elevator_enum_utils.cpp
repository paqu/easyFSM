#include "elevator_enum_utils.h"
#include "elevator_events.h"
#include "elevator_states.h"

// Elevator implementations (new)
std::string ElevatorEnumUtils::elevator_state_to_string(ElevatorState state) {
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

std::string ElevatorEnumUtils::elevator_event_to_string(ElevatorEvent event) {
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
