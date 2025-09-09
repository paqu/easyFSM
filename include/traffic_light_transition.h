#pragma once
#include "state_transition.h"
#include "system_events.h"
#include "traffic_states.h"
#include <functional>

/**
 * @brief Conditional transition based on pedestrian request
 */
class TrafficLightTransition : public IStateTransition {
  private:
    TrafficState from_state;
    SystemEvent trigger_event;
    TrafficState to_state_normal;
    TrafficState to_state_with_pedestrian;
    std::function<bool()> has_pedestrian_request;
    bool check_pedestrian_request;

  public:
    // Simple transition constructor
    TrafficLightTransition(TrafficState from, SystemEvent event,
                           TrafficState to);

    // Conditional transition constructor
    TrafficLightTransition(TrafficState from, SystemEvent event,
                           TrafficState to_normal, TrafficState to_pedestrian,
                           std::function<bool()> ped_check);

    TrafficState get_from_state() const override;
    SystemEvent get_trigger_event() const override;
    TrafficState get_to_state() const override;
    bool can_transition(TrafficState current_state,
                        SystemEvent event) const override;
};
