#pragma once

// Forward declarations
enum class TrafficState;
enum class SystemEvent;

#include <string>

class EnumUtils {
  public:
    static std::string state_to_string(TrafficState state);
    static std::string event_to_string(SystemEvent event);
};
