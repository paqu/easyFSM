#pragma once

// Forward declarations
enum class TrafficState;
enum class SystemEvent;
enum class ElevatorState;
enum class ElevatorEvent;

#include <string>

class EnumUtils {
  public:
    // Traffic light utilities
    static std::string state_to_string(TrafficState state);
    static std::string event_to_string(SystemEvent event);

    // Elevator utilities
    static std::string elevator_state_to_string(ElevatorState state);
    static std::string elevator_event_to_string(ElevatorEvent event);
};
