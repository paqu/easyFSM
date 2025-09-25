#pragma once
#include "../core/state_transition.h"
#include <functional>

namespace state_machine {
/**
 * @brief Generic conditional state transition
 * Chooses between two target states based on a condition function
 */
template <typename StateType, typename EventType>
class ConditionalStateTransition
    : public IStateTransition<StateType, EventType> {
  private:
    StateType from_state;
    EventType trigger_event;
    StateType to_state_normal;
    StateType to_state_conditional;
    std::function<bool()> condition_check;

  public:
    ConditionalStateTransition(StateType from, EventType event,
                               StateType to_normal, StateType to_conditional,
                               std::function<bool()> condition)
        : from_state(from), trigger_event(event), to_state_normal(to_normal),
          to_state_conditional(to_conditional), condition_check(condition) {}

    StateType get_from_state() const override { return from_state; }
    EventType get_trigger_event() const override { return trigger_event; }

    StateType get_to_state() const override {
        return (condition_check && condition_check()) ? to_state_conditional
                                                      : to_state_normal;
    }

    bool can_transition(StateType current_state,
                        EventType event) const override {
        return this->from_state == current_state &&
               this->trigger_event == event;
    }
};
} // namespace state_machine
