#pragma once

namespace state_machine {
/**
 * @brief Interface for state transitions
 */
template <typename StateType, typename EventType> class IStateTransition {
  public:
    virtual ~IStateTransition() = default;
    virtual StateType get_from_state() const = 0;
    virtual EventType get_trigger_event() const = 0;
    virtual StateType get_to_state() const = 0;
    virtual bool can_transition(StateType current_state,
                                EventType event) const = 0;
};
} // namespace state_machine
