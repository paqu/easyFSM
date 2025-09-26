#pragma once

// Forward declaration
enum class ElevatorState;
enum class ElevatorEvent;

#include <string>

class ElevatorEnumUtils {
  public:
    // Elevator utilities
    static std::string elevator_state_to_string(ElevatorState state);
    static std::string elevator_event_to_string(ElevatorEvent event);
};
