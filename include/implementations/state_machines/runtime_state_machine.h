#pragma once
#include "state_machine.h"
#include "state_transition.h"
#include <algorithm>
#include <memory>
#include <set>
#include <vector>

/**
 * @brief Runtime configurable state machine
 */
template <typename StateType, typename EventType>
class RuntimeStateMachine : public IStateMachine<StateType, EventType> {
  private:
    StateType current_state;
    std::vector<std::unique_ptr<IStateTransition<StateType, EventType>>>
        transitions;
    std::set<StateType> states;
    std::set<EventType> events;

  public:
    explicit RuntimeStateMachine(StateType initial_state)
        : current_state(initial_state) {
        states.insert(initial_state);
    }

    StateType get_current_state() const override { return current_state; }

    void set_state(StateType state) override {
        states.insert(state);
        current_state = state;
    }

    void add_transition(std::unique_ptr<IStateTransition<StateType, EventType>>
                            transition) override {
        states.insert((transition->get_from_state()));
        states.insert((transition->get_to_state()));
        events.insert((transition->get_trigger_event()));
        transitions.push_back(std::move(transition));
    }

    StateType get_next_state(StateType current_state,
                             EventType event) const override {
        auto it = std::find_if(
            transitions.begin(), transitions.end(),
            [&current_state, &event](
                const std::unique_ptr<IStateTransition<StateType, EventType>>
                    &t) {
                return t->can_transition(static_cast<StateType>(current_state),
                                         static_cast<EventType>(event));
            });

        if (it != transitions.end()) {
            return static_cast<StateType>((*it)->get_to_state());
        }
        return current_state;
    }

    bool process_event(EventType event) override {
        StateType next_state = get_next_state(current_state, event);
        if (next_state != current_state) {
            current_state = next_state;
            return true;
        }
        return false;
    }

    std::vector<StateType> get_all_states() const override {
        return std::vector<StateType>(states.begin(), states.end());
    }

    std::vector<EventType> get_all_events() const override {
        return std::vector<EventType>(events.begin(), events.end());
    }
};
