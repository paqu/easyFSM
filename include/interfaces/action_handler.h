#pragma once

enum class TrafficState;
enum class TrafficEvent;

/**
 * @brief Interface for handling state transitions
 */
template <typename StateType, typename EventType> class IActionHandler {
  public:
    virtual ~IActionHandler() = default;
    virtual void handle(StateType current_state, EventType event,
                        StateType next_state) = 0;
};
