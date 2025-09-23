#pragma once

// Forward declarations
enum class TrafficState;
enum class SystemEvent;

/**
 * @brief Interface for state transitions
 */
template <typename StateType, typename EventType> class IStateTransition {
  public:
    virtual ~IStateTransition() = default;
    virtual TrafficState get_from_state() const = 0;
    virtual SystemEvent get_trigger_event() const = 0;
    virtual TrafficState get_to_state() const = 0;
    virtual bool can_transition(StateType current_state,
                                EventType event) const = 0;
};
