#pragma once

// Forward declarations
enum class TrafficState;
enum class TrafficEvent;

#include <string>

class TrafficEnumUtils {
  public:
    // Traffic light utilities
    static std::string state_to_string(TrafficState state);
    static std::string event_to_string(TrafficEvent event);
};
