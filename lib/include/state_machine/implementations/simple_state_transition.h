#pragma once
#include "../core/state_transition.h"

namespace state_machine {

/**
 * @brief Simple unconditional state transition
 */

template <typename StateType, typename EventType>
class SimpleStateTransition : public IStateTransition<StateType, EventType> {
  private:
    StateType from_state;
    EventType trigger_event;
    StateType to_state;

  public:
    SimpleStateTransition(StateType from_state, EventType trigger_event,
                          StateType to_state)
        : from_state(from_state), trigger_event(trigger_event),
          to_state(to_state) {}

    StateType get_from_state() const override { return from_state; }
    EventType get_trigger_event() const override { return trigger_event; }
    StateType get_to_state() const override { return to_state; }

    bool can_transition(StateType current_state,
                        EventType event) const override {
        return this->from_state == current_state &&
               this->trigger_event == event;
    }
};
} // namespace state_machine
