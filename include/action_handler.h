#pragma once

enum class TrafficState;
enum class SystemEvent;

/**
 * @brief Interface for handling state transitions
 */
class IActionHandler {
  public:
    virtual ~IActionHandler() = default;
    virtual void handle(TrafficState current_state, SystemEvent event,
                        TrafficState next_state) = 0;
};
