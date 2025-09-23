#pragma once
#include <memory>
#include <vector>

class IStateTransition;

/**
 * @brief Generic state machine interface
 */
template <typename StateType, typename EventType> class IStateMachine {
  public:
    virtual ~IStateMachine() = default;
    virtual StateType get_current_state() const = 0;
    virtual bool process_event(EventType event) = 0;
    virtual StateType get_next_state(StateType current_state,
                                     EventType event) const = 0;
    virtual void
    add_transition(std::unique_ptr<IStateTransition> transition) = 0;
    virtual void set_state(StateType state) = 0;
    virtual std::vector<StateType> get_all_states() const = 0;
    virtual std::vector<EventType> get_all_events() const = 0;
};
