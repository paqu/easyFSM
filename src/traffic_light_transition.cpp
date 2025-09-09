#include "traffic_light_transition.h"

// Simple transition constructor
TrafficLightTransition::TrafficLightTransition(TrafficState from,
                                               SystemEvent event,
                                               TrafficState to)
    : from_state(from), trigger_event(event), to_state_normal(to),
      to_state_with_pedestrian(to), check_pedestrian_request(false) {}

// Conditional transition constructor
TrafficLightTransition::TrafficLightTransition(TrafficState from,
                                               SystemEvent event,
                                               TrafficState to_normal,
                                               TrafficState to_pedestrian,
                                               std::function<bool()> ped_check)
    : from_state(from), trigger_event(event), to_state_normal(to_normal),
      to_state_with_pedestrian(to_pedestrian),
      has_pedestrian_request(ped_check), check_pedestrian_request(true) {}

TrafficState TrafficLightTransition::get_from_state() const {
    return from_state;
}

SystemEvent TrafficLightTransition::get_trigger_event() const {
    return trigger_event;
}

TrafficState TrafficLightTransition::get_to_state() const {
    if (check_pedestrian_request && has_pedestrian_request &&
        has_pedestrian_request()) {
        return to_state_with_pedestrian;
    }
    return to_state_normal;
}

bool TrafficLightTransition::can_transition(TrafficState current_state,
                                            SystemEvent event) const {
    return this->from_state == current_state && this->trigger_event == event;
}
