#include "simple_state_transition.h"

SimpleStateTransition::SimpleStateTransition(TrafficState from,
                                             SystemEvent event, TrafficState to)
    : from_state(from), trigger_event(event), to_state(to) {}

TrafficState SimpleStateTransition::get_from_state() const {
    return from_state;
}

SystemEvent SimpleStateTransition::get_trigger_event() const {
    return trigger_event;
}

TrafficState SimpleStateTransition::get_to_state() const { return to_state; }

bool SimpleStateTransition::can_transition(TrafficState current_state,
                                           SystemEvent event) const {
    return this->from_state == current_state && this->trigger_event == event;
}
