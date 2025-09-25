#pragma once

namespace state_machine {
/**
 * @brief Interface for handling state transitions
 */
template <typename StateType, typename EventType> class IActionHandler {
  public:
    virtual ~IActionHandler() = default;
    virtual void handle(StateType current_state, EventType event,
                        StateType next_state) = 0;
};
} // namespace state_machine
