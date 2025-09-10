#include "runtime_state_machine.h"

RuntimeStateMachine::RuntimeStateMachine(TrafficState initial_state)
    : current_state(initial_state) {
    states.insert(initial_state);
}

TrafficState RuntimeStateMachine::get_current_state() const {
    return current_state;
}

void RuntimeStateMachine::set_state(TrafficState state) {
    states.insert(state);
    current_state = state;
}

void RuntimeStateMachine::add_transition(
    std::unique_ptr<IStateTransition> transition) {
    states.insert(transition->get_from_state());
    states.insert(transition->get_to_state());
    events.insert(transition->get_trigger_event());
    transitions.push_back(std::move(transition));
}

TrafficState RuntimeStateMachine::get_next_state(TrafficState current_state,
                                                 SystemEvent event) const {
    auto it = std::find_if(
        transitions.begin(), transitions.end(),
        [&current_state, &event](const std::unique_ptr<IStateTransition> &t) {
            return t->can_transition(current_state, event);
        });

    if (it != transitions.end()) {
        return (*it)->get_to_state();
    }
    return current_state;
}

bool RuntimeStateMachine::process_event(SystemEvent event) {

    TrafficState next_state = get_next_state(current_state, event);
    if (next_state != current_state) {
        current_state = next_state;
        return true;
    }
    return false;
}

std::vector<TrafficState> RuntimeStateMachine::get_all_states() const {
    return std::vector<TrafficState>(states.begin(), states.end());
}
std::vector<SystemEvent> RuntimeStateMachine::get_all_events() const {
    return std::vector<SystemEvent>(events.begin(), events.end());
}
